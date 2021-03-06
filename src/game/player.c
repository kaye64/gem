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
 * player.c
 *
 * Defines a player
 */
#include <game/player.h>

#include <math.h>

#include <script/hook.h>
#include <script/api/player.h>
#include <game/packet/builders.h>
#include <game/game_login.h>
#include <game/world.h>
#include <game/game_service.h>
#include <util/log.h>

#define LOG_TAG "player"

/**
 * Initializes a player
 */
static void player_init(player_t* player)
{
	object_init(codec, &player->codec);
	object_init(queue, &player->packet_queue_in);
	object_init(queue, &player->packet_queue_out);
	object_init(isaac, &player->isaac_in);
	object_init(isaac, &player->isaac_out);
	object_init(mob, &player->mob);
	object_init(entity_tracker, &player->state.known_players);
	object_init(item_collection, &player->inventory);
}

/**
 * Frees a player
 */
static void player_free(player_t* player)
{
	object_free(&player->inventory);
	object_free(&player->state.known_players);
	object_free(&player->mob);
	object_free(&player->isaac_out);
	object_free(&player->isaac_in);
	object_free(&player->packet_queue_out);
	object_free(&player->packet_queue_in);
	object_free(&player->codec);
}

/**
 * Performs player logic updates (processing the walking queue etc.)
 */
void player_tick_before(world_t* world, player_t* player)
{
	mob_t* mob = &player->mob;
	region_t current_region = mob->region;

	/* Update the player's path */
	int position_updated = 0;
	if (mob->running) {
		position_updated = mob_update_path(mob);
	}
	position_updated = mob_update_path(mob) || position_updated;

	/* Trigger script position update hook */
	if (position_updated) {
		player_position_args_t args = { .player = player, .warped = 0 };
		hook_notify(SCRIPT_HOOK_PLAYER_POSITION, (void*)&args);
	}

	/* Rebase the region on the player's location */
	location_t position = mob_position(mob);
	region_t new_region = center_region_on(position);
	int delta_x = abs(new_region.base.x - current_region.base.x);
	int delta_y = abs(new_region.base.y - current_region.base.y);
	// If they've moved more than 5 sectors in any direction,
	// rebase the region in the client
	if (delta_x >= 5 || delta_y >= 5) {
		mob->update_flags |= MOB_FLAG_REGION_UPDATE;
		mob->region = new_region;
	}

	/* Make sure the player is registered in the right sector */
	sector_t sector = position.sector;
	if (sector.x != mob->entity.known_sector.x || sector.y != mob->entity.known_sector.y) {
		world_sector_t* old_world_sector = world_get_sector(world, mob->entity.known_sector);
		world_sector_t* new_world_sector = world_get_sector(world, sector);
		mob->entity.known_sector = sector;
		sector_unregister_player(world, old_world_sector, player);
		sector_register_player(world, new_world_sector, player);
	}

	/* add any new players */
	world_sector_t** local_sectors = world_get_observed_sectors(world, sector);

	/* Notify the player of all other players in the surrounding sectors. */
	for (int i = 0; i < NUM_OBSERVED_SECTORS; i++) {
		world_sector_t* local_sector = local_sectors[i];

		player_t* other_player = NULL;
		list_for_each(&local_sector->players) {
			list_for_get(other_player);
			bool already_tracking = entity_tracker_is_tracking(&player->state.known_players, entity_for_player(other_player));
			location_t our_location = mob_position(mob_for_player(player));
			location_t other_location = mob_position(mob_for_player(other_player));
			int delta_x = abs(other_location.x - our_location.x);
			int delta_y = abs(other_location.y - our_location.y);
			bool within_range = (delta_x < ENTITY_OBSERVE_RADIUS && delta_y < ENTITY_OBSERVE_RADIUS);
			if (other_player == player || already_tracking || !within_range) {
				continue;
			}
			entity_tracker_add(&player->state.known_players, entity_for_player(other_player));
		}
	}

	/* remove any players we're no longer observing */
	entity_tracker_t* tracker = &player->state.known_players;
	tracked_entity_t* item = NULL;
	list_for_each(&tracker->entities) {
		list_for_get(item);
		if (tracked_entity_is_removing(item) || tracked_entity_is_adding(item)) {
			continue;
		}

		entity_t* entity = item->entity;
		player_t* other_player = player_for_entity(entity);

		location_t our_location = mob_position(mob_for_player(player));
		location_t other_location = mob_position(mob_for_player(other_player));
		int delta_x = abs(other_location.x - our_location.x);
		int delta_y = abs(other_location.y - our_location.y);
		bool within_range = (delta_x < ENTITY_OBSERVE_RADIUS && delta_y < ENTITY_OBSERVE_RADIUS);
		if (other_player->login_stage != STAGE_EXITING && within_range) {
			continue;
		}
		entity_tracker_remove(tracker, entity);
	}
}

