#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

#include <util/object.h>
#include <net/server.h>
#include <game/service.h>

typedef struct dispatcher dispatcher_t;

struct dispatcher {
	object_t object;
	server_t server;
	service_t* game_service;
	service_t* update_service;
};

extern object_proto_t dispatcher_proto;

void dispatcher_config(dispatcher_t* dispatcher, const char* addr, service_t* game, service_t* update);
void dispatcher_start(dispatcher_t* dispatcher, struct ev_loop* loop);

#endif /* _DISPATCHER_H_ */
