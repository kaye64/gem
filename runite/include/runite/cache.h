#ifndef _CACHE_H_
#define _CACHE_H_

#include <stdint.h>

struct cache_index {
	uint32_t file_size : 24;
	uint32_t start_block : 24;
} __attribute__((packed));
typedef struct cache_index cache_index_t;

struct cache_block {
	int file_id : 16;
	int file_pos : 16;
	int next_block : 24;
	int cache_id : 8;
	char data[512];
} __attribute__((packed));
typedef struct cache_block cache_block_t;

struct cache {
	int num_indices;
	int* num_files;
	int num_blocks;
	cache_index_t** indices;
	cache_block_t* data;
	int must_free;
};
typedef struct cache cache_t;

cache_t* cache_open_dir(cache_t* cache, const char* directory);
cache_t* cache_open(cache_t* cache, int num_indices, const char** indexFiles, const char* dataFile);
void cache_free(cache_t* cache);

uint32_t cache_query_size(cache_t* cache, int index, int file);
int cache_get(cache_t* cache, int index, int file, char* buffer);

#endif /* _CACHE_H_ */
