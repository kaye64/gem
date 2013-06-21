#include <world/mob.h>

void mob_warp_to(mob_t* mob, location_t position)
{
	mob->update_flags |= MOB_FLAG_REGION_UPDATE;
	mob->pos = position;
}
