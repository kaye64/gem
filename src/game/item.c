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
 * item.c
 *
 * Defines an item instance/stack
 */
#include <game/item.h>

item_t item_create(item_definition_t definition)
{
	item_t item;
	item.definition = definition;
	item.stack_count = 1;
	return item;
}

item_t item_create_by_id(int id)
{
	item_definition_t definition = item_def_lookup(id);
	return item_create(definition);
}
