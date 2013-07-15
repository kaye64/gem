#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_

#include <stdbool.h>

#include <util/list.h>
#include <util/queue.h>
#include <util/object.h>
#include <net/server.h>
#include <game/service.h>
#include <runite/cache.h>
#include <crypto/isaac.h>
#include <crypto/rsa.h>
#include <net/codec.h>
#include <game/packet/packet.h>
#include <game/player.h>
#include <game/world.h>
#include <game/entity_list.h>

typedef struct game_service game_service_t;

struct game_service {
	object_t object;
	service_t service;
	isaac_t rand_gen;
	rsa_t* rsa;
	entity_list_t player_list;
	world_t world;
};

extern object_proto_t game_service_proto;

void game_config(game_service_t* game, rsa_t* rsa, cache_t* cache);
void game_process_io(game_service_t* game);
void player_sync(game_service_t* game_service);

#endif /* _GAME_SERVICE_H_ */
