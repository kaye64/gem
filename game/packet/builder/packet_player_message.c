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
 * packet_player_message.c
 *
 * Defines the player message packet builder
 */
#include <game/packet/builder/packet_player_message.h>

#include <string.h>

/**
 * Constructs a player message packet
 */
packet_t* packet_build_player_message(char* message)
{
	packet_t* player_message = packet_create(packet_lookup(PACKET_TYPE_OUT, PKT_CL_PLAYER_MESSAGE));
	codec_puts(&player_message->payload, message, strlen(message), CODEC_JSTRING);
	return player_message;
}
