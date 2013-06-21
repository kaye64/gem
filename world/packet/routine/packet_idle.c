/**
 * packet_idle.c
 *
 * Defines the packet routine for the Idle packet
 */
#include <world/packet/routine/packet_idle.h>

#include <util/log.h>

#define LOG_TAG "packet_idle"

void packet_idle(game_client_t* client, packet_t* packet)
{
	DEBUG("recieved idle packet from %s", client->username);
}
