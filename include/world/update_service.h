#ifndef _UPDATE_SERVICE_H_
#define _UPDATE_SERVICE_H_

#include <net/server.h>
#include <world/service.h>
#include <runite/cache.h>

#define PRIORITY_URGENT 2
#define PRIORITY_PRELOAD 1
#define PRIORITY_BACKGROUND 0

struct update_service {
	service_t service;
	cache_t* cache;
	int must_free;
};
typedef struct update_service update_service_t;

struct update_client {

};
typedef struct update_client update_client_t;

struct update_request {
	uint8_t cache_id;
	uint16_t file_id;
	uint8_t priority;
} __attribute__((packed));
typedef struct update_request update_request_t;

update_service_t* update_create(update_service_t* update, cache_t* cache);
void update_free(update_service_t* update);

#endif /* _UPDATE_SERVICE_H_ */
