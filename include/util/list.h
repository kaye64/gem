#ifndef _LIST_H_
#define _LIST_H_

#include <stdbool.h>
#include <stdlib.h>

#include <util/object.h>

typedef struct list_node list_node_t;
typedef struct list list_t;

struct list_node {
	list_node_t* prev;
	list_node_t* next;
};

struct list {
	object_t object;
	list_node_t* top;
	list_node_t* bottom;
};

extern object_proto_t list_proto;

bool list_empty(list_t* list);

void list_push_back(list_t* list, list_node_t* node);
void list_push_front(list_t* list, list_node_t* node);
void list_insert_before(list_t* list, list_node_t* other, list_node_t* node);
void list_insert_after(list_t* list, list_node_t* other, list_node_t* node);

list_node_t* list_back(list_t* list);
list_node_t* list_front(list_t* list);
int list_count(list_t* list);

void list_erase(list_t* list, list_node_t* node);

#endif /* _LIST_H_ */
