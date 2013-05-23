#include <net/server.h>

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <core/log.h>

#define LOG_TAG "server"

void accept_cb(struct ev_loop* loop, server_io_t* watcher, int revents);

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
	server->socket_fd = -1;
	return server;
}

int server_start(server_t* server)
{
	server->io_loop = ev_loop_new(EVBACKEND_EPOLL | EVFLAG_NOENV);
	server->socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (server->socket_fd < 0) {
		ERROR("server_start: socket open failed");
		return 0;
	}

	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(server->port);
	address.sin_addr.s_addr = inet_addr(server->addr);

	if (bind(server->socket_fd, (struct sockaddr*)&address, sizeof(address)) != 0) {
		ERROR("server_start: bind failed");
		return 0;
	}

	if (listen(server->socket_fd, 0) < 0) {
		ERROR("server_start: listen failed");
		return 0;
	}

	ev_io_init((struct ev_io*)&server->io_accept, accept_cb, server->socket_fd, EV_READ);
	server->io_accept.server = server;
	ev_io_start(server->io_loop, (struct ev_io*)&server->io_accept);
	return 1;
}

void accept_cb(struct ev_loop* loop, server_io_t* watcher, int revents)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	if (EV_ERROR & revents) {
		ERROR("accept_cb: invalid event");
		return;
	}

	int client_fd = accept(watcher->server->socket_fd, (struct sockaddr*)&client_addr, &client_len);
	if (client_fd < 0) {
		ERROR("accept_cb: accept failed");
		return;
	}
	INFO("accept_cb: accepted new client from %s", inet_ntoa(client_addr.sin_addr));
}

void server_stop(server_t* server)
{

}

void server_poll(server_t* server, int block)
{
	ev_loop(server->io_loop, (block ? 0 : EVRUN_NOWAIT));
}
