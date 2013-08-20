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
 * update_service.c
 *
 * The client update service
 */
#include <game/update_service.h>

#include <string.h>
#include <stdlib.h>
#include <runite/util/math.h>
#include <runite/util/container_of.h>
#include <runite/util/codec.h>

#include <util/log.h>
#include <game/dispatcher.h>

#define LOG_TAG "update"

static cache_file_t null_file = { .file_size = 0, .data = NULL };

void* update_service_accept(service_client_t* service_client);
int update_service_handshake(service_client_t* service_client);
void update_service_read(service_client_t* service_client);
void update_service_write(service_client_t* service_client);
void update_service_drop(service_client_t* service_client);

/**
 * Initializes an update_service_t
 */
static void update_service_init(update_service_t* update)
{
	update->service.accept_cb = (service_accept_t)&update_service_accept;
	update->service.handshake_cb = (service_handshake_t)&update_service_handshake;
	update->service.read_cb = (service_read_t)&update_service_read;
	update->service.write_cb = (service_write_t)&update_service_write;
	update->service.drop_cb = (service_drop_t)&update_service_drop;
}

/**
 * Properly frees the update service
 */
void update_service_free(update_service_t* update)
{

}

/**
 * Configures an update service
 * - cache: The cache to serve
 */
void update_config(update_service_t* update, cache_t* cache)
{
	update->cache = cache;
}

/**
 * Validates a new service_client_t
 * returns: An item to use as the service_client's attribute
 */
void* update_service_accept(service_client_t* service_client)
{
	update_client_t* client = (update_client_t*)malloc(sizeof(update_client_t));
	client->current_request = (update_request_t*)NULL;
	object_init(queue, &client->request_queue);
	return client;
}

/**
 * Performs any handshake routines between the client/server
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
 * Called when data is available to be read by the client
 */
void update_service_read(service_client_t* service_client)
{
	update_service_t* update_service = container_of(service_client->service, update_service_t, service);
	update_client_t* update_client = (update_client_t*)service_client->attrib;
	client_t* client = &service_client->client;
	cache_t* cache = update_service->cache;
	codec_t codec;
	object_init(codec, &codec);

	// Read the request
	if (!codec_buffer_read(&codec, &client->read_buffer, 4)) {
		goto exit;
	}

	update_request_t* req = (update_request_t*)malloc(sizeof(update_request_t));
	req->cache_id = codec_get8(&codec);
	req->file_id = codec_get16(&codec);
	req->priority = codec_get8(&codec);
	req->cache_id++;

	// Validate it
	if (req->cache_id > cache->num_indices) {
		WARN("request for invalid cache id %d", req->cache_id);
		goto exit;
	}
	if (req->file_id > cache->num_files[req->cache_id]) {
		WARN("request for invalid file id %d in cache %d", req->file_id, req->cache_id);
		goto exit;
	}
	switch (req->priority) {
	case PRIORITY_URGENT:
	case PRIORITY_PRELOAD:
	case PRIORITY_BACKGROUND:
		break;
	default:
		WARN("request with invalid priority %d", req->priority);
		goto exit;
	}

	// Queue it
	queue_push(&update_client->request_queue, &req->list_node);

exit:
	object_free(&codec);
}

/**
 * Called to signal that we can write to the client
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
		request->file = cache_get_file(cache, request->cache_id, request->file_id);
		if (request->file == NULL) {
			WARN("unable to serve request for %d:%d. dropping", request->cache_id, request->file_id);
			/* Just serve a null file */
			request->file = &null_file;
		}

		update_client->current_request = request;
	}

	cache_file_t* cache_file = request->file;

	// Construct the chunk
	update_response_t chunk;
	chunk.cache_id = request->cache_id-1;
	chunk.file_id = request->file_id;
	chunk.file_size = cache_file->file_size;
	chunk.chunk_num = request->next_chunk;
	int ofs = request->next_chunk*500;
	int chunk_size = min(500, cache_file->file_size-ofs);

	codec_t codec;
	object_init(codec, &codec);
	codec_put8(&codec, chunk.cache_id);
	codec_put16(&codec, chunk.file_id);
	codec_put16(&codec, chunk.file_size);
	codec_put8(&codec, chunk.chunk_num);
	codec_putn(&codec, cache_file->data+ofs, chunk_size);

	if (!codec_buffer_write(&codec, &client->write_buffer)) {
		return;
	}

	object_free(&codec);

	request->next_chunk++;

	if ((uint16_t)(request->next_chunk*500) >= cache_file->file_size) {
		// Last chunk of the file, clean up
		free(update_client->current_request);
		update_client->current_request = (update_request_t*)NULL;
		request->file = NULL;
	}
}

/**
 * Called to perform any client cleanup
 */
void update_service_drop(service_client_t* service_client)
{
	update_client_t* update_client = (update_client_t*)service_client->attrib;
	while (!queue_empty(&update_client->request_queue)) {
		list_node_t* list_node = queue_pop(&update_client->request_queue);
		update_request_t* request = container_of(list_node, update_request_t, list_node);
		free(request);
	}
	object_free(&update_client->request_queue);
	free(update_client);
}

object_proto_t update_service_proto = {
	.init = (object_init_t)update_service_init,
	.free = (object_free_t)update_service_free
};
