#ifndef _API_LOCATION_H_
#define _API_LOCATION_H_

#include <Python.h>

#include <game/location.h>

typedef struct api_location api_location_t;
struct api_location {
	PyObject_HEAD
	location_t location;
	int absolute_x;
	int absolute_y;
	int absolute_z;
	int sector_x;
	int sector_y;
	int sector_z;
};

extern PyTypeObject location_type;

void api_location_init_type(PyObject* module);
PyObject* api_location_create(location_t location);
void api_location_init(api_location_t* api_location, location_t location);

#endif /* _API_LOCATION_H_ */
