#ifndef _ENTITY_TRACKER_H_
#define _ENTITY_TRACKER_H_

#include <stdbool.h>

#include <util/object.h>
#include <util/list.h>
#include <game/entity.h>
#include <game/entity_list.h>

typedef struct entity_tracker entity_tracker_t;
typedef struct tracked_entity tracked_entity_t;

struct entity_tracker {
	object_t object;
	list_t entities;
};

struct tracked_entity {
	list_node_t node;
	entity_t* entity;
	int state; /* one of TRACK_STATE_* */
};

#define TRACK_STATE_INCOMING 0
#define TRACK_STATE_TRACKING 1
#define TRACK_STATE_OUTGOING 2

extern object_proto_t entity_tracker_proto;

void entity_tracker_add(entity_tracker_t* tracker, entity_t* entity);
void entity_tracker_remove(entity_tracker_t* tracker, entity_t* entity);
void entity_tracker_clear(entity_tracker_t* tracker);
void entity_tracker_tick(entity_tracker_t* tracker);
int entity_tracker_count_known(entity_tracker_t* tracker);
bool entity_tracker_is_tracking(entity_tracker_t* tracker, entity_t* entity);
bool entity_tracker_is_removing(entity_tracker_t* tracker, entity_t* entity);
entity_list_node_t* entity_tracker_front_in(entity_tracker_t* tracker);
entity_list_node_t* entity_tracker_front(entity_tracker_t* tracker);
entity_list_node_t* entity_tracker_front_out(entity_tracker_t* tracker);

bool tracked_entity_is_adding(tracked_entity_t* entity);
bool tracked_entity_is_removing(tracked_entity_t* entity);

#endif /* _ENTITY_TRACKER_H_ */
