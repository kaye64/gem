/**
 *  This file is part of Gem.
 *
 *  Gem is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Gem is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Gem.  If not, see <http://www.gnu.org/licenses/\>.
 */

#ifndef _PACKET_HANDLER_H_
#define _PACKET_HANDLER_H_

#include <game/packet/packet.h>
#include <game/game_service.h>

typedef void(*packet_cb)(player_t*, packet_t*);

void packet_dispatch(player_t* player, packet_t* packet);

#endif /* _PACKET_HANDLER_H_ */
