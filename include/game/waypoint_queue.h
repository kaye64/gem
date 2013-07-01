#ifndef _WAYPOINT_QUEUE_H_
#define _WAYPOINT_QUEUE_H_

#include <util/object.h>
#include <util/queue.h>
#include <game/location.h>

typedef struct waypoint waypoint_t;
typedef struct waypoint_queue waypoint_queue_t;

struct waypoint {
	location_t point;
	list_node_t node;
};

struct waypoint_queue {
	object_t object;
	queue_t waypoints;
};

extern object_proto_t waypoint_queue_proto;

void waypoint_queue_clear(waypoint_queue_t* queue);
void waypoint_queue_push(waypoint_queue_t* queue, location_t point);
location_t waypoint_queue_tick(waypoint_queue_t* queue, location_t current_location);
bool waypoint_queue_empty(waypoint_queue_t* queue);

#endif /* _WAYPOINT_QUEUE_H_ */
