#ifndef _UPDATE_SERVICE_H_
#define _UPDATE_SERVICE_H_

#include <stdbool.h>

#include <util/object.h>
#include <util/queue.h>
#include <net/server.h>
#include <game/service.h>
#include <runite/cache.h>

#define PRIORITY_URGENT 2
#define PRIORITY_PRELOAD 1
#define PRIORITY_BACKGROUND 0

typedef struct update_service update_service_t;
typedef struct update_request update_request_t;
typedef struct update_response update_response_t;
typedef struct update_client update_client_t;

struct update_service {
	object_t object;
	service_t service;
	cache_t* cache;
};

extern object_proto_t update_service_proto;

struct update_request {
	uint8_t cache_id;
	uint16_t file_id;
	uint8_t priority;
	size_t file_size;
	uint8_t next_chunk;
	unsigned char* payload;
	list_node_t list_node;
} __attribute__((packed));

struct update_response {
	uint8_t cache_id;
	uint16_t file_id;
	uint16_t file_size;
	uint8_t chunk_num;
} __attribute__((packed));

struct update_client {
	queue_t request_queue;
	update_request_t* current_request;
};

void update_config(update_service_t* update, cache_t* cache);

#endif /* _UPDATE_SERVICE_H_ */
