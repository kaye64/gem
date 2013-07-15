#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <stdint.h>

#include <util/object.h>
#include <game/location.h>

typedef struct entity entity_t;
typedef uint16_t entity_index;
#define ENTITY_INDEX_MAX UINT16_MAX // (65k)

struct entity {
	object_t object;
	entity_index index;
	location_t position;
	sector_t known_sector;
};

extern object_proto_t entity_proto;

void entity_warp_to(entity_t* entity, location_t position);
location_t entity_position(entity_t* entity);

#endif /* _ENTITY_H_ */
