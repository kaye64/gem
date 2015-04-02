/**
 *  This file is part of Gem.
 *
 *  Gem is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Gem is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Gem.  If not, see <http://www.gnu.org/licenses/\>.
 */

/**
 * item_collection.c
 *
 * An item_collection has a unique index, a position, and 'known' sector (the
 * sector the item_collection was last registered in)
 */
#include <game/item_collection.h>

#include <string.h>

/**
 * Creates and initializes an item_collection_t
 */
static void item_collection_init(item_collection_t* item_collection)
{
	item_collection->collection = NULL;
}

/**
 * Frees an item_collection_t
 */
static void item_collection_free(item_collection_t* item_collection)
{
	if (item_collection->collection != NULL) {
		free(item_collection->collection);
	}
}

void item_collection_alloc(item_collection_t* item_collection, int size)
{
	item_collection->collection = calloc(size, sizeof(item_t));
	item_collection->size = size;
}

static int item_collection_find_first_empty(item_collection_t* item_collection)
{
	for (int i = 0; i < item_collection->size; i++) {
		item_t* item = &item_collection->collection[i];
		if (item->stack_count == 0) {
			return i;
		}
	}
	return -1;
}

int item_collection_insert(item_collection_t* item_collection, item_t item)
{
	int slot = item_collection_find_first_empty(item_collection);
	return item_collection_insert_at(item_collection, slot, item);
}

int item_collection_insert_at(item_collection_t* item_collection, int slot, item_t item)
{
	item_t* item_ptr = &item_collection->collection[slot];
	if (item_ptr->stack_count > 0) {
		return 0;
	}
	memcpy(item_ptr, &item, sizeof(item_t));
	return 1;
}

int item_collection_contains(item_collection_t* item_collection, item_definition_t definition)
{
	return item_collection_find_first(item_collection, definition) != NULL;
}

item_t* item_collection_find_first(item_collection_t* item_collection, item_definition_t definition)
{
	for (int i = 0; i < item_collection->size; i++) {
		item_t* item = &item_collection->collection[i];
		if (item->stack_count == 0) {
			continue;
		}
		if (item->definition.id == definition.id) {
			return item;
		}
	}
	return NULL;
}

int item_collection_count(item_collection_t* item_collection, item_definition_t definition)
{
	int count = 0;
	for (int i = 0; i < item_collection->size; i++) {
		item_t* item = &item_collection->collection[i];
		if (item->stack_count == 0) {
			continue;
		}
		if (item->definition.id != definition.id) {
			continue;
		}
		count += item->stack_count;
	}
	return count;
}

int item_collection_remove(item_collection_t* item_collection, item_definition_t definition, int count)
{
	if (item_collection_count(item_collection, definition) < count) {
		return 0;
	}
	while (count > 0) {
		item_t* item = item_collection_find_first(item_collection, definition);
		if (count > item->stack_count) {
			count -= item->stack_count;
			item->stack_count = 0;
		} else {
			item->stack_count -= count;
			count = 0;
		}
	}
	return 1;
}

item_t* item_collection_get(item_collection_t* item_collection, int index)
{
	if (index < item_collection->size) {
		return NULL;
	}

	return &item_collection->collection[index];
}

object_proto_t item_collection_proto = {
	.init = (object_init_t)item_collection_init,
	.free = (object_free_t)item_collection_free
};
