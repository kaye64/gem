#include <world/location.h>

int local_x(location_t loc)
{
	return loc.x - (8 * origin_region_x(loc));
}

int local_y(location_t loc)
{
	return loc.y - (8 * origin_region_y(loc));
}

int origin_region_x(location_t loc)
{
	return region_x(loc) - 6;
}

int origin_region_y(location_t loc)
{
	return region_y(loc) - 6;
}

int region_x(location_t loc)
{
	return loc.x / 8;
}

int region_y(location_t loc)
{
	return loc.y / 8;
}

region_t origin_region(location_t loc)
{
	region_t region = { .x = origin_region_x(loc), .y = origin_region_y(loc) };
	return region;
}

region_t region(location_t loc)
{
	region_t region = { .x = region_x(loc), .y = region_y(loc) };
	return region;
}
