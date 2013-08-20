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
 * server.c
 *
 * Defines and provides access to a generic network server
 */
#include <net/server.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <runite/util/container_of.h>

#include <util/log.h>

#define LOG_TAG "server"

void accept_cb(struct ev_loop* loop, struct ev_io* watcher, int revents);
void client_io_avail(struct ev_loop* loop, struct ev_io* io_read, int revents);

/**
 * Initializes a new server
 */
static void server_init(server_t* server)
{
	server->fd = -1;
	server->buf_size = DEFAULT_BUFFER_SIZE;
	server->io_loop = 0;
	server->buffer = NULL;
	object_init(list, &server->client_list);
}

/**
 * Properly frees a server_t
 */
static void server_free(server_t* server)
{
	/* force a cleanup of each client */
	while (!list_empty(&server->client_list)) {
		list_node_t* node = list_front(&server->client_list);
		client_t* client = container_of(node, client_t, node);
		server_client_cleanup(server, client);
	}

	object_free(&server->client_list);
	free(server->buffer);
}

/**
 * Configures a server
 */
void server_config(server_t* server, const char* addr, int port, uint8_t flags)
{
	strcpy(server->addr, addr);
	server->port = port;
	server->fd = -1;
	server->buf_size = DEFAULT_BUFFER_SIZE;
	server->buffer = (unsigned char*)malloc(server->buf_size);
	server->io_loop = 0;
	server->flags = flags;
	object_init(list, &server->client_list);
}

/**
 * Starts a server io loop
 *  - loop: The loop to run on
 * returns: true on success
 */
bool server_start(server_t* server, struct ev_loop* loop)
{
	server->io_loop = loop;
	server->fd = socket(PF_INET, SOCK_STREAM, 0);
	if (server->fd < 0) {
		ERROR("socket open failed");
		return false;
	}

	int temp = 1;
	setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(temp));

	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(server->port);
	address.sin_addr.s_addr = inet_addr(server->addr);

	if (bind(server->fd, (struct sockaddr*)&address, sizeof(address)) != 0) {
		ERROR("bind failed");
		return false;
	}

	if (listen(server->fd, 0) < 0) {
		ERROR("listen failed");
		return false;
	}

	ev_io_init(&server->io_accept, accept_cb, server->fd, EV_READ);
	ev_io_start(server->io_loop, &server->io_accept);
	return true;
}

/**
 * The ev client accept callback
 */
void accept_cb(struct ev_loop* loop, struct ev_io* accept_io, int revents)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	server_t* server = container_of(accept_io, server_t, io_accept);

	if (EV_ERROR & revents) {
		ERROR("invalid event");
		return;
	}

	int fd = accept(server->fd, (struct sockaddr*)&client_addr, &client_len);
	if (fd < 0) {
		ERROR("accept failed");
		return;
	}

	client_t* client = server_client_init(server, fd, client_addr.sin_addr);
	if (client == NULL) {
		close(fd);
		INFO("client dropped");
		return;
	}

	list_push_back(&server->client_list, &client->node);

	INFO("accepted new client from %s:%d", inet_ntoa(client->addr), server->port);

	ev_io_init(&client->io_read, client_io_avail, client->fd, EV_READ|EV_WRITE);
	ev_io_start(loop, &client->io_read);
}

/**
 * Cleanly disconnects a client
 */
void server_client_drop(server_t* server, client_t* client)
{
	client->client_drop = true;
}

/**
 * The ev client io callback
 */
