#ifndef _GAME_CLIENT_H_
#define _GAME_CLIENT_H_

#include <util/list.h>
#include <util/queue.h>
#include <net/stream_codec.h>
#include <world/mob.h>
#include <crypto/isaac.h>
#include <world/packet/packet.h>

#define PLAYER_RIGHTS_NORMAL 0
#define PLAYER_RIGHTS_MODERATOR 1
#define PLAYER_RIGHTS_ADMIN 2
#define PLAYER_RIGHTS_SUPER 3

struct game_client {
	list_node_t node;
	/* client config */
	uint32_t client_uid;
	char username[32];
	char password[32]; // We should just validate on login and remove this from memory
	bool high_memory;
	int rights;
	int login_stage;
	mob_t mob;
	/* cryption */
	uint64_t server_isaac_seed;
	uint64_t client_isaac_seed;
	isaac_t isaac_in;
	isaac_t isaac_out;
	/* net */
	queue_t packet_queue_in;
	queue_t packet_queue_out;
	stream_codec_t codec;
};
typedef struct game_client game_client_t;

void game_client_enqueue_packet(game_client_t* game_client, packet_t* packet);

#endif /* _GAME_CLIENT_H_ */
