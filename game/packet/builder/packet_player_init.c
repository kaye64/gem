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
 * packet_player_init.c
 *
 * Defines the player init packet builder
 */
#include <game/packet/builder/packet_player_init.h>

/**
 * Constructs a player init packet
 */
packet_t* packet_build_player_init(player_t* player)
{
	entity_t* entity = entity_for_player(player);
	packet_t* player_init = packet_create(packet_lookup(PACKET_TYPE_OUT, PKT_CL_PLAYER_INIT));
	codec_put8f(&player_init->payload, 1, CODEC_OFS128); // Membership status (1 = member)
	codec_put16f(&player_init->payload, entity->index, CODEC_OFS128);
	return player_init;
}
