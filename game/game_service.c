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
 * Initializes a game_service_t
 */
static void game_init(game_service_t* game)
{
	object_init(isaac, &game->rand_gen);
	object_init(entity_list, &game->player_list);
	object_init(world, &game->world);
	game->service.accept_cb = (service_accept_t)&game_service_accept;
	game->service.handshake_cb = (service_handshake_t)&game_service_handshake;
	game->service.read_cb = (service_read_t)&game_service_read;
	game->service.write_cb = (service_write_t)&game_service_write;
	game->service.drop_cb = (service_drop_t)&game_service_drop;
}

/**
 * Properly frees the game service
 */
static void game_free(game_service_t* game)
{
	object_free(&game->world);
	object_free(&game->player_list);
	object_free(&game->rand_gen);
}

/**
 * Configures the game service
 *  - rsa: The RSA keypair to use
 *  - cache: The cache to serve
 */
void game_config(game_service_t* game, rsa_t* rsa, cache_t* cache)
{
	game->rsa = rsa;
}

/**
 * Validates a new service_client_t
 * returns: An item to use as the service_client's attribute
 */
void* game_service_accept(service_client_t* service_client)
{
	player_t* player = object_new(player);
	player->login_stage = STAGE_INIT;
	return player;
}

/**
 * Called to perform any client cleanup
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
 * Performs any handshake routines between the client/server
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
 * Called when data is available to be read by the client
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
 * Called to signal that we can write to the client
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
 * Dispatches packets in the packet queue to their handler routines
 */
void game_process_io(game_service_t* game)
{
	entity_list_node_t* player_node = entity_list_front(&game->player_list);
	while (player_node != NULL) {
		player_t* player = player_for_entity(player_node->entity);
		while (!queue_empty(&player->packet_queue_in)) {
			list_node_t* packet_node = queue_pop(&player->packet_queue_in);
			packet_t* packet = container_of(packet_node, packet_t, node);
			packet_dispatch(player, packet);
			packet_free(packet);
		}
		player_node = entity_next(player_node);
	}
}

/**
 * Syncs players, mobs, items etc. between clients
 */
void player_sync(game_service_t* game_service)
{
	entity_list_node_t* player_node = entity_list_front(&game_service->player_list);
	while (player_node != NULL) {
		player_t* player = player_for_entity(player_node->entity);
		player_node = entity_next(player_node);

		player_tick_before(&game_service->world, player);
	}

	player_node = entity_list_front(&game_service->player_list);
	while (player_node != NULL) {
		player_t* player = player_for_entity(player_node->entity);
		player_node = entity_next(player_node);

		if (player->login_stage == STAGE_COMPLETE) {
			// Check if the client has changed region
			if (player->mob.update_flags & MOB_FLAG_REGION_UPDATE) {
				player_enqueue_packet(player, packet_build_region_update(player));
			}
			player_enqueue_packet(player, packet_build_player_update(player));

			// Check if we need to update tabs
			if (player->update_flags & PLAYER_FLAG_TAB_UPDATE) {
				for (int i = 0; i < 14; i++) {
					player_enqueue_packet(player, packet_build_tab_update(player, i));
				}
			}

			// Check if we need to log the player out
			if (player->update_flags & PLAYER_FLAG_LOGOUT) {
				player_enqueue_packet(player, packet_build_player_logout(player));
			}
		}

	  	if (player->login_stage == STAGE_EXITING) {
			// We've hopefully removed the player from everyone's local list now, log them out properly.
			player->login_stage = STAGE_CLEANUP;
		}
	}

	player_node = entity_list_front(&game_service->player_list);
	while (player_node != NULL) {
		player_t* player = player_for_entity(player_node->entity);
		player_node = player_node;
		player_node = entity_next(player_node);

		player_tick_after(&game_service->world, player);

		if (player->login_stage == STAGE_CLEANUP) {
  			player_logout(game_service, player);
			object_free(player);
			continue;
		}

		// Clear the client's update flags
		player->mob.update_flags = 0;
		player->update_flags = 0;
	}
}

object_proto_t game_service_proto = {
	.init = (object_init_t)game_init,
	.free = (object_free_t)game_free
};
