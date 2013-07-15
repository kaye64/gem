#ifndef _ENTITY_LIST_H_
#define _ENTITY_LIST_H_

#include <util/object.h>
#include <util/list.h>
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
