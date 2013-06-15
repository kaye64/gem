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

#include <util/log.h>
#include <util/container_of.h>

#define LOG_TAG "server"

void accept_cb(struct ev_loop* loop, struct ev_io* watcher, int revents);
void client_io_avail(struct ev_loop* loop, struct ev_io* io_read, int revents);

/**
 * server_create
 *
 * Initializes a new server
 *  - server: Some preallocated memory, or null to put on heap
 *  - addr: The address to bind to
 *  - port: The port to bind to
 *  - flags: Server flags. One of SF_*. See server.h
 * returns: The initialized server
 */
server_t* server_create(server_t* server, const char* addr, int port, uint8_t flags)
{
	if (server == NULL) {
		server = (server_t*)malloc(sizeof(server_t));
		server->must_free = true;
	} else {
		server->must_free = false;
	}
	strcpy(server->addr, addr);
	server->port = port;
	server->fd = -1;
	server->buf_size = SERVER_DEFAULT_BUFFER_SIZE;
	server->io_loop = 0;
	server->flags = flags;
	return server;
}

/**
 * server_free
 *
 * Properly frees a server_t
 *  - server: The server
 */
void server_free(server_t* server)
{
	if (server->must_free) {
		free(server);
	}
}

/**
 * server_start
 *
 * Starts a server io loop
 *  - server: The server
 *  - loop: The loop to run on
 * returns: true on success
 */
bool server_start(server_t* server, struct ev_loop* loop)
{
	server->io_loop = loop;
	server->fd = socket(PF_INET, SOCK_STREAM, 0);
	if (server->fd < 0) {
		ERROR("server_start: socket open failed");
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
		ERROR("server_start: bind failed");
		return false;
	}

	if (listen(server->fd, 0) < 0) {
		ERROR("server_start: listen failed");
		return false;
	}

	ev_io_init(&server->io_accept, accept_cb, server->fd, EV_READ);
	ev_io_start(server->io_loop, &server->io_accept);
	return true;
}

/**
 * accept_cb
 *
 * The ev client accept callback
 *  - loop: The event loop
 *  - accept_io: The io handler
 *  - revents: Event flags
 */
void accept_cb(struct ev_loop* loop, struct ev_io* accept_io, int revents)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	server_t* server = container_of(accept_io, server_t, io_accept);

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

	INFO("accept_cb: accepted new client from %s:%d", inet_ntoa(client->addr), server->port);

	ev_io_init(&client->io_read, client_io_avail, client->fd, EV_READ|EV_WRITE);
	ev_io_start(loop, &client->io_read);
}

/**
 * server_client_drop
 *
 * Cleanly disconnects a client
 *  - server: The server
 *  - client: The client
 */
void server_client_drop(server_t* server, client_t* client)
{
	client->client_drop = true;
}

/**
 * client_io_avail
 *
 * The ev client io callback
 *  - loop: The event loop
 *  - accept_io: The io handler
 *  - revents: Event flags
 */
void client_io_avail(struct ev_loop* loop, struct ev_io* io_read, int revents)
{
	client_t* client = container_of(io_read, client_t, io_read);
	// Check for errors
	if (EV_ERROR & revents) {
		ERROR("client_io_avail: invalid event");
		return;
	}

	server_t* server = client->server;
	char buffer[server->buf_size];	// todo: work out a better place to put this buffer
	int report_read = 0;

	if (client->client_drop) {
		ev_io_stop(server->io_loop, &client->io_read);
		close(client->fd);
		server_client_cleanup(server, client);
		return;
	}

	if (revents & EV_READ) {
		int read_avail = recv(client->fd, buffer, server->buf_size, 0);

		// Check for read error
		if (read_avail < 0) {
			WARN("client_io_avail: %s", strerror(errno));
			switch (errno) {
			case ECONNRESET:
			case ETIMEDOUT:
				read_avail = 0; // drop the client later on
				break;
			default:
				ERROR("client_io_avail: unable to handle error: %s", strerror(errno));
				break;
			}
			return;
		}

		// Check for client drop
		if (read_avail == 0) {
			server_client_drop(server, client);
			INFO("client_io_avail: client dropped");
			return;
		}

		int buffered = buffer_write(&client->read_buffer, buffer, read_avail);
		if (buffered != read_avail) {
			WARN("client_io_avail: buffer overflow. dropping %i bytes", read_avail-buffered);
		}


		if (client->handshake_stage == HANDSHAKE_ACCEPTED) {
			report_read = 1;
		}
	}

	if (revents & EV_WRITE) { // flush write buffer if necessary
		if (client->handshake_stage == HANDSHAKE_ACCEPTED) {
			server->write_cb(client, server);
		}

		size_t write_avail = buffer_read(&client->write_buffer, buffer, server->buf_size);
		int write_caret = 0;
		while (write_avail > write_caret) {
			size_t sent = send(client->fd, buffer+write_caret, write_avail, 0);

			// Check for write error
			if (sent < 0) {
				WARN("client_io_avail: %s", strerror(errno));
				switch (errno) {
				case ECONNRESET:
				case ETIMEDOUT:
					sent = 0; // drop the client later on
					break;
				default:
					ERROR("client_io_avail: unable to handle error: %s", strerror(errno));
					break;
				}
				return;
			}

			// Check for client drop
			if (sent == 0) {
				server_client_drop(server, client);
				INFO("client_io_avail: client dropped");
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
				report_read = 1;
			}
			break;
		}
	}

	if (client->handshake_stage == HANDSHAKE_ACCEPTED &&
		buffer_read_avail(&client->read_buffer) > 0 &&
		server->flags & SF_PARTIAL_READ) {
		// re-report previously buffered but un-processed data
		report_read = 1;
	}

	if (report_read) {
		server->read_cb(client, server);
	}
}

/**
 * server_client_init
 *
 * Initializes a client
 *  - server: The server
 *  - fd: The file descriptor of the client
 *  - addr: The address of the client
 * returns: An initialized client
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
	buffer_create(&client->read_buffer, server->buf_size);
	buffer_create(&client->write_buffer, server->buf_size);
	return client;
}

/**
 * server_client_cleanup
 *
 * Cleans up a client for exit
 *  - server: The server
 *  - client: The client
 */
void server_client_cleanup(server_t* server, client_t* client)
{
	buffer_free(&client->read_buffer);
	buffer_free(&client->write_buffer);
	server->drop_cb(client, server);
}

/**
 * server_stop
 *
 * Stops the server io loop
 *  - server: The server
 */
void server_stop(server_t* server)
{
	ev_io_stop(server->io_loop, &server->io_accept);
	close(server->fd);
}
