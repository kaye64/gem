#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_

#include <net/server.h>
#include <world/service.h>

struct game_service {
	service_t service;
	int must_free;
};
typedef struct game_service game_service_t;

struct game_client {

};
typedef struct game_client game_client_t;

game_service_t* game_create(game_service_t* game);
void game_free(game_service_t* game);

#endif /* _GAME_SERVICE_H_ */
