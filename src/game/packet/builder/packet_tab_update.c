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
 * packet_tab_update.c
 *
 * Defines the tab update packet builder
 */
#include <game/packet/builder/packet_tab_update.h>

/**
 * Constructs a tab update packet
 */
packet_t* packet_build_tab_update(player_t* player, int tab)
{
	packet_t* tab_update = packet_create(packet_lookup(PACKET_TYPE_OUT, PKT_CL_TAB_UPDATE));
	codec_put16(&tab_update->payload, player->tab_interfaces[tab]);
	codec_put8f(&tab_update->payload, tab, CODEC_OFS128);
	return tab_update;
}
