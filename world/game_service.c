/**
 * game_service.c
 *
 * The game server
 */
#include <world/game_service.h>

#include <stdlib.h>

#include <util/log.h>
#include <util/container_of.h>
#include <world/dispatcher.h>

#define LOG_TAG "update"

void* game_service_accept(service_client_t* service_client);
int game_service_handshake(service_client_t* service_client);
void game_service_read(service_client_t* service_client);
void game_service_write(service_client_t* service_client);
void game_service_drop(service_client_t* service_client);

int game_process_login(game_service_t* game_service, client_t* client, game_client_t* game_client);

/**
 * game_create
 *
 * Allocates and creates an game_service_t
 *  - update: Some preallocated memory, or null to put on heap
 *  - cache: The cache to serve
 * returns: The game_service_t
 */
game_service_t* game_create(game_service_t* game, rsa_t* rsa, cache_t* cache)
{
	if (game == NULL) {
		game = (game_service_t*)malloc(sizeof(game_service_t));
		game->must_free = true;
	} else {
		game->must_free = false;
	}
	game->rsa = rsa;
	game->service.accept_cb = (service_accept_t)&game_service_accept;
	game->service.handshake_cb = (service_handshake_t)&game_service_handshake;
	game->service.read_cb = (service_read_t)&game_service_read;
	game->service.write_cb = (service_write_t)&game_service_write;
	game->service.drop_cb = (service_drop_t)&game_service_drop;
	return game;
}

/**
 * game_free
 *
 * Properly frees the update service
 *  - game: The service to free
 */
void game_free(game_service_t* game)
{
	if (game->must_free) {
		free(game);
	}
}

/**
 * game_service_accept
 *
 * Validates a new service_client_t
 *  - service_client: The service client
 * returns: An item to use as the service_client's attribute
 */
void* game_service_accept(service_client_t* service_client)
{
	game_client_t* client = (game_client_t*)malloc(sizeof(game_client_t));
	codec_create(&client->codec);
	client->login_stage = STAGE_INIT;
	return client;
}

/**
 * game_service_handshake
 *
 * Performs any handshake routines between the client/server
 *  - service_client: The service client
 * returns: One of HANDSHAKE_{DENIED,PENDING,ACCEPTED}
 */
int game_service_handshake(service_client_t* service_client)
{
	game_service_t* game_service = container_of(service_client->service, game_service_t, service);
	game_client_t* game_client = (game_client_t*)service_client->attrib;
	client_t* client = &service_client->client;
	int status = game_process_login(game_service, client, game_client);
	if (status == LOGIN_PENDING) {
		return HANDSHAKE_PENDING;
	}

	codec_seek(&game_client->codec, 0);
	codec_put8(&game_client->codec, status, 0);

	int handshake_status = HANDSHAKE_ACCEPTED;

	if (status == LOGIN_OKAY) {
		codec_put8(&game_client->codec, PLAYER_RIGHTS_SUPER, 0);
		codec_put8(&game_client->codec, 0, 0); // I believe this can toggle advanced player stats for bot detection
	} else {
		handshake_status = HANDSHAKE_DENIED;
	}
	codec_buffer_write(&game_client->codec, &client->write_buffer);
	return handshake_status;
}

/**
 * game_service_read
 *
 * Called when data is available to be read by the client
 *  - service_client: The service client
 */
void game_service_read(service_client_t* service_client)
{

}

/**
 * game_service_write
 *
 * Called to signal that we can write to the client
 *  - service_client: The service client
 */
void game_service_write(service_client_t* service_client)
{

}

/**
 * game_service_drop
 *
 * Called to perform any client cleanup
 *  - service_client: The service client
 */
void game_service_drop(service_client_t* service_client)
{
	game_client_t* game_client = (game_client_t*)service_client->attrib;
	free(game_client);
}

