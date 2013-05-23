#include <net/server.h>

void accept_cb(struct ev_loop* loop, server_io_t* watcher, int revents);

server_t* server_create(const char* addr, int port)
{
	server_t* server = (server_t*)malloc(sizeof(server_t));
	strcpy(server->addr, addr);
	server->port = port;
	return server;
}

int server_start(server_t* server)
{
	server->io_loop = ev_loop_new(EVBACKEND_EPOLL | EVFLAG_NOENV);
	server->socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (server->socket_fd < 0) {
		return 1;
	}

	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(server->port);
	address.sin_addr.s_addr = inet_addr(server->addr);

	if (bind(server->socket_fd, (struct sockaddr*)&address, sizeof(address)) != 0) {
		return 1;
	}

	if (listen(server->socket_fd, 0) < 0) {
		return 1;
	}

	ev_io_init(&server->io_accept, accept_cb, server->socket_fd, EV_READ);
	ev_io_start(server->io_loop, &server->io_accept);
}

void accept_cb(struct ev_loop* loop, server_io_t* watcher, int revents)
{

}

void server_stop(server_t* server)
{

}

void server_poll(server_t* server)
{

}
