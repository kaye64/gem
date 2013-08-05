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

#endif /* _GAME_CLIENT_H_ */
