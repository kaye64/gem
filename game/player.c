/**
 * player.c
 *
 * Defines a player
 */
#include <game/player.h>

#include <math.h>

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
}

/**
 * Frees a player
 */
static void player_free(player_t* player) {
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
void player_logic_update(world_t* world, player_t* player)
{
	mob_t* mob = &player->mob;
	region_t current_region = mob->region;

	/* Update the player's path */
	if (mob->running) {
		mob_update_path(mob);
	}
	mob_update_path(mob);

	/* Rebase the region on the player's location */
	region_t new_region = center_region_on(mob->pos);
	int delta_x = abs(new_region.base.x - current_region.base.x);
	int delta_y = abs(new_region.base.y - current_region.base.y);
	// If they've moved more than 5 sectors in any direction,
	// rebase the region in the client
	if (delta_x >= 5 || delta_y >= 5) {
		mob->update_flags |= MOB_FLAG_REGION_UPDATE;
		mob->region = new_region;
	}

	/* Make sure the player is registered in the right sector */
	sector_t sector = mob->pos.sector;
	if (sector.x != mob->sector.x || sector.y != mob->sector.y) {
		world_sector_t* old_world_sector = world_get_sector(world, mob->sector);
		world_sector_t* new_world_sector = world_get_sector(world, sector);
		mob->sector = sector;
		sector_unregister_player(old_world_sector, player);
		sector_register_player(new_world_sector, player);
	}
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
	world_sector_t* sector = world_get_sector(world, player->mob.sector);
	sector_register_player(sector, player);
	list_push_back(&game_service->player_list, &player->service_node);
	INFO("Player login: %s", player->username);
}

/**
 * Called when a new player logs out
 */
void player_logout(game_service_t* game_service, player_t* player)
{
	world_t* world = &game_service->world;
	world_sector_t* sector = world_get_sector(world, player->mob.sector);
	sector_unregister_player(sector, player);
	list_erase(&game_service->player_list, &player->service_node);
	INFO("Player logout: %s", player->username);
}

object_proto_t player_proto = {
	.init = (object_init_t)player_init,
	.free = (object_free_t)player_free
};
