#include <runite/cache.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <netinet/in.h>
#include <zlib.h>

#include <runite/util/byte_order.h>
#include <runite/util/sorted_list.h>

int strcmp_wrap(void* a, void* b) {
	return strcmp((const char*)a, (const char*)b);
}

cache_t* cache_open_dir(cache_t* cache, const char* directory)
{
	DIR *dir = opendir(directory);
	struct dirent *entry;
	int num_indices = 0;

	link_t* index_list = NULL;

	if (dir == NULL) {
		return NULL;
	}

	char data_file[256];

	while ((entry = readdir(dir)) != NULL) {
		if (strstr(entry->d_name, "idx")) {
			index_list = list_insert(index_list, entry->d_name, strcmp_wrap);
			num_indices++;
		} else if (strstr(entry->d_name, "dat")) {
			sprintf(data_file, "%s/%s", directory, entry->d_name);
		}
	}

	if (data_file == NULL || num_indices == 0) {
		return NULL;
	}

	char* index_files[num_indices];
	int i = 0;
	for (link_t* index = list_top(index_list); index != NULL; index = index->next) {
		index_files[i] = (char*)malloc(256 * sizeof(char));
		sprintf(index_files[i++], "%s/%s", directory, (char*)index->data);
	}

	cache = cache_open(cache, num_indices, (const char**)index_files, data_file);

	for (i = 0; i < num_indices; i++) {
		free(index_files[i]);
	}
	list_free(index_list);
	closedir(dir);
	return cache;
}

cache_t* cache_open(cache_t* cache, int num_indices, const char** indexFiles, const char* dataFile)
{
	if (cache == NULL) { // Caller wants us to allocate and manage one
		cache = (cache_t*)malloc(sizeof(cache_t));
		cache->must_free = true;
	} else {
		cache->must_free = false;
	}
	cache->num_indices = num_indices;

	/* Read the data file into memory */
	FILE *data_fd = fopen(dataFile, "r");
	fseek(data_fd, 0, SEEK_END);
	int data_size = ftell(data_fd);
	int data_block_size = sizeof(cache_block_t);
	cache->num_blocks = data_size / data_block_size;
	fseek(data_fd, 0, SEEK_SET);
	cache->data = (cache_block_t*)calloc(cache->num_blocks, data_block_size);
	fread(cache->data, data_block_size, cache->num_blocks, data_fd);
	fclose(data_fd);

	/* Convert to local byte order */
	for (int i = 0; i < cache->num_blocks; i++) {
		cache->data[i].file_id = to_host_order_s(cache->data[i].file_id);
		cache->data[i].file_pos = to_host_order_s(cache->data[i].file_pos);
		cache->data[i].next_block = to_host_order_tri(cache->data[i].next_block);
	}

	/* Read the indices into memory */
	cache->indices = (cache_index_t**)calloc(num_indices, sizeof(cache_index_t*));
	cache->num_files = (int*)calloc(sizeof(int), num_indices);
	for (int i = 0; i < num_indices; i++) {
		FILE *index_fd = fopen(indexFiles[i], "r");
		fseek(index_fd, 0, SEEK_END);
		int index_size = ftell(index_fd);
		int index_entry_size = sizeof(cache_index_t);
		cache->num_files[i] = index_size / index_entry_size;
		cache->indices[i] = (cache_index_t*)calloc(cache->num_files[i], index_entry_size);
		fseek(index_fd, 0, SEEK_SET);
		fread(cache->indices[i], index_entry_size, cache->num_files[i], index_fd);
		fclose(index_fd);

		/* convert to local byte order */
		for (int x = 0; x < cache->num_files[i]; x++)
		{
			cache->indices[i][x].file_size = to_host_order_tri(cache->indices[i][x].file_size);
			cache->indices[i][x].start_block = to_host_order_tri(cache->indices[i][x].start_block);
		}
	}

	return cache;
}
void cache_free(cache_t* cache)
{
	free(cache->num_files);
	for (int i = 0; i < cache->num_indices; i++) {
		free(cache->indices[i]);
	}
	free(cache->indices);
	free(cache->data);
	if (cache->must_free) {
		free(cache);
	}
}

void cache_gen_crc(cache_t* cache, int index, char* buffer)
{
	int num_files = cache->num_files[index];
	size_t num_crcs = (num_files+1);
	size_t buf_len = num_crcs*4;
	uint32_t crc_buf[num_crcs];
	/* calculate the crc table */
	crc_buf[num_files] = 1234;
	for (int i = 0; i < num_files; i++) {
		size_t file_len = cache_query_size(cache, index, i);
		char file_buffer[file_len];
		cache_get(cache, index, i, file_buffer);
		crc_buf[i] = crc32(0L, Z_NULL, 0);
		crc_buf[i] = crc32(crc_buf[i], (const unsigned char*)file_buffer, file_len);
		crc_buf[num_files] = (crc_buf[num_files] << 1) + crc_buf[i];
		crc_buf[i] = htonl(crc_buf[i]);
	}
	crc_buf[num_files] = htonl(crc_buf[num_files]);
	memcpy(buffer, (char*)&crc_buf, buf_len);
}

uint32_t cache_query_size(cache_t* cache, int index, int file)
{
	return cache->indices[index][file].file_size;
}

bool cache_get(cache_t* cache, int index, int file, char* buffer)
{
	if (file < 0 || file > cache->num_files[index]) {
		return false;
	}
	cache_index_t cache_index = cache->indices[index][file];
	int current_block = cache_index.start_block;
	int write_caret = 0;
	int to_read = cache_index.file_size;
	int file_part = 0;

	while (current_block != 0) {
		if (current_block <= 0 || current_block > cache->num_blocks) {
			return false;
		}
		cache_block_t block = cache->data[current_block];
		int read_this_block = to_read;
		if (read_this_block > 512) {
			read_this_block = 512;
		}
		if (block.file_id != file || block.file_pos != file_part || block.cache_id-1 != index) {
			return 0;
		}
		memcpy(&buffer[write_caret], block.data, read_this_block);
		write_caret += read_this_block;
		to_read -= read_this_block;
		current_block = block.next_block;
		file_part++;
	}
	if (to_read > 0) {
		printf("file incomplete\n");
		return false;
	}
	return true;
}
