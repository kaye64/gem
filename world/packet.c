#include <world/packet.h>

#include <stdlib.h>

packet_t* packet_create(packet_def_t packet_def)
{
	packet_t* packet = (packet_t*)malloc(sizeof(packet_t));
	packet->def = packet_def;
	codec_create(&packet->payload);
	return packet;
}

void packet_free(packet_t* packet)
{
	codec_free(&packet->payload);
	free(packet);
}
