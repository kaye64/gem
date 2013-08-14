#ifndef _API_MOB_H_
#define _API_MOB_H_

#include <Python.h>

#include <script/api/entity.h>

typedef struct mob mob_t;
typedef struct api_mob api_mob_t;
struct api_mob {
	api_entity_t entity;
	mob_t* mob;
};

extern PyTypeObject mob_type;

void api_mob_init_type(PyObject* module);
PyObject* api_mob_create(mob_t* mob);
void api_mob_init(api_mob_t* api_mob, mob_t* mob);

#endif /* _API_MOB_H_ */
