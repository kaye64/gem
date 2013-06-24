#include <game/packet/builder/packet_region_update.h>

packet_t* packet_build_region_update(player_t* player)
{
	location_t location = player->mob.pos;
	packet_t* region_update = packet_create(packet_lookup(PACKET_TYPE_OUT, PKT_CL_REGION_UPDATE));
	codec_put16f(&region_update->payload, location.sector.x, CODEC_OFS128);
	codec_put16(&region_update->payload, location.sector.y);
	return region_update;
}
