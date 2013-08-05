#ifndef _PACKET_TAB_UPDATE_H_
#define _PACKET_TAB_UPDATE_H_

#include <game/packet/packet.h>
#include <game/game_service.h>

packet_t* packet_build_tab_update(player_t* player, int tab);

#endif /* _PACKET_TAB_UPDATE_H_ */
