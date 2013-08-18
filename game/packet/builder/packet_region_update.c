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
 * packet_region_update.c
 *
 * Defines the region update packet builder
 */
#include <game/packet/builder/packet_region_update.h>

/**
 * Constructs a region update packet
 */
packet_t* packet_build_region_update(player_t* player)
{
	location_t location = mob_position(&player->mob);
	packet_t* region_update = packet_create(packet_lookup(PACKET_TYPE_OUT, PKT_CL_REGION_UPDATE));
	codec_put16f(&region_update->payload, location.sector.x, CODEC_OFS128);
	codec_put16(&region_update->payload, location.sector.y);
	return region_update;
}
