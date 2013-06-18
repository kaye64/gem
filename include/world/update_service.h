#ifndef _UPDATE_SERVICE_H_
#define _UPDATE_SERVICE_H_

#include <stdbool.h>

#include <net/server.h>
#include <world/service.h>
#include <util/queue.h>
#include <runite/cache.h>

#define PRIORITY_URGENT 2
#define PRIORITY_PRELOAD 1
#define PRIORITY_BACKGROUND 0

struct update_service {
	service_t service;
	cache_t* cache;
	bool must_free;
};
typedef struct update_service update_service_t;

struct update_request {
	uint8_t cache_id;
	uint16_t file_id;
	uint8_t priority;
	size_t file_size;
	int next_chunk;
	unsigned char* payload;
	queue_item_t queue_item;
} __attribute__((packed));
typedef struct update_request update_request_t;

struct update_response {
	uint8_t cache_id;
	uint16_t file_id;
	uint16_t file_size;
	uint8_t chunk_num;
} __attribute__((packed));
typedef struct update_response update_response_t;

struct update_client {
	queue_t request_queue;
	update_request_t* current_request;
};
typedef struct update_client update_client_t;

update_service_t* update_create(update_service_t* update, cache_t* cache);
void update_free(update_service_t* update);

#endif /* _UPDATE_SERVICE_H_ */
