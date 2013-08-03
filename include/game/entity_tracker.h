#ifndef _ENTITY_TRACKER_H_
#define _ENTITY_TRACKER_H_

#include <stdbool.h>

#include <util/object.h>
#include <util/list.h>
#include <game/entity.h>
#include <game/entity_list.h>

typedef struct entity_tracker entity_tracker_t;

struct entity_tracker {
	object_t object;
	list_t in_entities;
	list_t entities;
	list_t out_entities;
};

extern object_proto_t entity_tracker_proto;

void entity_tracker_add(entity_tracker_t* tracker, entity_t* entity);
void entity_tracker_remove(entity_tracker_t* tracker, entity_t* entity);
void entity_tracker_clear(entity_tracker_t* tracker);
void entity_tracker_tick(entity_tracker_t* tracker);
bool entity_tracker_is_tracking(entity_tracker_t* tracker, entity_t* entity);
bool entity_tracker_is_removing(entity_tracker_t* tracker, entity_t* entity);
entity_list_node_t* entity_tracker_front_in(entity_tracker_t* tracker);
entity_list_node_t* entity_tracker_front(entity_tracker_t* tracker);
entity_list_node_t* entity_tracker_front_out(entity_tracker_t* tracker);

#endif /* _ENTITY_TRACKER_H_ */










