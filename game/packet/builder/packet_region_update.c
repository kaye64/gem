#include <game/packet/builder/packet_region_update.h>

packet_t* packet_build_region_update(game_client_t* game_client)
{
	location_t* location = &game_client->mob.pos;
	packet_t* region_update = packet_create(packet_lookup(PACKET_TYPE_OUT, PKT_CL_REGION_UPDATE));
	int reg_x = region_x(*location);
	int reg_y = region_y(*location);
	codec_put16f(&region_update->payload, reg_x, CODEC_OFS128);
	codec_put16(&region_update->payload, reg_y);
	return region_update;
}
