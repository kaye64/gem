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

#ifndef _MOB_H_
#define _MOB_H_

#include <stdint.h>
#include <stdbool.h>
#include <runite/util/object.h>

#include <game/location.h>
#include <game/waypoint_queue.h>
#include <game/entity.h>
#include <game/chat_message.h>

/* update flags */
#define MOB_FLAG_REGION_UPDATE (1 << 0)
#define MOB_FLAG_WALK_UPDATE (1 << 1)
#define MOB_FLAG_RUN_UPDATE (1 << 2)
#define MOB_FLAG_IDENTITY_UPDATE (1 << 3)
#define MOB_FLAG_CHAT_UPDATE (1 << 4)

#define MOB_FLAG_MOVEMENT_UPDATE (MOB_FLAG_REGION_UPDATE | MOB_FLAG_WALK_UPDATE | MOB_FLAG_RUN_UPDATE)

/* mob directions */
#define MOB_DIR_NORTH 1
#define MOB_DIR_NORTH_EAST 2
#define MOB_DIR_EAST 4
#define MOB_DIR_SOUTH_EAST 7
#define MOB_DIR_SOUTH 6
#define MOB_DIR_SOUTH_WEST 5
#define MOB_DIR_WEST 3
#define MOB_DIR_NORTH_WEST 0
#define MOB_DIR_NONE -1

typedef struct mob mob_t;

struct mob {
	object_t object;
	uint16_t update_flags;
	entity_t entity;
	region_t region;
	waypoint_queue_t waypoint_queue; /* todo: expose via api */
	bool running; /* todo: expose via api */
	int direction;
	int last_direction;
	chat_message_t* chat_message;
};

extern object_proto_t mob_proto;

void mob_warp_to(mob_t* mob, location_t position);
int mob_update_path(mob_t* mob);
void mob_set_chat_message(mob_t* mob, chat_message_t* message); /* todo: expose via api */

location_t mob_position(mob_t* mob);

entity_t* entity_for_mob(mob_t* mob);

#endif /* _MOB_H_ */
