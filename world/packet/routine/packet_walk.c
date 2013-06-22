/**
 * packet_walk.c
 *
 * Defines the packet routine for the walking packets
 */
#include <world/packet/routine/packet_walk.h>

#include <util/log.h>

#define LOG_TAG "packet_walk"

void packet_walk(game_client_t* client, packet_t* packet)
{
	// The second walking packet appends 14 extra bytes
	// Not sure what it is yet. It's in method92. Ignore it for now.
	if (packet->def.opcode == PKT_SV_WALK_B) {
		packet->len -= 14;
	}
	mob_t* mob = &client->mob;
	waypoint_queue_clear(&mob->waypoint_queue);
	int waypoint_count = (packet->len-5)/2;
	int path[waypoint_count][2];
	int originX = codec_get16fp(&packet->payload, NULL, CODEC_OFS128 | CODEC_LITTLE);
	for (int i = 0; i < waypoint_count; i++) {
		int8_t wpX = codec_get8(&packet->payload) & 0xFF;
		int8_t wpY = codec_get8(&packet->payload) & 0xFF;
		path[i][0] = wpX;
		path[i][1] = wpY;
	}
	int originY = codec_get16fp(&packet->payload, NULL, CODEC_LITTLE);
	location_t origin = { .x = originX, .y = originY, .z = 0 };
	waypoint_queue_push(&mob->waypoint_queue, origin);
	for (int i = 0; i < waypoint_count; i++) {
		location_t loc = { .x = path[i][0] + originX, .y = path[i][1] + originY, .z = 0 };
		waypoint_queue_push(&mob->waypoint_queue, loc);
	}
	/* This byte is set to 1 when the player holds control,
	   however on my system (linux 3.9, openjdk 1.7) it doesn't seem to be recognized.
	   Is this a bug in the client? */
	client->mob.running = codec_get8(&packet->payload) == 1;
}
