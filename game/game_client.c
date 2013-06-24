#include <game/game_client.h>

#include <math.h>

/**
 * game_client_logic_update
 *
 * Performs client logic updates (processing the walking queue etc.)
 *  - game_client: The game client
 */
void game_client_logic_update(game_client_t* game_client)
{
	mob_t* mob = &game_client->mob;
	region_t old_region = mob->region;
	if (mob->running) {
		mob_update_path(mob);
	}
	mob_update_path(mob);
	region_t new_region = origin_region(mob->pos);
	int delta_x = abs(new_region.x - old_region.x);
	int delta_y = abs(new_region.y - old_region.y);
	if (delta_x >= 5 || delta_y >= 5) {
		mob->update_flags |= MOB_FLAG_REGION_UPDATE;
		mob->region = new_region;
	}
}

/**
 * game_client_enqueue_packet
 *
 * Queues an outgoing packet for a given game client
 *  - game_client: The client
 *  - packet: The packet
 */
void game_client_enqueue_packet(game_client_t* game_client, packet_t* packet)
{
	queue_push(&game_client->packet_queue_out, &packet->node);
}
