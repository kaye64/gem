#ifndef _PACKET_HANDLER_H_
#define _PACKET_HANDLER_H_

#include <game/packet/packet.h>
#include <game/game_service.h>

typedef void(*packet_cb)(player_t*, packet_t*);

void packet_dispatch(player_t* player, packet_t* packet);

#endif /* _PACKET_HANDLER_H_ */
