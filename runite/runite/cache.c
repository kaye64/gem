#include <runite/cache.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include <runite/util/sorted_list.h>

int strcmp_wrap(void* a, void* b) {
	return strcmp((const char*)a, (const char*)b);
}

cache_t* cache_open_dir(const char* directory)
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

	cache_t* cache = cache_open(num_indices, (const char**)index_files, data_file);

	for (i = 0; i < num_indices; i++) {
		free(index_files[i]);
	}
	list_free(index_list);
	closedir(dir);
	return cache;
}

cache_t* cache_open(int num_indices, const char** indexFiles, const char* dataFile)
{
	cache_t* cache = (cache_t*)malloc(sizeof(cache_t));
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
}

uint32_t cache_query_size(cache_t* cache, int index, int file)
{
	return cache->indices[index][file].file_size;
}

int cache_get(cache_t* cache, int index, int file, char* buffer)
{
	if (file < 0 || file > cache->num_files[index]) {
		return 0;
	}
	cache_index_t cache_index = cache->indices[index][file];
	int current_block = cache_index.start_block;
	int write_caret = 0;
	int to_read = cache_index.file_size;
	int file_part = 0;

	while (to_read > 0) {
		if (current_block <= 0 || current_block > cache->num_blocks) {
			return 0;
		}
		cache_block_t block = cache->data[current_block];
		int read_this_block = to_read;
		if (read_this_block > 512) {
			read_this_block = 512;
		}
		if (block.file_id != file || block.file_pos != file_part || block.cache_id != index) {
			return 0;
		}
		memcpy(&buffer[write_caret], block.data, read_this_block);
		write_caret += read_this_block;
		current_block = block.next_block;
	}
	return 1;
}
