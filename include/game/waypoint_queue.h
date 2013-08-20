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

#ifndef _WAYPOINT_QUEUE_H_
#define _WAYPOINT_QUEUE_H_

#include <runite/util/object.h>
#include <runite/util/queue.h>

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
