#ifndef _STACK_H_
#define _STACK_H_

#include <stdbool.h>

#include <util/object.h>

typedef struct int_stack int_stack_t;

struct int_stack {
	object_t object;
	int stack[32];
	int bottom;
};

extern object_proto_t int_stack_proto;

bool stack_push(int_stack_t* _stack, int i);
int stack_pop(int_stack_t* _stack);

#endif /* _STACK_H_ */
