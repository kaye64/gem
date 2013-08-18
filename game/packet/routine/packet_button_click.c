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
 * packet_button_click.c
 *
 * Defines the packet routine for the Idle packet
 */
#include <game/packet/routine/packet_button_click.h>

#include <script/hook.h>
#include <script/api/player.h>
#include <util/log.h>

#define LOG_TAG "packet_button_click"

/**
 * Acts on the player button click packet
 */
void packet_button_click(player_t* player, packet_t* packet)
{
	int button_id = codec_get16(&packet->payload);
	button_click_args_t args = { .player = player, .button = button_id };
	hook_notify(SCRIPT_HOOK_BUTTON_CLICK, (void*)&args);
}