void client_io_avail(struct ev_loop* loop, struct ev_io* io_read, int revents)
{
	client_t* client = container_of(io_read, client_t, io_read);
	// Check for errors
	if (EV_ERROR & revents) {
		ERROR("invalid event");
		return;
	}

	server_t* server = client->server;
	bool report_read = false;

	if (client->client_drop) {
		server_client_cleanup(server, client);
		return;
	}

	if (revents & EV_READ) {
		int read_avail = recv(client->fd, server->buffer, server->buf_size, 0);

		// Check for read error
		if (read_avail < 0) {
			switch (errno) {
			case ECONNRESET:
			case ETIMEDOUT:
				read_avail = 0; // drop the client later on
				break;
			default:
				ERROR("unable to handle error: %s", strerror(errno));
				break;
			}
			return;
		}

		// Check for client drop
		if (read_avail == 0) {
			server_client_drop(server, client);
			INFO("client dropped");
			return;
		}

		int buffered = buffer_write(&client->read_buffer, server->buffer, read_avail);
		if (buffered != read_avail) {
			WARN("buffer overflow. dropping %i bytes", read_avail-buffered);
		}


		if (client->handshake_stage == HANDSHAKE_ACCEPTED) {
			report_read = true;
		}
	}

	if (revents & EV_WRITE) { // flush write buffer if necessary
		if (client->handshake_stage == HANDSHAKE_ACCEPTED) {
			server->write_cb(client, server);
		}

		size_t write_avail = buffer_read(&client->write_buffer, server->buffer, server->buf_size);
		size_t write_caret = 0;
		while (write_avail > write_caret) {
			ssize_t sent = send(client->fd, server->buffer+write_caret, write_avail, 0);

			// Check for write error
			if (sent < 0) {
				switch (errno) {
				case ECONNRESET:
				case ETIMEDOUT:
					sent = 0; // drop the client later on
					break;
				default:
					ERROR("unable to handle error: %s", strerror(errno));
					break;
				}
				return;
			}

			// Check for client drop
			if (sent == 0) {
				server_client_drop(server, client);
				INFO("client dropped");
				return;
			}

			write_caret += sent;
		}
	}


	if (client->handshake_stage == HANDSHAKE_PENDING) {
		client->handshake_stage = server->handshake_cb(client, server);
		switch (client->handshake_stage) {
		case HANDSHAKE_DENIED:
			server_client_drop(server, client);
			INFO("handshake failed. dropping");
			break;
		case HANDSHAKE_ACCEPTED:
			// Notify client of any buffered data
			if (buffer_read_avail(&client->read_buffer) > 0) {
				report_read = true;
			}
			break;
		}
	}

	if (client->handshake_stage == HANDSHAKE_ACCEPTED &&
		buffer_read_avail(&client->read_buffer) > 0 &&
		server->flags & SF_PARTIAL_READ) {
		// re-report previously buffered but un-processed data
		report_read = true;
	}

	if (report_read) {
		server->read_cb(client, server);
	}
}

/**
 * Initializes a client
 *  - fd: The file descriptor of the client
 *  - addr: The address of the client
 */
client_t* server_client_init(server_t* server, int fd, struct in_addr addr)
{
	client_t* client = server->accept_cb(fd, addr, server);
	if (client == NULL) {
		return NULL;
	}
	client->fd = fd;
	client->addr = addr;
	client->server = server;
	client->handshake_stage = HANDSHAKE_PENDING;
	client->client_drop = false;
	object_init(buffer, &client->read_buffer);
	object_init(buffer, &client->write_buffer);
	buffer_realloc(&client->read_buffer, server->buf_size);
	buffer_realloc(&client->write_buffer, server->buf_size);
	return client;
}

/**
 * Cleans up a client for exit
 */
void server_client_cleanup(server_t* server, client_t* client)
{
	ev_io_stop(server->io_loop, &client->io_read);
	list_erase(&server->client_list, &client->node);
	object_free(&client->read_buffer);
	object_free(&client->write_buffer);
	server->drop_cb(client, server);
	close(client->fd);
}

/**
 * Stops the server io loop
 */
void server_stop(server_t* server)
{
	ev_io_stop(server->io_loop, &server->io_accept);
	close(server->fd);
}

object_proto_t server_proto = {
	.init = (object_init_t)server_init,
	.free = (object_free_t)server_free
};
