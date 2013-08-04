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
