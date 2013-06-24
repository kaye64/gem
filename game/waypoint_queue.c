#include <game/waypoint_queue.h>

#include <util/container_of.h>

int interpolate_coord(int i, int y);

waypoint_queue_t* waypoint_queue_create(waypoint_queue_t* queue)
{
	if (queue != NULL) {
		queue->must_free = false;
	} else {
		queue = (waypoint_queue_t*)malloc(sizeof(waypoint_queue_t));
		queue->must_free = true;
	}
	queue_create(&queue->waypoints);
	return queue;
}

void waypoint_queue_free(waypoint_queue_t* queue)
{
	queue_free(&queue->waypoints);
	if (queue->must_free) {
		free(queue);
	}
}

void waypoint_queue_clear(waypoint_queue_t* queue)
{
	queue_clear(&queue->waypoints);
}

void waypoint_queue_push(waypoint_queue_t* queue, location_t point)
{
	waypoint_t* waypoint = (waypoint_t*)malloc(sizeof(waypoint_t));
	waypoint->point = point;
	queue_push(&queue->waypoints, &waypoint->node);
}

location_t waypoint_queue_tick(waypoint_queue_t* queue, location_t current_location)
{
	if (queue_empty(&queue->waypoints)) {
		location_t empty = { .x = -1, .y = -1, .z = -1 };
		return empty;
	}
	list_node_t* next_waypoint_node = queue_peek(&queue->waypoints);
	waypoint_t* next_waypoint = container_of(next_waypoint_node, waypoint_t, node);
	if (current_location.x == next_waypoint->point.x && current_location.y == next_waypoint->point.y) {
		// We've reached a waypoint. Remove it, and continue
		queue_pop(&queue->waypoints);
		free(next_waypoint);
		return waypoint_queue_tick(queue, current_location);
	}
	location_t next_position = absolute_coord(interpolate_coord(current_location.x, next_waypoint->point.x), interpolate_coord(current_location.y, next_waypoint->point.y), current_location.z);
	return next_position;
}

bool waypoint_queue_empty(waypoint_queue_t* queue)
{
	return queue_empty(&queue->waypoints);
}

int interpolate_coord(int i, int y) {
	if (i == y) {
		return i;
	} else if (i > y) {
		return --i;
	} else if (i < y) {
		return ++i;
	}
	return 0;
}
