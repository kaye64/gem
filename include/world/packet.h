#ifndef _PACKET_H_
#define _PACKET_H_

#include <net/stream_codec.h>
#include <world/packet_def.h>
#include <util/list.h>

struct packet {
	packet_def_t def;
	stream_codec_t payload;
	list_node_t node;
};
typedef struct packet packet_t;

packet_t* packet_create(packet_def_t packet_def);
void packet_free(packet_t* packet);

#endif /* _PACKET_H_ */
