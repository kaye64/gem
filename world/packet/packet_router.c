/**
 * packet_router.c
 *
 * Defines and handles routing for each packet handling routine
 */
#include <world/packet/packet_router.h>

#include <util/log.h>

/* packet handlers */
#include <world/packet/packet_idle.h>

#define LOG_TAG "packet_router"

struct packet_handler {
	int opcode;
	packet_cb callback;
};
typedef struct packet_handler packet_handler_t;

packet_handler_t routing_table[] = {
	{ .opcode = PKT_SV_IDLE, .callback = packet_idle },
	{ .opcode = PKT_NULL, .callback = NULL }
};

/**
 * packet_dispatch
 *
 * Looks up a packet in the routing table and dispatches it if possible
 *  - client: The originating client
 *  - packet: The packet
 */
void packet_dispatch(game_client_t* client, packet_t* packet)
{
	int opcode = packet->def.opcode;
	for (int i = 0; ; i++) {
		if (routing_table[i].opcode == PKT_NULL) {
			DEBUG("no handler for packet %d", opcode);
			return;
		}
		if (routing_table[i].opcode == opcode) {
			routing_table[i].callback(client, packet);
			return;
		}
	}
}
