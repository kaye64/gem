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
#include <net/stream_codec.h>
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
	if (buffer_write(&client->write_buffer, (unsigned char*)empty, 8) < 8) {
		buffer_popp(&client->write_buffer);
		return HANDSHAKE_PENDING;
	}
	buffer_dropp(&client->write_buffer);

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
	stream_codec_t codec;
	codec_create(&codec);

	// Read the request
	if (!codec_buffer_read(&codec, &client->read_buffer, 4)) {
		return;
	}

	update_request_t* req = (update_request_t*)malloc(sizeof(update_request_t));
	req->cache_id = codec_get8(&codec);
	req->file_id = codec_get16(&codec);
	req->priority = codec_get8(&codec);
	req->cache_id++;

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
	queue_push(&update_client->request_queue, &req->list_node);
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
		list_node_t* list_node = queue_pop(&update_client->request_queue);
		request = container_of(list_node, update_request_t, list_node);

		// Resolve the file
		request->next_chunk = 0;
		request->file_size = cache_query_size(cache, request->cache_id, request->file_id);
		request->payload = (unsigned char*)malloc(request->file_size);
		if (!cache_get(cache, request->cache_id, request->file_id, request->payload)) {
			WARN("unable to serve request for %d:%d. dropping", request->cache_id, request->file_id);
			return;
		}

		update_client->current_request = request;
	}

	// Construct the chunk
	update_response_t chunk;
	chunk.cache_id = request->cache_id-1;
	chunk.file_id = request->file_id;
	chunk.file_size = request->file_size;
	chunk.chunk_num = request->next_chunk;
	int ofs = request->next_chunk*500;
	int chunk_size = min(500, request->file_size-ofs);

	stream_codec_t codec;
	codec_create(&codec);
	codec_put8(&codec, chunk.cache_id);
	codec_put16(&codec, chunk.file_id);
	codec_put16(&codec, chunk.file_size);
	codec_put8(&codec, chunk.chunk_num);
	codec_putn(&codec, request->payload+ofs, chunk_size);

	if (!codec_buffer_write(&codec, &client->write_buffer)) {
		return;
	}

	request->next_chunk++;

	if (request->next_chunk*500 >= request->file_size) {
		// Last chunk of the file, clean up
		free(request->payload);
		free(update_client->current_request);
		update_client->current_request = (update_request_t*)NULL;
		request->payload = (unsigned char*)NULL;
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
