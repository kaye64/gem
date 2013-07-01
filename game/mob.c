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
	mob->direction = mob->last_direction = MOB_DIR_NONE;
	mob->running = false;
	mob->update_flags = 0;
	mob_warp_to(mob, absolute_coord(0, 0, 0));
}

/**
 * Frees a mob_t
 */
static void mob_free(mob_t* mob)
{
	object_free(&mob->waypoint_queue);
}

/**
 * Changes the mob's position, clearing the walk queue
 *  - position: The location to warp to
 */
void mob_warp_to(mob_t* mob, location_t position)
{
	mob->update_flags |= MOB_FLAG_REGION_UPDATE;
	mob->pos = position;
	mob->region = center_region_on(mob->pos);
	mob->sector = position.sector;
	waypoint_queue_clear(&mob->waypoint_queue);
}

/**
 * Advances a mob's walk queue by one step. Call twice in one cycle for running
 */
void mob_update_path(mob_t* mob)
{
	location_t next_step = waypoint_queue_tick(&mob->waypoint_queue, mob->pos);
	if (next_step.x == -1 && next_step.y == -1) {
		return;
	}
	int delta_x = mob->pos.x - next_step.x;
	int delta_y = mob->pos.y - next_step.y;
	if (abs(delta_x) > 1 || abs(delta_y) > 1) {
		return;
	}
	mob->last_direction = mob->direction;
	mob->direction = mob_dir_map[delta_x+1][delta_y+1];
	mob->pos = next_step;
	if (mob->update_flags & MOB_FLAG_WALK_UPDATE) { // Called twice in one cycle
		mob->update_flags &= ~MOB_FLAG_WALK_UPDATE;
		mob->update_flags |= MOB_FLAG_RUN_UPDATE;
	} else {
		mob->update_flags |= MOB_FLAG_WALK_UPDATE;
	}
}

object_proto_t mob_proto = {
	.init = (object_init_t)mob_init,
	.free = (object_free_t)mob_free
};
