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
 * packet.c
 *
 * Defines the packet data structure
 */
#include <game/packet/packet.h>

#include <stdlib.h>

/**
 * Creates a packet_t from a given packet definition
 */
packet_t* packet_create(packet_def_t packet_def)
{
	packet_t* packet = (packet_t*)malloc(sizeof(packet_t));
	packet->def = packet_def;
	object_init(codec, &packet->payload);
	return packet;
}

/**
 * Frees a packet_t
 */
void packet_free(packet_t* packet)
{
	object_free(&packet->payload);
	free(packet);
}
