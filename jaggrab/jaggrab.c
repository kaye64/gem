#include <jaggrab/jaggrab.h>

#include <string.h>
#include <stdlib.h>

#include <util/log.h>
#include <util/math.h>

#define LOG_TAG "jaggrab"
#define REQUEST_EXPR "JAGGRAB /([a-z]+)[0-9\\-]+\n\n"
#define JAG_BUFFER_SIZE 1024 * 4 // 4k buffer

archive_client_t* client_accept(int fd, struct in_addr addr, archive_server_t* server);
int client_handshake(archive_client_t* client);
void client_read(archive_client_t* client);
void client_write(archive_client_t* client);
void client_drop(archive_client_t* client);
int resolve_archive(const char* archive);

archive_server_t* jaggrab_create(cache_t* cache, const char* addr)
{
	archive_server_t* server = (archive_server_t*)malloc(sizeof(archive_server_t));
	server_t* base_server = &server->server;
	server->cache = cache;
	server_create(base_server, addr, 43595);
	base_server->buf_size = JAG_BUFFER_SIZE;
	base_server->accept_cb = (client_accept_t)&client_accept;
	base_server->handshake_cb = (client_handshake_t)&client_handshake;
	base_server->read_cb = (client_read_t)&client_read;
	base_server->write_cb = (client_write_t)&client_write;
	base_server->drop_cb = (client_drop_t)&client_drop;
	cache_gen_crc(cache, 0, server->crc_table);
	int regexp_res = regcomp(&server->request_regexp, REQUEST_EXPR, REG_EXTENDED|REG_NEWLINE);
	if (regexp_res != 0) {
		char error_message[1024 * 4];
		regerror(regexp_res, &server->request_regexp, error_message, 1024 * 4);
		ERROR("jaggrab_create: compiling request regexp failed\n\n%s", error_message);
		return NULL;
	}
	return server;
}

void jaggrab_start(archive_server_t* server, struct ev_loop* loop)
{
	server_t* base_server = (server_t*)server;
	server_start(base_server, loop);
	INFO("Listening on %s:%d", base_server->addr, base_server->port);
}

void jaggrab_free(archive_server_t* server)
{
	server_free(&server->server);
	regfree(&server->request_regexp);
	free(server);
}

archive_client_t* client_accept(int fd, struct in_addr addr, archive_server_t* server)
{
	archive_client_t* client = (archive_client_t*)malloc(sizeof(archive_client_t));
	client->file_buffer = 0;
	client->file_caret = 0;
	client->file_size = 0;
	return client;
}

int client_handshake(archive_client_t* client)
{
	return HANDSHAKE_ACCEPTED; // no handshake
}

void client_read(archive_client_t* client)
{
	client_t* base_client = &client->client;
	archive_server_t* server = (archive_server_t*)base_client->server;

	/* read in the request */
	char request_buffer[128];
	buffer_pushp(&base_client->read_buffer);
	size_t read = buffer_read(&base_client->read_buffer, request_buffer, 128);
	request_buffer[read] = '\0';

	/* extract the archive name */
	const int max_matches = 2;
	regmatch_t matches[max_matches];
	if (regexec(&server->request_regexp, request_buffer, max_matches, matches, 0)) {
		WARN("client_read: no matches in request string");
		buffer_popp(&base_client->read_buffer);
		return;
	}
	int start = matches[1].rm_so;
	int end = matches[1].rm_eo;
	int len = end-start;
	char request[len+1];
	request[0] = '\0';
	strncat(request, request_buffer+start, len);

	/* buffer the file to be written */
	int archive_id = resolve_archive(request);
	if (archive_id > 0) {
		client->file_size = cache_query_size(server->cache, 0, archive_id);
		client->file_buffer = (char*)malloc(sizeof(char)*client->file_size);

		int cache_res = cache_get(server->cache, 0, archive_id, client->file_buffer);
		if (cache_res == 0) {
			ERROR("client_read: unable to retrieve archive %s from cache", request);
			return;
		}
	} else {
		client->file_size = 80;
		client->file_buffer = (char*)malloc(sizeof(char)*80);
		memcpy(client->file_buffer, server->crc_table, 80);
	}
}

int resolve_archive(const char* archive)
{
	/* todo: this is fugly. re-implement with map later */
	if (!strcmp(archive, "crc")) {
		return 0;
	}
	if (!strcmp(archive, "title")) {
		return 1;
	}
	if (!strcmp(archive, "config")) {
		return 2;
	}
	if (!strcmp(archive, "interface")) {
		return 3;
	}
	if (!strcmp(archive, "media")) {
		return 4;
	}
	if (!strcmp(archive, "versionlist")) {
		return 5;
	}
	if (!strcmp(archive, "textures")) {
		return 6;
	}
	if (!strcmp(archive, "wordenc")) {
		return 7;
	}
	if (!strcmp(archive, "sounds")) {
		return 8;
	}
	return 0;
}

void client_write(archive_client_t* client)
{
	if (client->file_buffer == 0 || client->file_caret > client->file_size) {
		return;
	}
	client_t* base_client = &client->client;
	size_t to_write = min(client->file_size-client->file_caret, buffer_write_avail(&base_client->write_buffer));
	size_t written = buffer_write(&base_client->write_buffer, client->file_buffer+client->file_caret, to_write);
	client->file_caret += written;
}

void client_drop(archive_client_t* client)
{
	if (client->file_buffer != 0) {
		free(client->file_buffer);
	}
	free(client);
}
