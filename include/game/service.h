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

#ifndef _SERVICE_H_
#define _SERVICE_H_

#include <net/server.h>

#define SERVICE_GAME 14
#define SERVICE_UPDATE 15

struct service_client;

typedef void*(*service_accept_t)(struct service_client* client);
typedef int(*service_handshake_t)(struct service_client* client);
typedef void(*service_read_t)(struct service_client* client);
typedef void(*service_write_t)(struct service_client* client);
typedef void(*service_drop_t)(struct service_client* client);

struct service {
	service_accept_t accept_cb;
	service_handshake_t handshake_cb;
	service_read_t read_cb;
	service_write_t write_cb;
	service_drop_t drop_cb;
};
typedef struct service service_t;

struct service_client {
	client_t client;
	service_t* service;
	void* attrib;
};
typedef struct service_client service_client_t;

#endif /* _SERVICE_H_ */
