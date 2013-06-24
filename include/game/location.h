#ifndef _LOCATION_H_
#define _LOCATION_H_

struct location {
	int x;
	int y;
	int z;
};
typedef struct location location_t;

struct region {
	int x;
	int y;
};
typedef struct region region_t;

int local_x(location_t loc);
int local_y(location_t loc);
int origin_region_x(location_t loc);
int origin_region_y(location_t loc);
int region_x(location_t loc);
int region_y(location_t loc);

region_t origin_region(location_t loc);
region_t region(location_t loc);

#endif /* _LOCATION_H_ */
