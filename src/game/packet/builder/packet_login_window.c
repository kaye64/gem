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
 * packet_login_window.c
 *
 * Defines the login window packet builder
 */
#include <game/packet/builder/packet_login_window.h>

/**
 * Constructs a login window packet
 */
packet_t* packet_build_login_window(player_t* player)
{
	packet_t* login_window = packet_create(packet_lookup(PACKET_TYPE_OUT, PKT_CL_LOGIN_WINDOW));
	codec_put8f(&login_window->payload, 0, CODEC_NEGATIVE); // days since recovery changed
	codec_put16f(&login_window->payload, 0, CODEC_OFS128); // num messages
	codec_put8(&login_window->payload, 0); // membership warning (??)
	codec_put32f(&login_window->payload, 0x7F000001, CODEC_MIDDLE_A); // last ip (127.0.0.1)
	codec_put16(&login_window->payload, 0); // last login
	return login_window;
}
