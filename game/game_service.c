/**
 * game_service.c
 *
 * The game server
 */
#include <game/game_service.h>

#include <stdlib.h>

#include <util/log.h>
#include <util/container_of.h>
#include <game/dispatcher.h>
#include <game/game_login.h>
#include <game/packet/packet_router.h>
#include <game/packet/builders.h>

#define LOG_TAG "game"

void* game_service_accept(service_client_t* service_client);
int game_service_handshake(service_client_t* service_client);
void game_service_read(service_client_t* service_client);
void game_service_write(service_client_t* service_client);
void game_service_drop(service_client_t* service_client);

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
	uint32_t seed = 0; // Not particularly important
	isaac_create(&game->rand_gen, &seed, 1);
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
	isaac_free(&game->rand_gen);
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
	player_t* player = player_create(NULL);
	player->login_stage = STAGE_INIT;
	return player;
}

/**
 * game_service_drop
 *
 * Called to perform any client cleanup
 *  - service_client: The service client
 */
void game_service_drop(service_client_t* service_client)
{
	player_t* player = (player_t*)service_client->attrib;
	if (player->login_stage == STAGE_COMPLETE) {
		player->login_stage = STAGE_EXITING;
	} else {
		player->login_stage = STAGE_CLEANUP;
	}
	service_client->attrib = NULL;
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
	player_t* player = (player_t*)service_client->attrib;
	client_t* client = &service_client->client;
	int status = game_process_login(game_service, client, player);
	if (status == LOGIN_PENDING) {
		return HANDSHAKE_PENDING;
	}

	codec_seek(&player->codec, 0);
	codec_put8(&player->codec, status);

	int handshake_status = HANDSHAKE_ACCEPTED;

	if (status == LOGIN_OKAY) {
		codec_put8(&player->codec, player->rights);
		codec_put8(&player->codec, 0); // I believe this can toggle advanced player stats for bot detection
		player_login(game_service, player);
	} else {
		handshake_status = HANDSHAKE_DENIED;
	}
	codec_buffer_write(&player->codec, &client->write_buffer);
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
	player_t* player = (player_t*)service_client->attrib;
	client_t* client = &service_client->client;
	buffer_pushp(&client->read_buffer);
	codec_seek(&player->codec, 0);
	if (!codec_buffer_read(&player->codec, &client->read_buffer, 1)) {
		buffer_popp(&client->read_buffer);
		return;
	}

	/* read the opcode */
	uint8_t opcode = codec_get8(&player->codec) - isaac_next(&player->isaac_in);
	int payload_len;
	packet_def_t definition = packet_lookup(PACKET_TYPE_IN, opcode);
	if (definition.opcode == PKT_NULL) {
		WARN("unknown opcode: %d", opcode);
		return;
	}

	/* find the payload length */
	packet_t* packet = packet_create(definition);
	switch (definition.type) {
	case PACKET_LEN_FIXED:
		payload_len = definition.len;
		break;
	case PACKET_LEN_8:
		if (!codec_buffer_read(&player->codec, &client->read_buffer, 1)) {
			buffer_popp(&client->read_buffer);
			packet_free(packet);
			return;
		}
		payload_len = codec_get8(&player->codec);
		break;
	case PACKET_LEN_16:
		if (!codec_buffer_read(&player->codec, &client->read_buffer, 2)) {
			buffer_popp(&client->read_buffer);
			packet_free(packet);
			return;
		}
		payload_len = codec_get16(&player->codec);
		break;
	}
	packet->len = payload_len;

	/* read the payload */
	if (codec_buffer_read(&packet->payload, &client->read_buffer, payload_len)) {
		codec_seek(&packet->payload, 0);
		queue_push(&player->packet_queue_in, &packet->node);
	} else {
		buffer_popp(&client->read_buffer);
		packet_free(packet);
	}
}

/**
 * game_service_write
 *
 * Called to signal that we can write to the client
 *  - service_client: The service client
 */
void game_service_write(service_client_t* service_client)
{
	player_t* player = (player_t*)service_client->attrib;
	client_t* client = &service_client->client;
	while (!queue_empty(&player->packet_queue_out)) {
		list_node_t* node = queue_pop(&player->packet_queue_out);
		packet_t* packet = container_of(node, packet_t, node);
		int payload_len = codec_len(&packet->payload);
		codec_seek(&player->codec, 0);
		int opcode = packet->def.opcode + isaac_next(&player->isaac_out);
		codec_put8(&player->codec, opcode);
		switch (packet->def.type) {
		case PACKET_LEN_FIXED:
			break;
		case PACKET_LEN_8:
			codec_put8(&player->codec, payload_len);
			break;
		case PACKET_LEN_16:
			codec_put16(&player->codec, payload_len);
			break;
		}
		codec_concat(&player->codec, &packet->payload);
		if (codec_buffer_write(&player->codec, &client->write_buffer)) {
			packet_free(packet);
		} else {
			WARN("unable to write packet");
			queue_push(&player->packet_queue_out, &packet->node);
		}
	}
}

/**
 * game_process_io
 *
 * Dispatches packets in the packet queue to their handler routines
 *  - game: The game service
 */
void game_process_io(game_service_t* game)
{
	list_node_t* player_node = list_front(&game->player_list);
	while (player_node != NULL) {
		player_t* player = container_of(player_node, player_t, node);
		while (!queue_empty(&player->packet_queue_in)) {
			list_node_t* packet_node = queue_pop(&player->packet_queue_in);
			packet_t* packet = container_of(packet_node, packet_t, node);
			packet_dispatch(player, packet);
			packet_free(packet);
		}
		player_node = player_node->next;
	}
}

/**
 * player_sync
 *
 * Syncs players, mobs, items etc. between clients
 *  - game_service: The game service
 */
void player_sync(game_service_t* game_service)
{
	list_node_t* player_node = list_front(&game_service->player_list);
	while (player_node != NULL) {
		player_t* player = container_of(player_node, player_t, node);
		player_node = player_node->next;

		player_logic_update(player);
	}

	player_node = list_front(&game_service->player_list);
	while (player_node != NULL) {
		player_t* player = container_of(player_node, player_t, node);
		player_node = player_node->next;

		if (player->login_stage == STAGE_COMPLETE) {
			/* todo: we need to remove the player from other player's local lists in the update packet when stage is STAGE_EXITING */
			// Check if the client has changed region
			if (player->mob.update_flags & MOB_FLAG_REGION_UPDATE) {
				player_enqueue_packet(player, packet_build_region_update(player));
			}
			player_enqueue_packet(player, packet_build_player_update(player));
		}

	  	if (player->login_stage == STAGE_EXITING) {
			// We've hopefully removed the player from everyone's local list now, log them out properly.
			player->login_stage = STAGE_CLEANUP;
		}
	}

	player_node = list_front(&game_service->player_list);
	while (player_node != NULL) {
		player_t* player = container_of(player_node, player_t, node);
		player_node = player_node->next;

		if (player->login_stage == STAGE_CLEANUP) {
  			player_logout(game_service, player);
			player_free(player);
			continue;
		}

		// Clear the client's update flags
		player->mob.update_flags = 0;
	}

}
