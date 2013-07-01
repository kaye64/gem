/**
 * stack.c
 *
 * Defines a simple integer stack
 */
#include <util/stack.h>

#include <stdlib.h>

/**
 * Initializes a stack
 */
static void stack_init(int_stack_t* stack)
{
	stack->bottom = 0;
}

/**
 * Properly frees a stack
 */
static void stack_free(int_stack_t* stack)
{

}

/**
 * Pushes an item i to a stack
 * returns: Whether the push was successful
 */
bool stack_push(int_stack_t* stack, int i)
{
	if (stack->bottom < 32) {
		stack->stack[stack->bottom++] = i;
		return true;
	}
	return false;
}

/**
 * Pops an item from a stack
 */
int stack_pop(int_stack_t* stack)
{
	if (stack->bottom > 0) {
		return stack->stack[--stack->bottom];
	}
	return 0;
}

object_proto_t int_stack_proto = {
	.init = (object_init_t)stack_init,
	.free = (object_free_t)stack_free
};
