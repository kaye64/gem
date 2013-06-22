#ifndef _MOB_H_
#define _MOB_H_

#include <stdint.h>

#include <world/location.h>

#define MOB_FLAG_REGION_UPDATE (1 << 0)
#define MOB_FLAG_WALK_UPDATE (1 << 1)
#define MOB_FLAG_RUN_UPDATE (1 << 2)
#define MOB_FLAG_APPEARANCE_UPDATE (1 << 3)

#define MOB_FLAG_MOVEMENT_UPDATE (MOB_FLAG_REGION_UPDATE | MOB_FLAG_WALK_UPDATE | MOB_FLAG_RUN_UPDATE)

struct mob {
	uint16_t update_flags;
	location_t pos;
};
typedef struct mob mob_t;

void mob_warp_to(mob_t* mob, location_t position);

#endif /* _MOB_H_ */
