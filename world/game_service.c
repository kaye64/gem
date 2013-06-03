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

game_service_t* game_create(game_service_t* game)
{
	if (game == NULL) {
		game = (game_service_t*)malloc(sizeof(game_service_t));
		game->must_free = 1;
	} else {
		game->must_free = 0;
	}
	game->service.accept_cb = (service_accept_t)&game_service_accept;
	game->service.handshake_cb = (service_handshake_t)&game_service_handshake;
	game->service.read_cb = (service_read_t)&game_service_read;
	game->service.write_cb = (service_write_t)&game_service_write;
	game->service.drop_cb = (service_drop_t)&game_service_drop;
	return game;
}

void game_free(game_service_t* game)
{
	if (game->must_free == 1) {
		free(game);
	}
}

void* game_service_accept(service_client_t* service_client)
{
	game_client_t* client = (game_client_t*)malloc(sizeof(game_client_t));
	return client;
}

int game_service_handshake(service_client_t* service_client)
{
	return HANDSHAKE_ACCEPTED;
}

void game_service_read(service_client_t* service_client)
{

}

void game_service_write(service_client_t* service_client)
{

}

void game_service_drop(service_client_t* service_client)
{
	game_client_t* game_client = (game_client_t*)service_client->attrib;
	free(game_client);
}
