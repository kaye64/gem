#ifndef _STACK_H_
#define _STACK_H_

#include <stdbool.h>

/* There's a naming conflict with something in signal.h that I can't
   be bothered to fix properly right now */
struct _stack {
	int stack[32];
	int bottom;
	bool must_free;
};
typedef struct _stack _stack_t;

_stack_t* stack_create(_stack_t* _stack);
void stack_free(_stack_t* _stack);

bool stack_push(_stack_t* _stack, int i);
int stack_pop(_stack_t* _stack);

#endif /* _STACK_H_ */
