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

#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

#include <runite/util/object.h>

#include <net/server.h>
#include <game/service.h>

typedef struct dispatcher dispatcher_t;

struct dispatcher {
	object_t object;
	server_t server;
	service_t* game_service;
	service_t* update_service;
};

extern object_proto_t dispatcher_proto;

void dispatcher_config(dispatcher_t* dispatcher, const char* addr, service_t* game, service_t* update);
void dispatcher_start(dispatcher_t* dispatcher, struct ev_loop* loop);

#endif /* _DISPATCHER_H_ */
