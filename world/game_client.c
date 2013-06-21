#include <world/game_client.h>

/**
 * game_client_enqueue_packet
 *
 * Queues an outgoing packet for a given game client
 *  - game_client: The client
 *  - packet: The packet
 */
void game_client_enqueue_packet(game_client_t* game_client, packet_t* packet)
{
	queue_push(&game_client->packet_queue_out, &packet->node);
}
