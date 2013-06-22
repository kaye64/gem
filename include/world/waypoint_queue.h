#ifndef _WAYPOINT_QUEUE_H_
#define _WAYPOINT_QUEUE_H_

#include <util/queue.h>
#include <world/location.h>

struct waypoint {
	location_t point;
	list_node_t node;
};
typedef struct waypoint waypoint_t;

struct waypoint_queue {
	queue_t waypoints;
	bool must_free;
};
typedef struct waypoint_queue waypoint_queue_t;

waypoint_queue_t* waypoint_queue_create(waypoint_queue_t* queue);
void waypoint_queue_free(waypoint_queue_t* queue);

void waypoint_queue_clear(waypoint_queue_t* queue);
void waypoint_queue_push(waypoint_queue_t* queue, location_t point);
location_t waypoint_queue_tick(waypoint_queue_t* queue, location_t current_location);
bool waypoint_queue_empty(waypoint_queue_t* queue);

#endif /* _WAYPOINT_QUEUE_H_ */
