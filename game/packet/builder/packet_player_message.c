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
