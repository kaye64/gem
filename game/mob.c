#include <game/mob.h>

#include <util/log.h>

#define LOG_TAG "mob"

int mob_dir_map[3][3] = {
	{MOB_DIR_NORTH_EAST, MOB_DIR_EAST, MOB_DIR_SOUTH_EAST},
	{MOB_DIR_NORTH, MOB_DIR_NONE, MOB_DIR_SOUTH},
	{MOB_DIR_NORTH_WEST, MOB_DIR_WEST, MOB_DIR_SOUTH_WEST}
};

mob_t* mob_create(mob_t* mob)
{
	if (mob != NULL) {
		mob->must_free = false;
	} else {
		mob = (mob_t*)malloc(sizeof(mob_t));
		mob->must_free = true;
	}
	waypoint_queue_create(&mob->waypoint_queue);
	mob->direction = mob->last_direction = MOB_DIR_NONE;
	mob->running = false;
	return mob;
}

void mob_free(mob_t* mob)
{
	waypoint_queue_free(&mob->waypoint_queue);
	if (mob->must_free) {
		free(mob);
	}
}

void mob_warp_to(mob_t* mob, location_t position)
{
	mob->update_flags |= MOB_FLAG_REGION_UPDATE;
	mob->pos = position;
	waypoint_queue_clear(&mob->waypoint_queue);
}

void mob_update_path(mob_t* mob)
{
	location_t next_step = waypoint_queue_tick(&mob->waypoint_queue, mob->pos);
	if (next_step.x == -1 && next_step.y == -1) {
		return;
	}
	int delta_x = mob->pos.x - next_step.x;
	int delta_y = mob->pos.y - next_step.y;
	if (delta_x < -1 || delta_x > 1 || delta_y < -1 || delta_y > 1) {
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
