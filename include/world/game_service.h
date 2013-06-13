#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_

#include <stdbool.h>

#include <net/server.h>
#include <world/service.h>
#include <runite/cache.h>

struct game_service {
	service_t service;
	bool must_free;
};
typedef struct game_service game_service_t;

struct game_client {

};
typedef struct game_client game_client_t;

game_service_t* game_create(game_service_t* game, cache_t* cache);
void game_free(game_service_t* game);

#endif /* _GAME_SERVICE_H_ */
