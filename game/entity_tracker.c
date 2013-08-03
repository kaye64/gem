/**
 * entity_tracker.c
 *
 * Represents a player's local list of watched entities
 */
#include <game/entity_tracker.h>

#include <util/container_of.h>
#include <util/log.h>

#define LOG_TAG "entity_tracker"

/**
 * Creates and initializes an entity_tracker_t
 */
static void entity_tracker_init(entity_tracker_t* entity_tracker)
{
	object_init(list, &entity_tracker->in_entities);
	object_init(list, &entity_tracker->entities);
	object_init(list, &entity_tracker->out_entities);
}

/**
 * Frees an entity_tracker_t
 */
static void entity_tracker_free(entity_tracker_t* entity_tracker)
{
	object_free(&entity_tracker->in_entities);
	object_free(&entity_tracker->entities);
	object_free(&entity_tracker->out_entities);
}

/**
 * Moves all entities from one list to another
 * if dest is null, the entities are freed instead
 */
static void list_move_all(list_t* source, list_t* dest)
{
	list_node_t* node_iter = list_front(source);
	entity_list_node_t* node = NULL;
	while (node_iter != NULL) {
		node = container_of(node_iter, entity_list_node_t, node);
		node_iter = node_iter->next;

		list_erase(source, &node->node);
		if (dest == NULL) {
			free(node);
		} else {
			list_push_back(dest, &node->node);
		}
	}
}

/**
 * Removes an entity from a list
 */
static void list_remove_entity(list_t* list, entity_t* entity)
{
	list_node_t* node_iter = list_front(list);
	entity_list_node_t* node = NULL;
	while (node_iter != NULL) {
		node = container_of(node_iter, entity_list_node_t, node);
		
		entity_t* this_entity = node->entity;
		if (this_entity == entity) {
			list_erase(list, node_iter);
			free(node);
			return;
		}
		node_iter = node_iter->next;
	}
}

/**
 * Moves all known entities into the outgoing list, and clears the incoming list
 */
void entity_tracker_clear(entity_tracker_t* tracker)
{
	list_move_all(&tracker->in_entities, NULL);
	list_move_all(&tracker->entities, &tracker->out_entities);
}

/**
 * Processes the incoming and outgoing lists
 */
void entity_tracker_tick(entity_tracker_t* tracker)
{
	/* move all incoming entities into the main entity list */
	list_move_all(&tracker->in_entities, &tracker->entities);

	/* remove all outgoing entities from the main entity list */
	list_node_t* node_iter = list_front(&tracker->out_entities);
	entity_list_node_t* node = NULL;
	while (node_iter != NULL) {
		node = container_of(node_iter, entity_list_node_t, node);
		node_iter = node_iter->next;

		entity_t* entity = node->entity;
		list_remove_entity(&tracker->entities, entity);
	}

	/* clear the outgoing list */
	list_move_all(&tracker->out_entities, NULL);
}

/**
 * Adds an entity to the tracker
 */
void entity_tracker_add(entity_tracker_t* tracker, entity_t* entity)
{
	entity_list_node_t* node = (entity_list_node_t*)malloc(sizeof(entity_list_node_t*));
	node->entity = entity;
	list_push_back(&tracker->in_entities, &node->node);
}

/**
 * Removes an entity from the tracker
 */
void entity_tracker_remove(entity_tracker_t* tracker, entity_t* entity)
{
	/* find the entity in our list */
	list_t* list = &tracker->entities;
	list_node_t* node_iter = list_front(list);
	entity_list_node_t* node = NULL;
	while (node_iter != NULL) {
		node = container_of(node_iter, entity_list_node_t, node);
		node_iter = node_iter->next;

		if (node->entity == entity) {
			/* found it. we can safely add it to the outgoing list */
			entity_list_node_t* new_node = (entity_list_node_t*)malloc(sizeof(entity_list_node_t*));
			new_node->entity = entity;
			list_push_back(&tracker->out_entities, &new_node->node);
			break;
		}
		node = NULL;
	}

	/* still haven't found it. search the incoming list, too.. */
	if (node == NULL) {
		list = &tracker->in_entities;
		node_iter = list_front(list);
		while (node_iter != NULL) {
			node = container_of(node_iter, entity_list_node_t, node);
			node_iter = node_iter->next;

			if (node->entity == entity) {
				list_erase(list, &node->node);
				break;
			}
			node = NULL;
		}
	}

	if (node == NULL) {
		/* doesn't exist */
		ERROR("tried to remove non-existant entity");
		return;
	}
}

/**
 * Checks if this tracker is tracking the entity
 */
bool entity_tracker_is_tracking(entity_tracker_t* tracker, entity_t* entity)
{
	list_node_t* node_iter = list_front(&tracker->entities);
	entity_list_node_t* node = NULL;
	while (node_iter != NULL) {
		node = container_of(node_iter, entity_list_node_t, node);
		node_iter = node_iter->next;
		
		if (node->entity == entity) {
			return true;
		}
	}

	node_iter = list_front(&tracker->in_entities);
	node = NULL;
	while (node_iter != NULL) {
		node = container_of(node_iter, entity_list_node_t, node);
		node_iter = node_iter->next;
		
		if (node->entity == entity) {
			return true;
		}
	}

	return false;
}

/**
 * Checks if this tracker is removing the entity
 */
bool entity_tracker_is_removing(entity_tracker_t* tracker, entity_t* entity)
{
	list_node_t* node_iter = list_front(&tracker->out_entities);
	entity_list_node_t* node = NULL;
	while (node_iter != NULL) {
		node = container_of(node_iter, entity_list_node_t, node);
		node_iter = node_iter->next;
		
		if (node->entity == entity) {
			return true;
		}
	}
	return false;
}

object_proto_t entity_tracker_proto = {
	.init = (object_init_t)entity_tracker_init,
	.free = (object_free_t)entity_tracker_free
};
