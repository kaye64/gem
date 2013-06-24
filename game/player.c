#include <game/player.h>

#include <math.h>

player_t* player_create(player_t* player)
{
	if (player == NULL) {
		player = (player_t*)malloc(sizeof(player_t));
		player->must_free = true;
	} else {
		player->must_free = false;
	}
	codec_create(&player->codec);
	queue_create(&player->packet_queue_in);
	queue_create(&player->packet_queue_out);
	mob_create(&player->mob);
	return player;
}

void player_free(player_t* player) {
	mob_free(&player->mob);
	queue_free(&player->packet_queue_out);
	queue_free(&player->packet_queue_in);
	codec_free(&player->codec);
	if (player->must_free) {
		free(player);
	}
}

/**
 * player_logic_update
 *
 * Performs player logic updates (processing the walking queue etc.)
 *  - player: The player
 */
void player_logic_update(player_t* player)
{
	mob_t* mob = &player->mob;
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
 * player_enqueue_packet
 *
 * Queues an outgoing packet for a given player
 *  - player: The player
 *  - packet: The packet
 */
void player_enqueue_packet(player_t* player, packet_t* packet)
{
	queue_push(&player->packet_queue_out, &packet->node);
}