#include <world/location.h>

int region_x(location_t loc)
{
	return loc.x / 8;
}

int region_y(location_t loc)
{
	return loc.y / 8;
}
