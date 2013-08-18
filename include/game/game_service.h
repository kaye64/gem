/**
 *  This file is part of Gem.
 *
 *  Gem is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Gem is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Gem.  If not, see <http://www.gnu.org/licenses/\>.
 */

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
