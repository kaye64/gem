/**
 * update_service.c
 *
 * The client update service
 */
#include <world/update_service.h>

#include <string.h>
#include <stdlib.h>

#include <util/math.h>
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
	client->current_request = (update_request_t*)NULL;
	queue_create(&client->request_queue);
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
	update_client_t* update_client = (update_client_t*)service_client->attrib;
	client_t* client = &service_client->client;
	cache_t* cache = update_service->cache;

	// Read the request
	buffer_pushp(&client->read_buffer);
	update_request_t* req = (update_request_t*)malloc(sizeof(update_request_t));
	if (buffer_read(&client->read_buffer, (char*)req, 4) < 4) {
		buffer_popp(&client->read_buffer);
		return;
	}
	req->cache_id++;
	req->file_id = to_host_order_s(req->file_id);

	// Validate it
	if (req->cache_id < 0 || req->cache_id > cache->num_indices) {
		WARN("request for invalid cache id %d", req->cache_id);
		return;
	}
	if (req->file_id < 0 || req->file_id > cache->num_files[req->cache_id]) {
		WARN("request for invalid file id %d in cache %d", req->file_id, req->cache_id);
		return;
	}
	switch (req->priority) {
	case PRIORITY_URGENT:
	case PRIORITY_PRELOAD:
	case PRIORITY_BACKGROUND:
		break;
	default:
		WARN("request with invalid priority %d", req->priority);
		return;
	}

	// Queue it
	queue_push(&update_client->request_queue, &req->queue_item);
}

/**
 * client_write
 *
 * Called to signal that we can write to the client
 *  - service_client: The service client
 */
void update_service_write(service_client_t* service_client)
{
	update_service_t* update_service = container_of(service_client->service, update_service_t, service);
	update_client_t* update_client = (update_client_t*)service_client->attrib;
	client_t* client = &service_client->client;
	cache_t* cache = update_service->cache;

	// Ensure we have a chunk to send
	update_request_t* request = update_client->current_request;
	if (request == NULL) {
		if (queue_empty(&update_client->request_queue)) {
			return;
		}
		queue_item_t* queue_item = queue_pop(&update_client->request_queue);
		request = container_of(queue_item, update_request_t, queue_item);

		// Resolve the file
		request->next_chunk = 0;
		request->file_size = cache_query_size(cache, request->cache_id, request->file_id);
		request->payload = (char*)malloc(request->file_size);
		if (!cache_get(cache, request->cache_id, request->file_id, request->payload)) {
			WARN("unable to serve request for %d:%d. dropping", request->cache_id, request->file_id);
			return;
		}

		update_client->current_request = request;
	}

	// Construct the chunk
	update_response_t chunk;
	chunk.cache_id = request->cache_id-1;
	chunk.file_id = to_net_order_s(request->file_id);
	chunk.file_size = to_net_order_s(request->file_size);
	chunk.chunk_num = request->next_chunk;
	int ofs = request->next_chunk*500;
	int chunk_size = min(500, request->file_size-ofs);

	buffer_pushp(&client->write_buffer);
	if (buffer_write(&client->write_buffer, (char*)&chunk, sizeof(update_response_t)) < sizeof(update_response_t)) {
		buffer_popp(&client->write_buffer);
		return;
	}
	if (buffer_write(&client->write_buffer, request->payload+ofs, chunk_size) < chunk_size) {
		buffer_popp(&client->write_buffer);
		return;
	}

	request->next_chunk++;

	if (request->next_chunk*500 >= request->file_size) {
		// Last chunk of the file, clean up
		free(request->payload);
		free(update_client->current_request);
		update_client->current_request = (update_request_t*)NULL;
		request->payload = (char*)NULL;
		request->next_chunk = 0;
	}
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
	queue_free(&update_client->request_queue);
	free(update_client);
}