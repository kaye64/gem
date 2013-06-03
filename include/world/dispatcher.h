#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

#include <net/server.h>
#include <world/service.h>

struct dispatcher {
	server_t server;
	service_t* game_service;
	service_t* update_service;
};
typedef struct dispatcher dispatcher_t;

dispatcher_t* dispatcher_create(const char* addr, service_t* game, service_t* update);
void dispatcher_free(dispatcher_t* dispatcher);
void dispatcher_start(dispatcher_t* dispatcher, struct ev_loop* loop);

#endif /* _DISPATCHER_H_ */
