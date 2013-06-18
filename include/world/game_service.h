#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_

#include <stdbool.h>

#include <net/server.h>
#include <world/service.h>
#include <runite/cache.h>
#include <crypto/isaac.h>
#include <crypto/rsa.h>
#include <net/stream_codec.h>

#define STAGE_INIT 0
#define STAGE_HANDSHAKE 1
#define STAGE_SECURE_BLOCK 2
#define STAGE_COMPLETE 3

#define LOGIN_PENDING 0
#define LOGIN_DELAY 1
#define LOGIN_OKAY 2
#define LOGIN_INVALID_CREDENTIALS 3
#define LOGIN_DISABLED 4
#define LOGIN_DUPLICATE_SESSION 5
#define LOGIN_UPDATED 6
#define LOGIN_SERVER_FULL 7
#define LOGIN_NO_LOGIN_SERVER 8
#define LOGIN_TOO_MANY_CONNECTIONS 9
#define LOGIN_BAD_SESSION_ID 10
#define LOGIN_REJECTED 11
#define LOGIN_MEMBERS_WORLD 12
#define LOGIN_INCOMPLETE 13
#define LOGIN_UPDATING 14
#define LOGIN_UNKNOWN 15 // What does this do?
#define LOGIN_ATTEMPTS_EXCEEDED 16
#define LOGIN_MEMBERS_AREA 17
#define LOGIN_INVALID_LOGIN_SERVER 20
#define LOGIN_INVALID_TRANSFERRING 21

#define PLAYER_RIGHTS_NORMAL 0
#define PLAYER_RIGHTS_MODERATOR 1
#define PLAYER_RIGHTS_ADMIN 2
#define PLAYER_RIGHTS_SUPER 3

struct game_service {
	service_t service;
	isaac_t rand_gen;
	rsa_t* rsa;
	bool must_free;
};
typedef struct game_service game_service_t;

struct game_client {
	uint32_t client_uid;
	char username[32];
	char password[32]; // We should just validate on login and remove this from memory
	bool high_memory;
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
