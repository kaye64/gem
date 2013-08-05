/**
 * packet_tab_update.c
 *
 * Defines the tab update packet builder
 */
#include <game/packet/builder/packet_tab_update.h>

/**
 * Constructs a tab update packet
 */
packet_t* packet_build_tab_update(player_t* player, int tab)
{
	packet_t* tab_update = packet_create(packet_lookup(PACKET_TYPE_OUT, PKT_CL_TAB_UPDATE));
	codec_put16(&tab_update->payload, player->tab_interfaces[tab]);
	codec_put8f(&tab_update->payload, tab, CODEC_OFS128);
	return tab_update;
}
