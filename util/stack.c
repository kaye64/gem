#include <util/stack.h>

#include <stdlib.h>

/**
 * stack_create
 *
 * Initializes a stack
 *  - stack: Some preallocated memory or NULL to put on heap
 * returns: The stack
 */
_stack_t* stack_create(_stack_t* stack)
{
	if (stack == NULL) {
		stack = (_stack_t*)malloc(sizeof(_stack_t));
		stack->must_free = true;
	} else {
		stack->must_free = false;
	}
	stack->bottom = 0;
	return stack;
}

/**
 * stack_free
 *
 * Properly frees a stack
 *  - stack: The stack
 */
void stack_free(_stack_t* stack)
{
	if (stack->must_free) {
		free(stack);
	}
}

/**
 * stack_push
 *
 * Pushes an item i to a stack
 *  - stack: The stack
 *  - i: The item to push
 * returns: Whether the push was successful
 */
bool stack_push(_stack_t* stack, int i)
{
	if (stack->bottom < 32) {
		stack->stack[stack->bottom++] = i;
		return true;
	}
	return false;
}

/**
 * stack_pop
 *
 * Pops an item from a stack
 *  - stack: The stack
 * returns: The item
 */
int stack_pop(_stack_t* stack)
{
	if (stack->bottom > 0) {
		return stack->stack[--stack->bottom];
	}
	return 0;
}
