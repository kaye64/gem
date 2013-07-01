/**
 * packet.c
 *
 * Defines the packet data structure
 */
#include <game/packet/packet.h>

#include <stdlib.h>

/**
 * Creates a packet_t from a given packet definition
 */
packet_t* packet_create(packet_def_t packet_def)
{
	packet_t* packet = (packet_t*)malloc(sizeof(packet_t));
	packet->def = packet_def;
	object_init(codec, &packet->payload);
	return packet;
}

/**
 * Frees a packet_t
 */
void packet_free(packet_t* packet)
{
	object_free(&packet->payload);
	free(packet);
}
