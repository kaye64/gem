#ifndef _UPDATE_SERVICE_H_
#define _UPDATE_SERVICE_H_

#include <net/server.h>
#include <world/service.h>

struct update_service {
	service_t service;
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
};
typedef struct update_request update_request_t;

update_service_t* update_create(update_service_t* update);
void update_free(update_service_t* update);

#endif /* _UPDATE_SERVICE_H_ */
