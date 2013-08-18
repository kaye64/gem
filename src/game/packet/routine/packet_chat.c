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
 * packet_chat.c
 *
 * Defines the packet routine for the Idle packet
 */
#include <game/packet/routine/packet_chat.h>

#include <game/chat_message.h>
#include <util/log.h>

#define LOG_TAG "packet_chat"

/**
 * Acts on the player chat packet
 */
void packet_chat(player_t* player, packet_t* packet)
{
	chat_message_t* message = object_new(chat_message);
	message->effects = codec_get8fp(&packet->payload, NULL, CODEC_OFS128);
	message->color = codec_get8fp(&packet->payload, NULL, CODEC_OFS128);
	message->length = packet->len - 2;
	message->message = (unsigned char*)malloc(message->length+1);
	unsigned char* message_enc = (unsigned char*)malloc(message->length);
	codec_getn(&packet->payload, message_enc, message->length);
	for (int i = 0; i < message->length; i++) {
		message->message[i] = message_enc[message->length - 1 - i] - 128;
	}
	mob_set_chat_message(mob_for_player(player), message);
}
