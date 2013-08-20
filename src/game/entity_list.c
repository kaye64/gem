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
 * entity_list.c
 *
 * Tracks a set of entities, assigning them an index
 */
#include <game/entity_list.h>

#include <runite/util/container_of.h>

#include <util/log.h>

#define LOG_TAG "entity_list"

/**
 * Creates and initializes an entity_list_t
 */
static void entity_list_init(entity_list_t* entity_list)
{
	object_init(list, &entity_list->entities);
	object_init(bitmap, &entity_list->index_bitmap);
	bitmap_alloc(&entity_list->index_bitmap, ENTITY_INDEX_MAX);
	bitmap_clear(&entity_list->index_bitmap);
	bitmap_set(&entity_list->index_bitmap, 0, true); // We use index 0 as an error
	entity_list->prev_index = 1;
}

/**
 * Frees an entity_list_t
 */
static void entity_list_free(entity_list_t* entity_list)
{
	object_free(&entity_list->index_bitmap);
	object_free(&entity_list->entities);
}

/**
 * Finds the next valid index, returning 0 if none are available (unlikely)
 */
static int next_index(entity_list_t* entity_list)
{
	int index = entity_list->prev_index;
	if ((index = bitmap_find_next(&entity_list->index_bitmap, index, false)) == -1) {
		// None found, search from 0 instead
		if ((index = bitmap_find_next(&entity_list->index_bitmap, 0, false)) == -1) {
			ERROR("entity list overflow");
			return 0;
		}
	}
	bitmap_set(&entity_list->index_bitmap, index, true);
	entity_list->prev_index = index;
	return index;
}

/**
 * Returns the first entity node in the list
 */
entity_list_node_t* entity_list_front(entity_list_t* list) {
	list_node_t* node = list_front(&list->entities);
	if (node != NULL) {
		return container_of(node, entity_list_node_t, node);
	}
	return NULL;
}

/**
 * Gets the next entity in an entity list
 */
entity_list_node_t* entity_next(entity_list_node_t* node)
{
	list_node_t* next_node = node->node.next;
	if (next_node != NULL) {
		return container_of(next_node, entity_list_node_t, node);
	}
	return NULL;
}

/**
 * Adds an entity to the list, assigning it a new index
 */
bool entity_list_add(entity_list_t* list, entity_t* entity)
{
	entity_index idx = next_index(list);
	if (idx == 0) {
		return false;
	}
	entity->index = idx;

	entity_list_node_t* node = (entity_list_node_t*)malloc(sizeof(entity_list_node_t*));
	node->entity = entity;
	list_push_back(&list->entities, &node->node);
	return true;
}

/**
 * Removes an entity from the list, assigning it index 0
 */
void entity_list_remove(entity_list_t* list, entity_t* entity)
{
	/* find the entity in our list */
	list_t* real_list = &list->entities;
	entity_list_node_t* item = NULL;
	list_for_each(real_list) {
		list_for_get(item);

		if (item->entity == entity) {
			break;
		}
		item = NULL;
	}

	if (item == NULL) {
		ERROR("tried to remove non-existant entity with index %d", entity->index);
		return;
	}

	list_erase(real_list, &item->node);
}

object_proto_t entity_list_proto = {
	.init = (object_init_t)entity_list_init,
	.free = (object_free_t)entity_list_free
};
