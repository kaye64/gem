#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_

#include <stdbool.h>

#include <net/server.h>
#include <world/service.h>
#include <runite/cache.h>
#include <crypto/isaac.h>
#include <crypto/rsa.h>
#include <net/stream_codec.h>
#include <util/list.h>

#define PLAYER_RIGHTS_NORMAL 0
#define PLAYER_RIGHTS_MODERATOR 1
#define PLAYER_RIGHTS_ADMIN 2
#define PLAYER_RIGHTS_SUPER 3

struct game_service {
	service_t service;
	isaac_t rand_gen;
	rsa_t* rsa;
	list_t player_list;
	bool must_free;
};
typedef struct game_service game_service_t;

struct game_client {
	list_node_t node;
	/* client config */
	uint32_t client_uid;
	char username[32];
	char password[32]; // We should just validate on login and remove this from memory
	bool high_memory;
	int rights;
	int login_stage;
	/* cryption */
	uint64_t server_isaac_seed;
	uint64_t client_isaac_seed;
	isaac_t isaac_in;
	isaac_t isaac_out;
	/* net */
	stream_codec_t codec;
};
typedef struct game_client game_client_t;

game_service_t* game_create(game_service_t* game, rsa_t* rsa, cache_t* cache);
void game_free(game_service_t* game);

#endif /* _GAME_SERVICE_H_ */
