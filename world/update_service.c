#include <world/update_service.h>

#include <stdlib.h>

#include <util/log.h>
#include <util/container_of.h>
#include <world/dispatcher.h>

#define LOG_TAG "update"

void* update_service_accept(service_client_t* service_client);
int update_service_handshake(service_client_t* service_client);
void update_service_read(service_client_t* service_client);
void update_service_write(service_client_t* service_client);
void update_service_drop(service_client_t* service_client);

update_service_t* update_create(update_service_t* update)
{
	if (update == NULL) {
		update = (update_service_t*)malloc(sizeof(update_service_t));
		update->must_free = 1;
	} else {
		update->must_free = 0;
	}
	update->service.accept_cb = (service_accept_t)&update_service_accept;
	update->service.handshake_cb = (service_handshake_t)&update_service_handshake;
	update->service.read_cb = (service_read_t)&update_service_read;
	update->service.write_cb = (service_write_t)&update_service_write;
	update->service.drop_cb = (service_drop_t)&update_service_drop;
	return update;
}

void update_free(update_service_t* update)
{
	if (update->must_free == 1) {
		free(update);
	}
}

void* update_service_accept(service_client_t* service_client)
{
	update_client_t* client = (update_client_t*)malloc(sizeof(update_client_t));
	return client;
}

int update_service_handshake(service_client_t* service_client)
{
	return HANDSHAKE_ACCEPTED;
}

void update_service_read(service_client_t* service_client)
{

}

void update_service_write(service_client_t* service_client)
{

}

void update_service_drop(service_client_t* service_client)
{
	update_client_t* update_client = (update_client_t*)service_client->attrib;
	free(update_client);
}
