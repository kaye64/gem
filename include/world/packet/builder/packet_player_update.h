#ifndef _PACKET_PLAYER_UPDATE_H_
#define _PACKET_PLAYER_UPDATE_H_

#include <world/packet/packet.h>
#include <world/game_service.h>

packet_t* packet_build_player_update(game_client_t* game_client);

#endif /* _PACKET_PLAYER_UPDATE_H_ */
