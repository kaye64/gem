#ifndef _SERVER_H_
#define _SERVER_H_

#include <ev.h>

struct server;

struct server_io {
	ev_io io;
	struct server* server;
};
typedef struct server_io server_io_t;

struct server {
	/* net stuff */
	char addr[16];
	int port;
	int socket_fd;
	/* libev stuff */
	struct ev_loop* io_loop;
	server_io_t io_accept;
	/* misc stuff */
	int must_free;
};
typedef struct server server_t;

// void* client_accept()
typedef void*(*client_accept_t)();
// int client_handshake(void* attrib)
typedef int(*client_handshake_t)(void*);
// void client_quit(void* attrib);
typedef void(*client_quit_t)(void*);

server_t* server_create(server_t* server, const char* addr, int port);
int server_start(server_t* server);
void server_stop(server_t* server);
void server_poll(server_t* server, int block);

#endif /* _SERVER_H_ */
