/**
 * packet.c
 *
 * Defines the packet data structure
 */
#include <world/packet.h>

#include <stdlib.h>

/**
 * packet_create
 *
 * Creates a packet_t from a given packet definition
 *  - packet_def: The packet definition
 * returns: The packet
 */
packet_t* packet_create(packet_def_t packet_def)
{
	packet_t* packet = (packet_t*)malloc(sizeof(packet_t));
	packet->def = packet_def;
	codec_create(&packet->payload);
	return packet;
}

/**
 * packet_free
 *
 * Frees a packet_t
 *  - packet: The packet
 */
void packet_free(packet_t* packet)
{
	codec_free(&packet->payload);
	free(packet);
}
