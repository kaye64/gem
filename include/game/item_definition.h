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

#ifndef _ITEM_DEF_H_
#define _ITEM_DEF_H_

#ifdef SWIG
#define SWIG_DEF(x) x;
#else
#define SWIG_DEF(x)
#endif /* SWIG */

SWIG_DEF(%rename(Definition) item_definition);
struct item_definition {
	SWIG_DEF(%immutable);
	int id;
	char name[64];
	int equip_slot;
	int equip_two_handed;
	int equip_full_body;
	int equip_full_hat;
	int equip_full_mask;
	int equip_req_attack;
	int equip_req_strength;
	int equip_req_defence;
	int equip_req_ranged;
	int equip_req_magic;
	SWIG_DEF(%mutable);
};
typedef struct item_definition item_definition_t;

#ifndef SWIG
int item_def_load();
#endif /* SWIG */

SWIG_DEF(%rename(Lookup) item_def_lookup);
item_definition_t item_def_lookup(int id);

#endif /* _ITEM_DEF_H_ */
