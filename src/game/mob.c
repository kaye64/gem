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
 * mob.c
 *
 * Defines a movable entity (mob)
 */
#include <game/mob.h>

#include <math.h>

#include <util/log.h>

#define LOG_TAG "mob"

int mob_dir_map[3][3] = {
	{MOB_DIR_NORTH_EAST, MOB_DIR_EAST, MOB_DIR_SOUTH_EAST},
	{MOB_DIR_NORTH, MOB_DIR_NONE, MOB_DIR_SOUTH},
	{MOB_DIR_NORTH_WEST, MOB_DIR_WEST, MOB_DIR_SOUTH_WEST}
};

/**
 * Creates and initializes a mob_t
 */
static void mob_init(mob_t* mob)
{
	object_init(waypoint_queue, &mob->waypoint_queue);
	object_init(entity, &mob->entity);
	mob->direction = mob->last_direction = MOB_DIR_NONE;
	mob->running = false;
	mob->update_flags = 0;
	mob_warp_to(mob, absolute_coord(0, 0, 0));
	mob->chat_message = NULL;
}

/**
 * Frees a mob_t
 */
static void mob_free(mob_t* mob)
{
	object_free(&mob->entity);
	object_free(&mob->waypoint_queue);
}

/**
 * Changes the mob's position, clearing the walk queue
 */
void mob_warp_to(mob_t* mob, location_t position)
{
	mob->update_flags |= MOB_FLAG_REGION_UPDATE;
	entity_warp_to(&mob->entity, position);
	mob->region = center_region_on(mob->entity.position);
	waypoint_queue_clear(&mob->waypoint_queue);
}

/**
 * Advances a mob's walk queue by one step. Call twice in one cycle for running
 */
int mob_update_path(mob_t* mob)
{
	location_t position = mob_position(mob);
	location_t next_step = waypoint_queue_tick(&mob->waypoint_queue, position);
	if (next_step.x == -1 && next_step.y == -1) {
		return 0;
	}
	int delta_x = position.x - next_step.x;
	int delta_y = position.y - next_step.y;
	if (abs(delta_x) > 1 || abs(delta_y) > 1) {
		return 0;
	}
	mob->last_direction = mob->direction;
	mob->direction = mob_dir_map[delta_x+1][delta_y+1];
	mob->entity.position = next_step;
	if (mob->update_flags & MOB_FLAG_WALK_UPDATE) { // Called twice in one cycle
		mob->update_flags &= ~MOB_FLAG_WALK_UPDATE;
		mob->update_flags |= MOB_FLAG_RUN_UPDATE;
	} else {
		mob->update_flags |= MOB_FLAG_WALK_UPDATE;
	}
	return 1;
}

/**
 * Sets a mob's chat message
 */
void mob_set_chat_message(mob_t* mob, chat_message_t* message)
{
	if (mob->chat_message != NULL) {
		object_free(mob->chat_message);
		mob->chat_message = NULL;
	}
	if (message != NULL) {
		mob->chat_message = message;
		mob->update_flags |= MOB_FLAG_CHAT_UPDATE;
	}
}

/**
 * Returns the mob's position
 */
location_t mob_position(mob_t* mob)
{
	return entity_position(&mob->entity);
}

/**
 * Gets an entity_t* for a mob_t*
 */
entity_t* entity_for_mob(mob_t* mob)
{
	return &mob->entity;
}

object_proto_t mob_proto = {
	.init = (object_init_t)mob_init,
	.free = (object_free_t)mob_free
};
