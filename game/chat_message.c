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
