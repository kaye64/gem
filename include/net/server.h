#ifndef _SERVER_H_
#define _SERVER_H_

#include <ev.h>
#include <netinet/in.h>

#include <net/buffer.h>

#define SERVER_DEFAULT_BUFFER_SIZE 4096 // todo: make cvar

// forward declarations
struct server;
struct client;

// client_t* client_accept(int fd, struct in_addr addr, struct server* server)
typedef struct client*(*client_accept_t)(int, struct in_addr, struct server*);
// int client_handshake(client_t* client)
typedef int(*client_handshake_t)(struct client*);
// void client_drop(client_t* client);
typedef void(*client_drop_t)(struct client*);

struct accept_io {
	ev_io read_io;
	struct server* server;
};
typedef struct accept_io accept_io_t;

struct server {
	/* net stuff */
	char addr[16];
	int port;
	int fd;
	int buf_size;
	/* libev stuff */
	struct ev_loop* io_loop;
	accept_io_t io_accept;
	/* callbacks */
	client_accept_t accept_cb;
	client_handshake_t handshake_cb;
	client_drop_t drop_cb;
	/* misc stuff */
	int must_free;
};
typedef struct server server_t;

struct client {
	/* net stuff */
	ev_io io_read;
	int fd;
	struct in_addr addr;
	buffer_t read_buffer;
	buffer_t write_buffer;
	/* misc stuff */
	server_t* server;
};
typedef struct client client_t;

server_t* server_create(server_t* server, const char* addr, int port);
void server_free(server_t* server);
int server_start(server_t* server);
void server_stop(server_t* server);
client_t* server_client_init(server_t* server, int fd, struct in_addr addr);
void server_client_drop(server_t* server, client_t* client);
void server_client_cleanup(server_t* server, client_t* client);
void server_poll(server_t* server, int block);

#endif /* _SERVER_H_ */
