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
 * item_definition.c
 */
#include <game/item_definition.h>

#include <util/log.h>

#define LOG_TAG "item_definition"

item_definition_t item_database[7000];

int item_def_load()
{
	/* For now, create a dummy database */
	for (int i = 0; i < 7000; i++) {
		item_database[i].id = i;
		item_database[i].equip_slot = 1;
	}
	return 7000;
}

item_definition_t item_def_lookup(int id)
{
	if (id < 0 || id > 7000) {
		ERROR("Tried to lookup item out of bounds: %i", id);
		return item_database[0];
	}
	return item_database[id];
}
