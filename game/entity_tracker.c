/**
 *  This file is part of Gem.
 *
 *  Gem is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Gem is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Gem.  If not, see <http://www.gnu.org/licenses/\>.
 */

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
	tracked_entity_t* item = NULL;
	list_for_each(&tracker->entities) {
		list_for_get(item);		
		entity_t* this_entity = item->entity;
		if (this_entity == entity) {
			switch (item->state) {
			case TRACK_STATE_INCOMING:
				list_erase(&tracker->entities, &item->node);
				free(item);
				break;
			case TRACK_STATE_TRACKING:
				item->state = TRACK_STATE_OUTGOING;
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
	tracked_entity_t* item = NULL;
	list_for_each(&tracker->entities) {
		list_for_get(item);
		switch (item->state) {
		case TRACK_STATE_INCOMING:
			list_erase(&tracker->entities, &item->node);
			free(item);
			break;
		case TRACK_STATE_TRACKING:
			item->state = TRACK_STATE_OUTGOING;
			break;
		}
	}
}

/**
 * Processes the incoming and outgoing lists
 */
void entity_tracker_tick(entity_tracker_t* tracker)
{
	tracked_entity_t* item = NULL;
	list_for_each(&tracker->entities) {
		list_for_get(item);
		switch (item->state) {
		case TRACK_STATE_INCOMING:
			item->state = TRACK_STATE_TRACKING;
			break;
		case TRACK_STATE_OUTGOING:
			list_erase(&tracker->entities, &item->node);
			free(item);
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
	tracked_entity_t* item = NULL;
	list_for_each(&tracker->entities) {
		list_for_get(item);
		if (item->state == TRACK_STATE_TRACKING || item->state == TRACK_STATE_OUTGOING) {
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
	tracked_entity_t* item = NULL;
	list_for_each(&tracker->entities) {
		list_for_get(item);		
		if (item->entity == entity) {
			return !tracked_entity_is_removing(item);
		}
	}
	return false;
}

/**
 * Checks if this tracker is removing the entity
 */
bool entity_tracker_is_removing(entity_tracker_t* tracker, entity_t* entity)
{
	tracked_entity_t* item = NULL;
	list_for_each(&tracker->entities) {
		list_for_get(item);
		if (item->entity == entity) {
			return tracked_entity_is_removing(item);
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
