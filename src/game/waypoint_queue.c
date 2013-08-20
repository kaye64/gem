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
 * waypoint_queue.c
 *
 * Defines a mob's waypoint queue
 */
#include <game/waypoint_queue.h>

#include <runite/util/container_of.h>

int interpolate_coord(int i, int y);

/**
 * Initializes a waypoint queue
 */
static void waypoint_queue_init(waypoint_queue_t* queue)
{
	object_init(queue, &queue->waypoints);
}

/**
 * Frees a waypoint queue
 */
static void waypoint_queue_free(waypoint_queue_t* queue)
{
	waypoint_queue_clear(queue);
	object_free(&queue->waypoints);
}

/**
 * Clears a waypoint queue
 */
void waypoint_queue_clear(waypoint_queue_t* queue)
{
	while (!queue_empty(&queue->waypoints)) {
		list_node_t* node = queue_pop(&queue->waypoints);
		waypoint_t* waypoint = container_of(node, waypoint_t, node);
		free(waypoint);
	}
}

/**
 * Adds a point to a waypoint queue
 */
void waypoint_queue_push(waypoint_queue_t* queue, location_t point)
{
	waypoint_t* waypoint = (waypoint_t*)malloc(sizeof(waypoint_t));
	waypoint->point = point;
	queue_push(&queue->waypoints, &waypoint->node);
}

/**
 * Advances the waypoint queue
 * returns: The mob's next position
 */
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

/**
 * Empties a waypoint queue
 */
bool waypoint_queue_empty(waypoint_queue_t* queue)
{
	return queue_empty(&queue->waypoints);
}

/**
 * Interpolates between two points
 */
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

object_proto_t waypoint_queue_proto = {
	.init = (object_init_t)waypoint_queue_init,
	.free = (object_free_t)waypoint_queue_free
};
