#ifndef _JAGGRAB_H_
#define _JAGGRAB_H_

#include <regex.h>
#include <stdbool.h>

#include <net/server.h>
#include <runite/cache.h>

struct archive_server {
	server_t io_server;
	cache_t* cache;
	regex_t request_regexp;
	char crc_table[80];
};
typedef struct archive_server archive_server_t;

struct archive_client {
	client_t io_client;
	char* file_buffer;
	int file_caret;
	int file_size;
};
typedef struct archive_client archive_client_t;

archive_server_t* jaggrab_create(cache_t* cache, const char* addr);
void jaggrab_free(archive_server_t* server);
void jaggrab_start(archive_server_t* server, struct ev_loop* loop);

#endif /* _JAGGRAB_H_ */