/**
 * Performs post player update tick stuff
 */
void player_tick_after(world_t* world, player_t* player)
{
	mob_set_chat_message(mob_for_player(player), NULL);
	entity_tracker_tick(&player->state.known_players);
}

/**
 * Queues an outgoing packet for a given player
 */
void player_enqueue_packet(player_t* player, packet_t* packet)
{
	queue_push(&player->packet_queue_out, &packet->node);
}

/**
 * Called when a new player successfully logs in
 */
void player_login(game_service_t* game_service, player_t* player)
{
	world_t* world = &game_service->world;
	world_sector_t* sector = world_get_sector(world, player->mob.entity.known_sector);
	sector_register_player(world, sector, player);
	if (!entity_list_add(&game_service->player_list, &player->mob.entity)) {
		ERROR("Ran out of entity indices. Probably a bug..");
	}

	player_enqueue_packet(player, packet_build_player_init(player));

	hook_notify(SCRIPT_HOOK_PLAYER_LOGIN, (void*)player);
}

/**
 * Called when a new player logs out
 */
void player_logout(game_service_t* game_service, player_t* player)
{
	world_t* world = &game_service->world;
	world_sector_t* sector = world_get_sector(world, player->mob.entity.known_sector);
	hook_notify(SCRIPT_HOOK_PLAYER_LOGOUT, (void*)player);
	sector_unregister_player(world, sector, player);
	entity_list_remove(&game_service->player_list, &player->mob.entity);
}

/**
 * Warps a player to a position
 */
void player_warp_to(player_t* player, location_t position) {
	mob_t* mob = mob_for_player(player);
	mob_warp_to(mob, position);

	player_position_args_t args = { .player = player, .warped = 1 };
	hook_notify(SCRIPT_HOOK_PLAYER_POSITION, (void*)&args);
}

/**
 * Sets an interface in a player's client tab
 */
void player_set_tab_interface(player_t* player, int tab_id, int interface_id)
{
	player->state.update_flags |= STATE_TAB_UPDATE;
	player->state.tab_interfaces[tab_id] = interface_id;
}

/**
 * Forces a player logout on the next cycle
 */
void player_force_logout(player_t* player)
{
	player->state.update_flags |= STATE_LOGOUT;
}

void player_send_message(player_t* player, const char* message)
{
	player_enqueue_packet(player, packet_build_player_message(message));
}

/**
 * Gets a player_t* for an entity_t*
 */
player_t* player_for_entity(entity_t* entity)
{
	mob_t* mob = container_of(entity, mob_t, entity);
	return player_for_mob(mob);
}

/**
 * Gets a player_t* for a mob_t*
 */
player_t* player_for_mob(mob_t* mob)
{
	player_t* player = container_of(mob, player_t, mob);
	return player;
}

/**
 * Gets a mob_t* for a player_t*
 */
mob_t* mob_for_player(player_t* player)
{
	return &player->mob;
}

/**
 * Gets an entity_t* for a player_t*
 */
entity_t* entity_for_player(player_t* player)
{
	return entity_for_mob(mob_for_player(player));
}

object_proto_t player_proto = {
	.init = (object_init_t)player_init,
	.free = (object_free_t)player_free
};
