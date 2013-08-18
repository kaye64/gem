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
 * chat_message.c
 *
 * Defines a chat message
 */
#include <game/chat_message.h>

/**
 * Creates a new chat_message_t
 */
static void chat_message_init(chat_message_t* chat_message)
{
	chat_message->message = NULL;
}

/**
 * Properly frees a chat_message_t
 */
static void chat_message_free(chat_message_t* chat_message)
{
	if (chat_message->message != NULL) {
		free(chat_message->message);
	}
}

object_proto_t chat_message_proto = {
	.init = (object_init_t)chat_message_init,
	.free = (object_free_t)chat_message_free
};
