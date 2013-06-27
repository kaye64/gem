#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_

#include <stdbool.h>

#include <net/server.h>
#include <game/service.h>
#include <runite/cache.h>
#include <crypto/isaac.h>
#include <crypto/rsa.h>
#include <net/stream_codec.h>
#include <util/list.h>
#include <util/queue.h>
#include <game/packet/packet.h>
#include <game/player.h>
#include <game/world.h>

struct game_service {
	service_t service;
	isaac_t rand_gen;
	rsa_t* rsa;
	list_t player_list;
	world_t world;
	bool must_free;
};
typedef struct game_service game_service_t;

game_service_t* game_create(game_service_t* game, rsa_t* rsa, cache_t* cache);
void game_free(game_service_t* game);

void game_process_io(game_service_t* game);
void player_sync(game_service_t* game_service);

#endif /* _GAME_SERVICE_H_ */
