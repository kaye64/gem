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
	object_init(list, &entity_tracker->entities);
}

/**
 * Frees an entity_tracker_t
 */
static void entity_tracker_free(entity_tracker_t* entity_tracker)
{
	object_free(&entity_tracker->entities);
}


/**
 * Adds an entity to the tracker
 */
void entity_tracker_add(entity_tracker_t* tracker, entity_t* entity)
{
	tracked_entity_t* tracked_entity = (tracked_entity_t*)malloc(sizeof(tracked_entity_t));
	tracked_entity->entity = entity;
	tracked_entity->state = TRACK_STATE_INCOMING;
	list_push_back(&tracker->entities, &tracked_entity->node);
}

/**
 * Removes an entity from the tracker
 */
void entity_tracker_remove(entity_tracker_t* tracker, entity_t* entity)
{
	list_node_t* node_iter = list_front(&tracker->entities);
	tracked_entity_t* tracked_entity = NULL;
	while (node_iter != NULL) {
		tracked_entity = container_of(node_iter, tracked_entity_t, node);
		node_iter = node_iter->next;
		
		entity_t* this_entity = tracked_entity->entity;
		if (this_entity == entity) {
			switch (tracked_entity->state) {
			case TRACK_STATE_INCOMING:
				list_erase(&tracker->entities, &tracked_entity->node);
				free(tracked_entity);
				break;
			case TRACK_STATE_TRACKING:
				tracked_entity->state = TRACK_STATE_OUTGOING;
				break;
			}
			return;
		}
	}	
	DEBUG("tried to remove untracked entity");
}

/**
 * Moves all known entities into the outgoing list, and clears the incoming list
 */
void entity_tracker_clear(entity_tracker_t* tracker)
{
	list_node_t* node_iter = list_front(&tracker->entities);
	tracked_entity_t* tracked_entity = NULL;
	while (node_iter != NULL) {
		tracked_entity = container_of(node_iter, tracked_entity_t, node);
		
		switch (tracked_entity->state) {
		case TRACK_STATE_INCOMING:
			list_erase(&tracker->entities, &tracked_entity->node);
			free(tracked_entity);
			break;
		case TRACK_STATE_TRACKING:
			tracked_entity->state = TRACK_STATE_OUTGOING;
			break;
		}
		node_iter = node_iter->next;
	}
}

/**
 * Processes the incoming and outgoing lists
 */
void entity_tracker_tick(entity_tracker_t* tracker)
{
	list_node_t* node_iter = list_front(&tracker->entities);
	tracked_entity_t* tracked_entity = NULL;
	while (node_iter != NULL) {
		tracked_entity = container_of(node_iter, tracked_entity_t, node);
		node_iter = node_iter->next;
		
		switch (tracked_entity->state) {
		case TRACK_STATE_INCOMING:
			tracked_entity->state = TRACK_STATE_TRACKING;
			break;
		case TRACK_STATE_OUTGOING:
			list_erase(&tracker->entities, &tracked_entity->node);
			free(tracked_entity);
			break;
		}
	}
}

/**
 * Counts the number of known (non outgoing, non purging) entities
 */
int entity_tracker_count_known(entity_tracker_t* tracker)
{
	int count = 0;
	list_node_t* node_iter = list_front(&tracker->entities);
	tracked_entity_t* tracked_entity = NULL;
	while (node_iter != NULL) {
		tracked_entity = container_of(node_iter, tracked_entity_t, node);
		node_iter = node_iter->next;
		
		if (tracked_entity->state == TRACK_STATE_TRACKING || tracked_entity->state == TRACK_STATE_OUTGOING) {
			count++;
		}
	}
	return count;
}

/**
 * Checks if this tracker is tracking the entity
 */
bool entity_tracker_is_tracking(entity_tracker_t* tracker, entity_t* entity)
{
	list_node_t* node_iter = list_front(&tracker->entities);
	tracked_entity_t* tracked_entity = NULL;
	while (node_iter != NULL) {
		tracked_entity = container_of(node_iter, tracked_entity_t, node);
		node_iter = node_iter->next;
		
		if (tracked_entity->entity == entity) {
			return !tracked_entity_is_removing(tracked_entity);
		}
	}
	return false;
}

/**
 * Checks if this tracker is removing the entity
 */
bool entity_tracker_is_removing(entity_tracker_t* tracker, entity_t* entity)
{
	list_node_t* node_iter = list_front(&tracker->entities);
	tracked_entity_t* tracked_entity = NULL;
	while (node_iter != NULL) {
		tracked_entity = container_of(node_iter, tracked_entity_t, node);
		node_iter = node_iter->next;
		
		if (tracked_entity->entity == entity) {
			return tracked_entity_is_removing(tracked_entity);
		}
	}
	return false;
}

/**
 * Checks is a tracked_entity_t is incoming
 */
bool tracked_entity_is_adding(tracked_entity_t* entity)
{
	return entity->state == TRACK_STATE_INCOMING;
}

/**
 * Checks is a tracked_entity_t is outgoing
 */
bool tracked_entity_is_removing(tracked_entity_t* entity)
{
	return entity->state == TRACK_STATE_OUTGOING;
}

object_proto_t entity_tracker_proto = {
	.init = (object_init_t)entity_tracker_init,
	.free = (object_free_t)entity_tracker_free
};
