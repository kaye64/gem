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

#ifndef _SERVER_H_
#define _SERVER_H_

#include <ev.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <runite/util/object.h>
#include <runite/util/list.h>

#include <util/config.h>
#include <net/buffer.h>

#define HANDSHAKE_PENDING 0
#define HANDSHAKE_DENIED 1
#define HANDSHAKE_ACCEPTED 2

// SF_PARTIAL_READ indicates that the service may leave full packets
// in the read buffer after a cycle. This is so that the event loop
// can re-report data which has already been signalled to the service
// in a previous iteration.
#define SF_PARTIAL_READ (1 << 0)

typedef struct server server_t;
typedef struct client client_t;

// client_t* client_accept(int fd, struct in_addr addr, server_t* server);
typedef struct client*(*client_accept_t)(int, struct in_addr, server_t*);
// int client_handshake(client_t* client, server_t* server);
typedef int(*client_handshake_t)(client_t*, server_t*);
// void client_read(client_t* client, server_t* server);
typedef void(*client_read_t)(client_t*, server_t*);
// void client_write(client_t* client, server_t* server);
typedef void(*client_write_t)(client_t*, server_t*);
// void client_drop(client_t* client, server_t* server);
typedef void(*client_drop_t)(client_t*, server_t*);

struct server {
	object_t object;
	/* net stuff */
	char addr[16];
	int port;
	int fd;
	int buf_size;
	unsigned char* buffer;
	/* libev stuff */
	struct ev_loop* io_loop;
	struct ev_io io_accept;
	/* callbacks */
	client_accept_t accept_cb;
	client_handshake_t handshake_cb;
	client_read_t read_cb;
	client_write_t write_cb;
	client_drop_t drop_cb;
	/* misc stuff */
	list_t client_list;
	uint8_t flags;
};

extern object_proto_t server_proto;

struct client {
	server_t* server;
	int handshake_stage;
	bool client_drop;
	list_node_t node;
	/* net stuff */
	struct ev_io io_read;
	int fd;
	struct in_addr addr;
	buffer_t read_buffer;
	buffer_t write_buffer;
};

void server_config(server_t* server, const char* addr, int port, uint8_t flags);
bool server_start(server_t* server, struct ev_loop* loop);
void server_stop(server_t* server);
client_t* server_client_init(server_t* server, int fd, struct in_addr addr);
void server_client_drop(server_t* server, client_t* client);
void server_client_cleanup(server_t* server, client_t* client);

#endif /* _SERVER_H_ */
