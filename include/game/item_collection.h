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

#ifndef _ITEM_COLLECTION_H_
#define _ITEM_COLLECTION_H_

#include <game/item.h>

#include <runite/util/object.h>

struct item_collection {
	object_t object;
	int size;
	item_t* collection;
};
typedef struct item_collection item_collection_t;

extern object_proto_t item_collection_proto;

void item_collection_alloc(item_collection_t* item_collection, int size);
int item_collection_insert(item_collection_t* item_collection, item_t item);
int item_collection_insert_at(item_collection_t* item_collection, int index, item_t item);
int item_collection_contains(item_collection_t* item_collection, item_definition_t definition);
item_t* item_collection_find_first(item_collection_t* item_collection, item_definition_t definition);
int item_collection_count(item_collection_t* item_collection, item_definition_t definition);
int item_collection_remove(item_collection_t* item_collection, item_definition_t definition, int count);
item_t* item_collection_get(item_collection_t* item_collection, int index);

#endif /* _ITEM_COLLECTION_H_ */
