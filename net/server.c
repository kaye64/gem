#include <net/server.h>

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <core/log.h>

#define LOG_TAG "server"


void accept_cb(struct ev_loop* loop, accept_io_t* watcher, int revents);
void client_io_avail(struct ev_loop *loop, client_t* client, int revents);

server_t* server_create(server_t* server, const char* addr, int port)
{
	if (server == NULL) {
		server = (server_t*)malloc(sizeof(server_t));
		server->must_free = 1;
	} else {
		server->must_free = 0;
	}
	strcpy(server->addr, addr);
	server->port = port;
	server->fd = -1;
	server->buf_size = SERVER_DEFAULT_BUFFER_SIZE;
	return server;
}

void server_free(server_t* server)
{
	if (server->must_free) {
		free(server);
	}
}

int server_start(server_t* server)
{
	server->io_loop = ev_loop_new(EVBACKEND_EPOLL | EVFLAG_NOENV);
	server->fd = socket(PF_INET, SOCK_STREAM, 0);
	if (server->fd < 0) {
		ERROR("server_start: socket open failed");
		return 0;
	}

	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(server->port);
	address.sin_addr.s_addr = inet_addr(server->addr);

	if (bind(server->fd, (struct sockaddr*)&address, sizeof(address)) != 0) {
		ERROR("server_start: bind failed");
		return 0;
	}

	if (listen(server->fd, 0) < 0) {
		ERROR("server_start: listen failed");
		return 0;
	}

	ev_io_init((struct ev_io*)&server->io_accept, accept_cb, server->fd, EV_READ);
	server->io_accept.server = server;
	ev_io_start(server->io_loop, (struct ev_io*)&server->io_accept);
	return 1;
}

void accept_cb(struct ev_loop* loop, accept_io_t* accept_io, int revents)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	server_t* server = accept_io->server;

	if (EV_ERROR & revents) {
		ERROR("accept_cb: invalid event");
		return;
	}

	int fd = accept(server->fd, (struct sockaddr*)&client_addr, &client_len);
	if (fd < 0) {
		ERROR("accept_cb: accept failed");
		return;
	}

	client_t* client = server_client_init(server, fd, client_addr.sin_addr);
	if (client == NULL) {
		close(fd);
		INFO("accept_cb: client dropped");
		return;
	}

	INFO("accept_cb: accepted new client from %s", inet_ntoa(client->addr));

	ev_io_init((struct ev_io*)client, client_io_avail, client->fd, EV_READ|EV_WRITE);
	ev_io_start(loop, (struct ev_io*)client);

	if (!server->handshake_cb(client)) {
		server_client_drop(server, client);
		INFO("accept_cb: handshake failed. dropping");
		return;
	}
}

void server_client_drop(server_t* server, client_t* client)
{
	ev_io_stop(server->io_loop, (struct ev_io*)client);
	close(client->fd);
	server_client_cleanup(server, client);
}

void client_io_avail(struct ev_loop *loop, client_t* client, int revents)
{
	// Check for errors
	if (EV_ERROR & revents) {
		ERROR("client_io_avail: invalid event");
		return;
	}

	server_t* server = client->server;
	char buffer[server->buf_size];	// todo: work out a better place to put this buffer

	if (revents & EV_READ) {
		size_t read_avail = recv(client->fd, buffer, server->buf_size, 0);

		// Check for read error
		if (read_avail < 0) {
			ERROR("client_io_avail: recv error");
			return;
		}

		// Check for client drop
		if (read_avail == 0) {
			server_client_drop(server, client);
			INFO("client_io_avail: client dropped");
			return;
		}

		int buffered = io_buf_write(&client->read_buffer, buffer, read_avail);
		if (buffered != read_avail) {
			WARN("client_io_avail: buffer overflow. dropping %i bytes", read_avail-buffered);
		}

		INFO("read %i", buffered);
	}

	if (revents & EV_WRITE) { // flush write buffer if necessary
		size_t write_avail = io_buf_read(&client->write_buffer, buffer, server->buf_size);
		int write_caret = 0;
		while (write_avail > write_caret) {
			size_t sent = send(client->fd, buffer+write_caret, write_avail, 0);

			// Check for write error
			if (sent < 0) {
				ERROR("client_io_avail: send error");
				return;
			}

			// Check for client drop
			if (sent == 0) {
				server_client_drop(server, client);
				INFO("client_io_avail: client dropped");
				return;
			}

			write_caret += sent;
			INFO("wrote %i", sent);
		}
	}
}

client_t* server_client_init(server_t* server, int fd, struct in_addr addr)
{
	client_t* client = server->accept_cb(fd, addr, server);
	if (client == NULL) {
		return NULL;
	}
	client->fd = fd;
	client->addr = addr;
	client->server = server;
	io_buf_create(&client->read_buffer, server->buf_size);
	io_buf_create(&client->write_buffer, server->buf_size);
	return client;
}

void server_client_cleanup(server_t* server, client_t* client)
{
	io_buf_free(&client->read_buffer);
	io_buf_free(&client->write_buffer);
	server->drop_cb(client);
}

void server_stop(server_t* server)
{
	ev_io_stop(server->io_loop, (struct ev_io*)&server->io_accept);
	close(server->fd);
}

void server_poll(server_t* server, int block)
{
	ev_loop(server->io_loop, (block ? 0 : EVRUN_NOWAIT));
}
