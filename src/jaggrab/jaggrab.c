/**
 *  This file is part of Gem.
 *
 *  Gem is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Gem is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Gem.  If not, see <http://www.gnu.org/licenses/\>.
 */

/**
 * jaggrab.c
 *
 * The jaggrab server
 */
#include <jaggrab/jaggrab.h>

#include <string.h>
#include <stdlib.h>
#include <runite/util/math.h>
#include <runite/util/container_of.h>

#include <util/log.h>

#define LOG_TAG "jaggrab"
#define REQUEST_EXPR "JAGGRAB /([a-z]+)[0-9\\-]+\n\n"
#define JAG_BUFFER_SIZE 1024 * 4 // 4k buffer

client_t* jaggrab_accept(int fd, struct in_addr addr, server_t* server);
int jaggrab_handshake(client_t* client, server_t* server);
void jaggrab_read(client_t* client, server_t* server);
void jaggrab_write(client_t* client, server_t* server);
void jaggrab_drop(client_t* client, server_t* server);
int resolve_archive(const char* archive);

/**
 * Allocates and creates an archive_server_t
 */
static void jaggrab_init(archive_server_t* server)
{
	server_t* base_server = &server->io_server;
	object_init(server, base_server);
	base_server->buf_size = JAG_BUFFER_SIZE;
	base_server->accept_cb = (client_accept_t)&jaggrab_accept;
	base_server->handshake_cb = (client_handshake_t)&jaggrab_handshake;
	base_server->read_cb = (client_read_t)&jaggrab_read;
	base_server->write_cb = (client_write_t)&jaggrab_write;
	base_server->drop_cb = (client_drop_t)&jaggrab_drop;
	int regexp_res = regcomp(&server->request_regexp, REQUEST_EXPR, REG_EXTENDED|REG_NEWLINE);
	if (regexp_res != 0) {
		char error_message[1024 * 4];
		regerror(regexp_res, &server->request_regexp, error_message, 1024 * 4);
		ERROR("jaggrab_create: compiling request regexp failed\n\n%s", error_message);
	}
}

/**
 * Properly frees the jaggrab server
 */
static void jaggrab_free(archive_server_t* server)
{
	object_free(&server->io_server);
	regfree(&server->request_regexp);
}

/**
 * Configures a jaggrab server
 *  - cache: The cache to serve
 */
void jaggrab_config(archive_server_t* server, cache_t* cache, const char* addr)
{
	server_t* base_server = &server->io_server;
	server_config(base_server, addr, 43595, SF_PARTIAL_READ);
	server->cache = cache;
	cache_gen_crc(cache, 0, &server->crc_table);
}

/**
 * Starts the server loop
 *  - loop: The event loop to listen on
 */
void jaggrab_start(archive_server_t* server, struct ev_loop* loop)
{
	server_t* base_server = &server->io_server;
	server_start(base_server, loop);
	INFO("Listening on %s:%d", base_server->addr, base_server->port);
}

/**
 * Validates/initializes a new client_t
 *  - fd: The client's file descriptor
 *  - addr: The client's address
 * returns: A properly allocated client_t, NULL on client denied
 */
client_t* jaggrab_accept(int fd, struct in_addr addr, server_t* server)
{
	archive_client_t* client = (archive_client_t*)malloc(sizeof(archive_client_t));
	client->file_caret = 0;
	client->file = NULL;
	return &client->io_client;
}

/**
 * Performs any handshake routines between the client/server
 * returns: One of HANDSHAKE_{DENIED,PENDING,ACCEPTED}
 */
int jaggrab_handshake(client_t* client, server_t* server)
{
	return HANDSHAKE_ACCEPTED; // no handshake
}

/**
 * Called when data is available to be read by the client
 */
void jaggrab_read(client_t* client, server_t* server)
{
	archive_server_t* archive_server = container_of(client->server, archive_server_t, io_server);
	archive_client_t* archive_client = container_of(client, archive_client_t, io_client);

	/* read in the request */
	char request_buffer[128];
	buffer_pushp(&client->read_buffer);
	size_t read = buffer_read(&client->read_buffer, (unsigned char*)request_buffer, 128);
	request_buffer[read] = '\0';

	/* extract the archive name */
	const int max_matches = 2;
	regmatch_t matches[max_matches];
	if (regexec(&archive_server->request_regexp, request_buffer, max_matches, matches, 0)) {
		WARN("jaggrab_read: no matches in request string");
		buffer_popp(&client->read_buffer);
		return;
	}
	buffer_dropp(&client->read_buffer);
	int start = matches[1].rm_so;
	int end = matches[1].rm_eo;
	int len = end-start;
	char request[len+1];
	request[0] = '\0';
	strncat(request, request_buffer+start, len);

	/* buffer the file to be written */
	int archive_id = resolve_archive(request);
	if (archive_id > 0) {
		archive_client->file = cache_get_file(archive_server->cache, 0, archive_id);
		if (!archive_client->file) {
			ERROR("jaggrab_read: unable to retrieve archive %s from cache", request);
			return;
		}
	} else {
		archive_client->file = &archive_server->crc_table;
	}
}

/**
 * Maps an archive name to its numerical index
 */
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

/**
 * Called to signal that we can write to the client
 */
void jaggrab_write(client_t* client, server_t* server)
{
	archive_client_t* archive_client = container_of(client, archive_client_t, io_client);
	if (archive_client->file == NULL) {
		return;
	}
	file_t* cache_file = archive_client->file;
	size_t to_write = cache_file->length - archive_client->file_caret;
	to_write = min(to_write, buffer_write_avail(&client->write_buffer));
	size_t written = buffer_write(&client->write_buffer, cache_file->data+archive_client->file_caret, to_write);
	archive_client->file_caret += written;
}

/**
 * Called to perform any client cleanup
 */
void jaggrab_drop(client_t* client, server_t* server)
{
	archive_client_t* archive_client = container_of(client, archive_client_t, io_client);
	free(archive_client);
}

object_proto_t archive_server_proto = {
	.init = (object_init_t)jaggrab_init,
	.free = (object_free_t)jaggrab_free
};
