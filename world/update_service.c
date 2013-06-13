/**
 * update_service.c
 *
 * The client update service
 */
#include <world/update_service.h>

#include <stdlib.h>

#include <util/log.h>
#include <util/container_of.h>
#include <world/dispatcher.h>
#include <runite/util/byte_order.h>

#define LOG_TAG "update"

void* update_service_accept(service_client_t* service_client);
int update_service_handshake(service_client_t* service_client);
void update_service_read(service_client_t* service_client);
void update_service_write(service_client_t* service_client);
void update_service_drop(service_client_t* service_client);

/**
 * update_create
 *
 * Allocates and creates an update_service_t
 *  - update: Some preallocated memory, or null to put on heap
 *  - cache: The cache to serve
 * returns: The update_service_t
 */
update_service_t* update_create(update_service_t* update, cache_t* cache)
{
	if (update == NULL) {
		update = (update_service_t*)malloc(sizeof(update_service_t));
		update->must_free = true;
	} else {
		update->must_free = false;
	}
	update->cache = cache;
	update->service.accept_cb = (service_accept_t)&update_service_accept;
	update->service.handshake_cb = (service_handshake_t)&update_service_handshake;
	update->service.read_cb = (service_read_t)&update_service_read;
	update->service.write_cb = (service_write_t)&update_service_write;
	update->service.drop_cb = (service_drop_t)&update_service_drop;
	return update;
}

/**
 * update_free
 *
 * Properly frees the update service
 *  - update: The service to free
 */
void update_free(update_service_t* update)
{
	if (update->must_free) {
		free(update);
	}
}

/**
 * update_service_accept
 *
 * Validates a new service_client_t
 *  - service_client: The service client
 * returns: An item to use as the service_client's attribute
 */
void* update_service_accept(service_client_t* service_client)
{
	update_client_t* client = (update_client_t*)malloc(sizeof(update_client_t));
	return client;
}

/**
 * update_service_handshake
 *
 * Performs any handshake routines between the client/server
 *  - service_client: The service client
 * returns: One of HANDSHAKE_{DENIED,PENDING,ACCEPTED}
 */
int update_service_handshake(service_client_t* service_client)
{
	client_t* client = &service_client->client;

	// Client reads but ignores 8 bytes
	static char* empty = "\0\0\0\0\0\0\0\0";
	buffer_pushp(&client->write_buffer);
	if (buffer_write(&client->write_buffer, empty, 8) < 8) {
		buffer_popp(&client->write_buffer);
		return HANDSHAKE_PENDING;
	}

	return HANDSHAKE_ACCEPTED;
}

/**
 * update_service_read
 *
 * Called when data is available to be read by the client
 *  - service_client: The service client
 */
void update_service_read(service_client_t* service_client)
{
	update_service_t* update_service = container_of(service_client->service, update_service_t, service);
	client_t* client = &service_client->client;

	// Read the request
	buffer_pushp(&client->read_buffer);
	update_request_t req;
	if (buffer_read(&client->read_buffer, (char*)&req, sizeof(update_request_t)) < sizeof(update_request_t)) {
		buffer_popp(&client->read_buffer);
		return;
	}
	req.file_id = to_host_order_s(req.file_id);

	// Validate it
	if (req.cache_id < 0 || req.cache_id > update_service->cache->num_indices) {
		WARN("request for invalid cache id %d", req.cache_id);
		return;
	}
	if (req.file_id < 0 || req.file_id > update_service->cache->num_files[req.cache_id]) {
		WARN("request for invalid file id %d in cache %d", req.file_id, req.cache_id);
		return;
	}
	switch (req.priority) {
	case PRIORITY_URGENT:
	case PRIORITY_PRELOAD:
	case PRIORITY_BACKGROUND:
		break;
	default:
		WARN("request with invalid priority %d", req.priority);
		return;
	}

	// Queue it
	DEBUG("request for %d:%d (%d)", req.cache_id, req.file_id, req.priority);
}

/**
 * client_write
 *
 * Called to signal that we can write to the client
 *  - service_client: The service client
 */
void update_service_write(service_client_t* service_client)
{

}

/**
 * client_drop
 *
 * Called to perform any client cleanup
 *  - service_client: The service client
 */
void update_service_drop(service_client_t* service_client)
{
	update_client_t* update_client = (update_client_t*)service_client->attrib;
	free(update_client);
}
