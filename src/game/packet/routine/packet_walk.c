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
 * packet_walk.c
 *
 * Defines the packet routine for the walking packets
 */
#include <game/packet/routine/packet_walk.h>

#include <util/log.h>

#define LOG_TAG "packet_walk"

/**
 * Acts on the player walk packets
 */
void packet_walk(player_t* player, packet_t* packet)
{
	// The map walk packet appends 14 extra bytes
	// Not sure what it is yet. Maybe anti-cheat stuff. It's in method92. Ignore it for now.
	if (packet->def.opcode == PKT_SV_WALK_MAP) {
		packet->len -= 14;
	}
	mob_t* mob = &player->mob;
	waypoint_queue_clear(&mob->waypoint_queue);
	int waypoint_count = (packet->len-5)/2;
	int path[waypoint_count][2];
	int originX = codec_get16fp(&packet->payload, NULL, CODEC_OFS128 | CODEC_LITTLE);
	for (int i = 0; i < waypoint_count; i++) {
		int8_t wpX = codec_get8(&packet->payload) & 0xFF;
		int8_t wpY = codec_get8(&packet->payload) & 0xFF;
		path[i][0] = wpX;
		path[i][1] = wpY;
	}
	int originY = codec_get16fp(&packet->payload, NULL, CODEC_LITTLE);
	location_t origin = absolute_coord(originX, originY, 0);
	waypoint_queue_push(&mob->waypoint_queue, origin);
	for (int i = 0; i < waypoint_count; i++) {
		location_t loc = absolute_coord(path[i][0] + originX, path[i][1] + originY, 0);
		waypoint_queue_push(&mob->waypoint_queue, loc);
	}
	/* This byte is set to 1 when the player holds control,
	   however on my system (linux 3.9, openjdk 1.7) it doesn't seem to be recognized.
	   Is this a bug in the player? */
	player->mob.running = (player->mob.running || codec_get8(&packet->payload) == 1);
}
