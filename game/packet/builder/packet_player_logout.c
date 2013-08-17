/**
 * packet_player_logout.c
 *
 * Defines the player logout packet builder
 */
#include <game/packet/builder/packet_player_logout.h>

/**
 * Constructs a player logout packet
 */
packet_t* packet_build_player_logout(player_t* player)
{
	entity_t* entity = entity_for_player(player);
	packet_t* player_logout = packet_create(packet_lookup(PACKET_TYPE_OUT, PKT_CL_PLAYER_LOGOUT));
	return player_logout;
}
