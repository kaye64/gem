#include <util/stack.h>

#include <stddef.h>
#include <stdlib.h>

stack_t* stack_create(stack_t* stack, int size)
{
	if (stack == NULL) {
		stack = (stack_t*)malloc(sizeof(stack_t));
		stack->must_free = 1;
	} else {
		stack->must_free = 0;
	}
	stack->data = (void**)malloc(sizeof(void*) * size);
	stack->top = 0;
	stack->size = size;
	return stack;
}

void stack_free(stack_t* stack)
{
	free(stack->data);
	if (stack->must_free) {
		free(stack);
	}
}

int stack_push(stack_t* stack, void* src)
{
	if (stack_remaining(stack) == 0) {
		return 0;
	}
	stack->data[stack->top++] = src;
	return 1;
}

int stack_pop(stack_t* stack, void** dest)
{
	if (stack_remaining(stack) >= stack->size) {
		return 0;
	}
	*dest = stack->data[--stack->top];
	return 1;
}

int stack_remaining(stack_t* stack)
{
	return stack->size - stack->top;
}
