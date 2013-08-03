/**
 * player.c
 *
 * Defines a player
 */
#include <game/player.h>

#include <math.h>

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
	object_init(entity_tracker, &player->known_players);
}

/**
 * Frees a player
 */
static void player_free(player_t* player) {
	object_free(&player->known_players);
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
	if (mob->running) {
		mob_update_path(mob);
	}
	mob_update_path(mob);

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

	sector_t our_sector = mob->entity.known_sector;

	/* remove any players we're no longer observing */
	entity_tracker_t* tracker = &player->known_players;
	list_node_t* node_iter = list_front(&tracker->entities);
	entity_list_node_t* node = NULL;
	while (node_iter != NULL) {
		node = container_of(node_iter, entity_list_node_t, node);
		node_iter = node_iter->next;
		
		entity_t* entity = node->entity;
		player_t* other_player = player_for_entity(entity);
		if (entity == entity_for_player(player)) {
			continue;
		}
		sector_t sector = entity->position.sector;
		int sector_delta_x = abs(our_sector.x - sector.x);
		int sector_delta_y = abs(our_sector.y - sector.y);
		if (sector_delta_x > 1 || sector_delta_y > 1 || other_player->login_stage == STAGE_EXITING) {
			entity_tracker_remove(tracker, entity); 
		}
	}
}

/**
 * Performs post player update tick stuff
 */
void player_tick_after(world_t* world, player_t* player)
{
	entity_tracker_tick(&player->known_players);
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
	INFO("Player login: %s, index: %d", player->username, player->mob.entity.index);
}

/**
 * Called when a new player logs out
 */
void player_logout(game_service_t* game_service, player_t* player)
{
	world_t* world = &game_service->world;
	world_sector_t* sector = world_get_sector(world, player->mob.entity.known_sector);
	sector_unregister_player(world, sector, player);
	entity_list_remove(&game_service->player_list, &player->mob.entity);
	INFO("Player logout: %s", player->username);
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
mob_t* mob_for_player(player_t* player) {
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
