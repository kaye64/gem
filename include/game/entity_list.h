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

#ifndef _ENTITY_LIST_H_
#define _ENTITY_LIST_H_

#include <runite/util/object.h>
#include <runite/util/list.h>

#include <util/bitmap.h>
#include <game/entity.h>

typedef struct entity_list entity_list_t;
typedef struct entity_list_node entity_list_node_t;

struct entity_list {
	object_t object;
	bitmap_t index_bitmap;
	entity_index prev_index;
	list_t entities;
};

struct entity_list_node {
	entity_t* entity;
	list_node_t node;
};

extern object_proto_t entity_list_proto;

bool entity_list_add(entity_list_t* list, entity_t* entity);
void entity_list_remove(entity_list_t* list, entity_t* entity);
entity_list_node_t* entity_list_front(entity_list_t* list);

entity_list_node_t* entity_next(entity_list_node_t* node);

#endif /* _ENTITY_LIST_H_ */
