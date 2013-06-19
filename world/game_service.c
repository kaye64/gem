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
#include <world/game_login.h>

#define LOG_TAG "game"

void* game_service_accept(service_client_t* service_client);
int game_service_handshake(service_client_t* service_client);
void game_service_read(service_client_t* service_client);
void game_service_write(service_client_t* service_client);
void game_service_drop(service_client_t* service_client);

void game_player_login(game_service_t* game_service, game_client_t* game_client);
void game_player_logout(game_service_t* game_service, game_client_t* game_client);

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
	list_create(&game->player_list);
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
	list_free(&game->player_list);
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
	codec_put8(&game_client->codec, status);

	int handshake_status = HANDSHAKE_ACCEPTED;

	if (status == LOGIN_OKAY) {
		codec_put8(&game_client->codec, game_client->rights);
		codec_put8(&game_client->codec, 0); // I believe this can toggle advanced player stats for bot detection
		game_player_login(game_service, game_client);
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
	game_service_t* game_service = container_of(service_client->service, game_service_t, service);
	game_client_t* game_client = (game_client_t*)service_client->attrib;
	if (game_client->login_stage == STAGE_COMPLETE) {
		game_player_logout(game_service, game_client);
	}
	free(game_client);
}

/**
 * game_player_login
 *
 * Called when a new player successfully logs in
 *  - game_service: The game service
 *  - game_client: The game client
 */
void game_player_login(game_service_t* game_service, game_client_t* game_client)
{
	list_push_back(&game_service->player_list, &game_client->node);
	INFO("Player login: %s", game_client->username);
}

/**
 * game_player_logout
 *
 * Called when a new player logs out
 *  - game_service: The game service
 *  - game_client: The game client
 */
void game_player_logout(game_service_t* game_service, game_client_t* game_client)
{
	list_erase(&game_service->player_list, &game_client->node);
	INFO("Player logout: %s", game_client->username);
}
