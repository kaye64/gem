#ifndef _STACK_H_
#define _STACK_H_

struct stack {
	void** data;
	int top;
	int size;
	int must_free;
};
typedef struct stack stack_t;

stack_t* stack_create(stack_t* stack, int size);
void stack_free(stack_t* stack);
int stack_push(stack_t* stack, void* src);
int stack_pop(stack_t* stack, void** dest);
int stack_remaining(stack_t* stack);

#endif /* _STACK_H_ */
