#ifndef _SERVICE_H_
#define _SERVICE_H_

#include <net/server.h>

#define SERVICE_GAME 14
#define SERVICE_UPDATE 15

struct service_client;

typedef void*(*service_accept_t)(struct service_client* client);
typedef int(*service_handshake_t)(struct service_client* client);
typedef void(*service_read_t)(struct service_client* client);
typedef void(*service_write_t)(struct service_client* client);
typedef void(*service_drop_t)(struct service_client* client);

struct service {
	service_accept_t accept_cb;
	service_handshake_t handshake_cb;
	service_read_t read_cb;
	service_write_t write_cb;
	service_drop_t drop_cb;
};
typedef struct service service_t;

struct service_client {
	client_t client;
	service_t* service;
	void* attrib;
};
typedef struct service_client service_client_t;

#endif /* _SERVICE_H_ */
