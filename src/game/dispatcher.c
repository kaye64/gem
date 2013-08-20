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

/**
 * dispatcher.c
 *
 * Dispatches world clients depending on the service handshake
 */
#include <game/dispatcher.h>

#include <stdlib.h>
#include <runite/util/container_of.h>

#include <util/log.h>

#define LOG_TAG "dispatcher"

client_t* dispatcher_accept(int fd, struct in_addr addr, dispatcher_t* dispatcher);
int dispatcher_handshake(service_client_t* client);
void dispatcher_read(service_client_t* client);
void dispatcher_write(service_client_t* client);
void dispatcher_drop(service_client_t* client);

/**
 * Initializes a dispatcher
 */
static void dispatcher_init(dispatcher_t* dispatcher)
{
	server_t* base_server = &dispatcher->server;
	object_init(server, base_server);
	base_server->accept_cb = (client_accept_t)&dispatcher_accept;
	base_server->handshake_cb = (client_handshake_t)&dispatcher_handshake;
	base_server->read_cb = (client_read_t)&dispatcher_read;
	base_server->write_cb = (client_write_t)&dispatcher_write;
	base_server->drop_cb = (client_drop_t)&dispatcher_drop;
}

/**
 * Properly frees a dispatcher
 */
static void dispatcher_free(dispatcher_t* dispatcher)
{
	object_free(&dispatcher->server);
}

/**
 * Configures a dispatcher
 *  - cache: The cache to serve
 */
void dispatcher_config(dispatcher_t* dispatcher, const char* addr, service_t* game, service_t* update)
{
	server_t* base_server = &dispatcher->server;
	server_config(base_server, addr, 43594, SF_PARTIAL_READ);
	dispatcher->game_service = game;
	dispatcher->update_service = update;
}

/**
 * Starts the server loop
 *  - loop: The event loop to listen on
 */
void dispatcher_start(dispatcher_t* dispatcher, struct ev_loop* loop)
{
	server_t* base_server = &dispatcher->server;
	server_start(base_server, loop);
	INFO("Listening on %s:%d", base_server->addr, base_server->port);
}

/**
 * Validates/initializes a new client_t
 *  - fd: The client's file descriptor
 *  - addr: The client's address
 * returns: A properly allocated client_t, NULL on client denied
 */
client_t* dispatcher_accept(int fd, struct in_addr addr, dispatcher_t* dispatcher)
{
	service_client_t* client = (service_client_t*)malloc(sizeof(service_client_t));
	client->service = NULL;
	client->attrib = NULL;
	return &client->client;
}

/**
 * Sets up a service_client to be handled by a given service
 *  - service_type: The service to assign to the client
 */
void dispatcher_set_service(dispatcher_t* dispatcher, service_client_t* service_client, int service_type)
{
	service_t* service = NULL;
	switch (service_type) {
	case SERVICE_GAME:
		service = dispatcher->game_service;
		DEBUG("dispatched client to game service");
		break;
	case SERVICE_UPDATE:
		service = dispatcher->update_service;
		DEBUG("dispatched client to update service");
		break;
	default:
		WARN("unknown service type %d", service_type);
		return;
	}

	service_client->service = service;
	if (service != NULL) {
		service_client->attrib = service_client->service->accept_cb(service_client);
	}
}

/**
 * Performs any handshake routines between the client/server
 * returns: One of HANDSHAKE_{DENIED,PENDING,ACCEPTED}
 */
int dispatcher_handshake(service_client_t* service_client)
{
	client_t* client = &service_client->client;
	dispatcher_t* dispatcher = container_of(client->server, dispatcher_t, server);
	if (service_client->service == NULL) {
		buffer_pushp(&client->read_buffer);

		unsigned char service_type = 0;
		if (buffer_read(&client->read_buffer, &service_type, 1) < 1) {
			buffer_popp(&client->read_buffer);
			return HANDSHAKE_PENDING;
		}
		buffer_dropp(&client->read_buffer);

		dispatcher_set_service(dispatcher, service_client, service_type);
		if (service_client->attrib == NULL) {
			return HANDSHAKE_DENIED;
		}
	}

	if (service_client->service == NULL) {
		return HANDSHAKE_PENDING;
	}

	return service_client->service->handshake_cb(service_client);
}

/**
 * Called when data is available to be read by the client
 */
void dispatcher_read(service_client_t* service_client)
{
	service_t* service = (service_t*)service_client->service;
	if (service == NULL) {
		return;
	}
	service->read_cb(service_client);
}

/**
 * Called to signal that we can write to the client
 */
void dispatcher_write(service_client_t* service_client)
{
	service_t* service = (service_t*)service_client->service;
	if (service == NULL) {
		return;
	}
	service->write_cb(service_client);
}

/**
 * Called to perform any client cleanup
 */
void dispatcher_drop(service_client_t* service_client)
{
	service_t* service = (service_t*)service_client->service;
	if (service == NULL) {
		return;
	}
	service->drop_cb(service_client);
	free(service_client);
}

object_proto_t dispatcher_proto = {
	.init = (object_init_t)dispatcher_init,
	.free = (object_free_t)dispatcher_free
};
