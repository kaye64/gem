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
 * packet_router.c
 *
 * Defines and handles routing for each packet handling routine
 */
#include <game/packet/packet_router.h>

#include <util/log.h>

/* packet handlers */
#include <game/packet/routine/packet_idle.h>
#include <game/packet/routine/packet_chat.h>
#include <game/packet/routine/packet_walk.h>
#include <game/packet/routine/packet_button_click.h>

#define LOG_TAG "packet_router"

struct packet_handler {
	int opcode;
	packet_cb callback;
};
typedef struct packet_handler packet_handler_t;

packet_handler_t routing_table[] = {
	{ .opcode = PKT_SV_IDLE, .callback = packet_idle },
	{ .opcode = PKT_SV_CHAT_MESSAGE, .callback = packet_chat },
	{ .opcode = PKT_SV_WALK_GAME, .callback = packet_walk },
	{ .opcode = PKT_SV_WALK_MAP, .callback = packet_walk },
	{ .opcode = PKT_SV_WALK_ENTITY, .callback = packet_walk },
	{ .opcode = PKT_SV_BUTTON_CLICK, .callback = packet_button_click },
	{ .opcode = PKT_NULL, .callback = NULL }
};

/**
 * Looks up a packet in the routing table and dispatches it if possible
 */
void packet_dispatch(player_t* player, packet_t* packet)
{
	int opcode = packet->def.opcode;
	for (int i = 0; ; i++) {
		if (routing_table[i].opcode == PKT_NULL) {
			DEBUG("no handler for packet %d", opcode);
			return;
		}
		if (routing_table[i].opcode == opcode) {
			routing_table[i].callback(player, packet);
			return;
		}
	}
}
