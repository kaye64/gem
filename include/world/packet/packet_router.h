#ifndef _PACKET_HANDLER_H_
#define _PACKET_HANDLER_H_

#include <world/packet/packet.h>
#include <world/game_service.h>

typedef void(*packet_cb)(game_client_t*, packet_t*);

void packet_dispatch(game_client_t* client, packet_t* packet);

#endif /* _PACKET_HANDLER_H_ */
