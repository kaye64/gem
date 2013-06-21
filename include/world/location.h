#ifndef _LOCATION_H_
#define _LOCATION_H_

struct location {
	int x;
	int y;
};
typedef struct location location_t;

int region_x(location_t loc);
int region_y(location_t loc);

#endif /* _LOCATION_H_ */
