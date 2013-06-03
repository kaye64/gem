#include <world/dispatcher.h>

#include <stdlib.h>

#include <util/container_of.h>
#include <util/log.h>

#define LOG_TAG "dispatcher"

client_t* dispatcher_accept(int fd, struct in_addr addr, dispatcher_t* dispatcher);
int dispatcher_handshake(service_client_t* client);
void dispatcher_read(service_client_t* client);
void dispatcher_write(service_client_t* client);
void dispatcher_drop(service_client_t* client);

dispatcher_t* dispatcher_create(const char* addr, service_t* game, service_t* update)
{
	dispatcher_t* dispatcher = (dispatcher_t*)malloc(sizeof(dispatcher_t));
	server_t* base_server = &dispatcher->server;
	server_create(base_server, addr, 43594);
	base_server->accept_cb = (client_accept_t)&dispatcher_accept;
	base_server->handshake_cb = (client_handshake_t)&dispatcher_handshake;
	base_server->read_cb = (client_read_t)&dispatcher_read;
	base_server->write_cb = (client_write_t)&dispatcher_write;
	base_server->drop_cb = (client_drop_t)&dispatcher_drop;
	dispatcher->game_service = game;
	dispatcher->update_service = update;
	return dispatcher;
}

void dispatcher_free(dispatcher_t* dispatcher)
{
	server_free(&dispatcher->server);
	free(dispatcher);
}

void dispatcher_start(dispatcher_t* dispatcher, struct ev_loop* loop)
{
	server_t* base_server = (server_t*)dispatcher;
	server_start(base_server, loop);
	INFO("Listening on %s:%d", base_server->addr, base_server->port);
}

client_t* dispatcher_accept(int fd, struct in_addr addr, dispatcher_t* dispatcher)
{
	service_client_t* client = (service_client_t*)malloc(sizeof(service_client_t));
	client->service = NULL;
	client->attrib = NULL;
	return &client->client;
}

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

int dispatcher_handshake(service_client_t* service_client)
{
	client_t* client = &service_client->client;
	dispatcher_t* dispatcher = container_of(client->server, dispatcher_t, server);
	if (service_client->service == NULL) {
		buffer_pushp(&client->read_buffer);

		char service_type = 0;
		if (buffer_read(&client->read_buffer, &service_type, 1) < 1) {
			buffer_popp(&client->read_buffer);
			return HANDSHAKE_PENDING;
		}

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

void dispatcher_read(service_client_t* service_client)
{
	service_t* service = (service_t*)service_client->service;
	if (service == NULL) {
		return;
	}
	service->read_cb(service_client);
}

void dispatcher_write(service_client_t* service_client)
{
	service_t* service = (service_t*)service_client->service;
	if (service == NULL) {
		return;
	}
	service->write_cb(service_client);
}

void dispatcher_drop(service_client_t* service_client)
{
	service_t* service = (service_t*)service_client->service;
	if (service == NULL) {
		return;
	}
	service->drop_cb(service_client);
}
