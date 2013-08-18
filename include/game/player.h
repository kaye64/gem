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

#ifndef _GAME_CLIENT_H_
#define _GAME_CLIENT_H_

#include <util/object.h>
#include <util/list.h>
#include <util/queue.h>
#include <game/entity_tracker.h>
#include <net/codec.h>
#include <game/mob.h>
#include <game/packet/packet.h>
#include <crypto/isaac.h>

#define PLAYER_RIGHTS_NORMAL 0
#define PLAYER_RIGHTS_MODERATOR 1
#define PLAYER_RIGHTS_ADMIN 2
#define PLAYER_RIGHTS_SUPER 3

#define PLAYER_FLAG_TAB_UPDATE (1 << 0) /* signals that a tab interface needs to be updated */
#define PLAYER_FLAG_LOGOUT (1 << 1) /* signals that the player should be logged out */

typedef struct world world_t;
typedef struct game_service game_service_t;
typedef struct player player_t;

struct player {
	object_t object;
	list_node_t node;
	/* client state */
	uint32_t client_uid;
	uint32_t index;
	char username[32];
	char password[32]; // We should just validate on login and remove this from memory
	bool high_memory;
	int rights;
	int login_stage;
	mob_t mob;
	entity_tracker_t known_players;
	int tab_interfaces[14];
	uint8_t update_flags;
	/* cryption */
	uint64_t server_isaac_seed;
	uint64_t client_isaac_seed;
	isaac_t isaac_in;
	isaac_t isaac_out;
	/* net */
	queue_t packet_queue_in;
	queue_t packet_queue_out;
	codec_t codec;
};

extern object_proto_t player_proto;

player_t* player_for_entity(entity_t* entity);
player_t* player_for_mob(mob_t* mob);

mob_t* mob_for_player(player_t* player);
entity_t* entity_for_player(player_t* player);

void player_tick_before(world_t* world, player_t* player);
void player_tick_after(world_t* world, player_t* player);
void player_enqueue_packet(player_t* player, packet_t* packet);

void player_login(game_service_t* game_service, player_t* player);
void player_logout(game_service_t* game_service, player_t* player);

void player_set_tab_interface(player_t* player, int tab_id, int interface_id);
void player_force_logout(player_t* player);

#endif /* _GAME_CLIENT_H_ */
