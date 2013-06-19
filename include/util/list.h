#ifndef _LIST_H_
#define _LIST_H_

#include <stdbool.h>
#include <stdlib.h>

struct list_node {
	struct list_node* prev;
	struct list_node* next;
};
typedef struct list_node list_node_t;

struct list {
	list_node_t* top;
	list_node_t* bottom;
	bool must_free;
};
typedef struct list list_t;

list_t* list_create(list_t* list);
void list_free(list_t* list);

bool list_empty(list_t* list);

void list_push_back(list_t* list, list_node_t* node);
void list_push_front(list_t* list, list_node_t* node);
void list_insert_before(list_t* list, list_node_t* other, list_node_t* node);
void list_insert_after(list_t* list, list_node_t* other, list_node_t* node);

list_node_t* list_back(list_t* list);
list_node_t* list_front(list_t* list);

void list_erase(list_t* list, list_node_t* node);

#endif /* _LIST_H_ */
