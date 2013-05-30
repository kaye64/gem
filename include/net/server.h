#ifndef _SERVER_H_
#define _SERVER_H_

#include <ev.h>
#include <netinet/in.h>

#include <net/buffer.h>

#define SERVER_DEFAULT_BUFFER_SIZE 4096 // todo: make cvar

#define HANDSHAKE_PENDING 0
#define HANDSHAKE_DENIED 1
#define HANDSHAKE_ACCEPTED 2

// forward declarations
struct server;
struct client;

// client_t* client_accept(int fd, struct in_addr addr, struct server* server);
typedef struct client*(*client_accept_t)(int, struct in_addr, struct server*);
// int client_handshake(client_t* client);
typedef int(*client_handshake_t)(struct client*);
// void client_read(client_t* client);
typedef void(*client_read_t)(struct client*);
// void client_write(client_t* client);
typedef void(*client_write_t)(struct client*);
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
	client_read_t read_cb;
	client_write_t write_cb;
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
	int handshake_stage;
};
typedef struct client client_t;

server_t* server_create(server_t* server, const char* addr, int port);
void server_free(server_t* server);
int server_start(server_t* server, struct ev_loop* loop);
void server_stop(server_t* server);
client_t* server_client_init(server_t* server, int fd, struct in_addr addr);
void server_client_drop(server_t* server, client_t* client);
void server_client_cleanup(server_t* server, client_t* client);

#endif /* _SERVER_H_ */
