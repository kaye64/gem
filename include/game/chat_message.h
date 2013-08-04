#ifndef _CHAT_MESSAGE_H_
#define _CHAT_MESSAGE_H_

#include <util/object.h>

typedef struct chat_message chat_message_t;

struct chat_message {
	object_t object;
	int effects;
	int color;
	int length;
	unsigned char* message;
};

extern object_proto_t chat_message_proto;

#endif /* _CHAT_MESSAGE_H_ */