int game_process_login(game_service_t* game_service, client_t* client, game_client_t* game_client)
{
	if (game_client->login_stage == STAGE_INIT) {
		codec_seek(&game_client->codec, 0);
		if (!codec_buffer_read(&game_client->codec, &client->read_buffer, 1)) {
			return LOGIN_PENDING;
		}

		// This is one byte of the hashed form of the user's login name.
		// Probably used to select a login server, but we don't need it.
		/*uint8_t name_hash = */codec_get8(&game_client->codec, NULL, 0);

		codec_seek(&game_client->codec, 0);
		// Write the 8 ignored bytes
		for (int i = 0; i < 8; i++) {
			codec_put8(&game_client->codec, 0, 0);
		}

		// Request the login packet
		codec_put8(&game_client->codec, 0, 0);

		// Send the server's random seed
		game_client->server_isaac_seed = isaac_next(&game_service->rand_gen);
		game_client->server_isaac_seed <<= 32;
		game_client->server_isaac_seed += isaac_next(&game_service->rand_gen);
		codec_put64(&game_client->codec, game_client->server_isaac_seed, 0);

		if (!codec_buffer_write(&game_client->codec, &client->write_buffer)) {
			return LOGIN_PENDING;
		}
		game_client->login_stage = STAGE_HANDSHAKE;
		return LOGIN_PENDING;
	}

	if (game_client->login_stage == STAGE_HANDSHAKE) {
		codec_seek(&game_client->codec, 0);
		// Read the login packet header
		if (!codec_buffer_read(&game_client->codec, &client->read_buffer, 2)) {
			return LOGIN_PENDING;
		}
		int login_type = codec_get8(&game_client->codec, NULL, 0);
		int login_len = codec_get8(&game_client->codec, NULL, 0);

		if (!codec_buffer_read(&game_client->codec, &client->read_buffer, login_len)) {
			return LOGIN_PENDING;
		}

		if (codec_get8(&game_client->codec, NULL, 0) != 255) {
			WARN("Client sent invalid login magic");
			return LOGIN_REJECTED;
		}
		uint16_t client_ver = codec_get16(&game_client->codec, NULL, 0);
		uint8_t client_mem_type = codec_get8(&game_client->codec, NULL, 0);

		if (client_ver != CLIENT_REVISION) {
			WARN("Client connected with unexpected revision %d", client_ver);
			return LOGIN_UPDATED;
		}

		if (client_mem_type < 0 || client_mem_type > 1) {
			WARN("Client connected with invalid memory type %d", client_mem_type);
			return LOGIN_REJECTED;
		}

		game_client->high_memory = (client_mem_type == 1);

		uint32_t client_archive_crcs[9];
		for (int i = 0; i < 9; i++) {
			// todo: validate these values
			client_archive_crcs[i] = codec_get32(&game_client->codec, NULL, 0);
		}
		game_client->login_stage = STAGE_SECURE_BLOCK;
		return LOGIN_PENDING;
	}

	if (game_client->login_stage == STAGE_SECURE_BLOCK) {
		// Decrypt the secure login block
		codec_block_decrypt(&game_client->codec, game_service->rsa);
		if (codec_get8(&game_client->codec, NULL, 0) != 10) {
			WARN("Corrupt secure login block");
			return LOGIN_REJECTED;
		}

		// Verify isaac seed, create the RNGs
		game_client->client_isaac_seed = codec_get64(&game_client->codec, NULL, 0);
		if (codec_get64(&game_client->codec, NULL, 0) != game_client->server_isaac_seed) {
			WARN("Client modified server isaac seed");
			return LOGIN_REJECTED;
		}
		uint32_t isaac_seed[4];
		isaac_seed[0] = game_client->client_isaac_seed >> 32;
		isaac_seed[1] = game_client->client_isaac_seed;
		isaac_seed[2] = game_client->server_isaac_seed >> 32;
		isaac_seed[3] = game_client->server_isaac_seed;
		isaac_create(&game_client->isaac_in, isaac_seed, 4);
		for (int i = 0; i < 4; i++) {
			isaac_seed[i] += 50;
		}
		isaac_create(&game_client->isaac_out, isaac_seed, 4);

		game_client->client_uid = codec_get32(&game_client->codec, NULL, 0);
		codec_gets(&game_client->codec, game_client->username, 32, CODEC_JSTRING);
		codec_gets(&game_client->codec, game_client->password, 32, CODEC_JSTRING);
		INFO("New client: %s, uid: %d", game_client->username, game_client->client_uid);
		/* Until we have a profile loader, just accept the client */
		game_client->login_stage = STAGE_COMPLETE;
		return LOGIN_OKAY;
	}
	return LOGIN_PENDING;
}
