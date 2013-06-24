#ifndef _PACKET_PLAYER_UPDATE_H_
#define _PACKET_PLAYER_UPDATE_H_

#include <game/packet/packet.h>
#include <game/game_service.h>

packet_t* packet_build_player_update(player_t* player);

#endif /* _PACKET_PLAYER_UPDATE_H_ */
