#ifndef _JAGGRAB_H_
#define _JAGGRAB_H_

#include <regex.h>
#include <stdbool.h>

#include <util/object.h>
#include <net/server.h>
#include <runite/cache.h>

typedef struct archive_server archive_server_t;
typedef struct archive_client archive_client_t;

struct archive_server {
	object_t object;
	server_t io_server;
	cache_t* cache;
	regex_t request_regexp;
	unsigned char crc_table[80];
};

extern object_proto_t archive_server_proto;

struct archive_client {
	client_t io_client;
	unsigned char* file_buffer;
	int file_caret;
	int file_size;
};

void jaggrab_config(archive_server_t* server, cache_t* cache, const char* addr);
void jaggrab_start(archive_server_t* server, struct ev_loop* loop);

#endif /* _JAGGRAB_H_ */
