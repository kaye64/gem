#ifndef _MOB_H_
#define _MOB_H_

#include <stdint.h>

#include <stdbool.h>

#include <util/object.h>
#include <game/location.h>
#include <game/waypoint_queue.h>

/* update flags */
#define MOB_FLAG_REGION_UPDATE (1 << 0)
#define MOB_FLAG_WALK_UPDATE (1 << 1)
#define MOB_FLAG_RUN_UPDATE (1 << 2)
#define MOB_FLAG_APPEARANCE_UPDATE (1 << 3)

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
	location_t pos;
	region_t region;
	sector_t sector;
	waypoint_queue_t waypoint_queue;
	bool running;
	int direction;
	int last_direction;
};

extern object_proto_t mob_proto;

void mob_warp_to(mob_t* mob, location_t position);
void mob_update_path(mob_t* mob);

#endif /* _MOB_H_ */
