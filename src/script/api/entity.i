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

%module(package="gem") Entity

%{
#include <game/entity.h>
#include <game/mob.h>
#include <game/player.h>
%}

%{
typedef struct player player;
typedef struct mob mob;
typedef struct entity entity;
typedef struct identity identity;
%}

%include "pyabc.i"
%include "stdint.i"

%import "location.i"

%rename(Entity) entity;
typedef struct {
	entity_index index;
	%immutable;
	location_t position;
	%mutable;
	%extend {
		void warp_to(location_t position);
	}
} entity;

%rename(Mob) mob;
%types(mob = entity) "return entity_for_mob($from);";
%pythonabc(mob, Entity);
typedef struct {
	region_t region;
	bool running;
} mob;

%rename(Identity, %$isclass) identity;
typedef struct {
	char username[32];
	char password[32];
	int combat_level;
	struct {
		int gender;
		int head_icon;
		/* models */
		int model_head;
		int model_head2;
		int model_cape;
		int model_neck;
		int model_equip_right;
		int model_equip_left;
		int model_torso;
		int model_arms;
		int model_legs;
		int model_hands;
		int model_feet;
		int model_beard;
		int color_hair;
		int color_torso;
		int color_leg;
		int color_feet;
		int color_skin;
	} appearance;
	int anim_idle;
	int anim_spot_rotate;
	int anim_walk;
	int anim_rotate_180;
	int anim_rotate_ccw;
	int anim_rotate_cw;
	int anim_run;
} identity;

%rename(Player) player;
%pythonabc(player, Mob);
%types(player = mob) "return mob_for_player($from);";
%types(player = entity) "return entity_for_player($from);";
typedef struct {
	char username[32];
	char password[32];
	uint32_t client_uid;
	uint32_t index;
	int rights;
	bool high_memory;
	identity identity;
	%extend {
		void set_tab_interface(int tab, int interface);
		void force_logout();
		void send_message(const char* message);
		void flush_identity() {
			$self->mob.update_flags |= MOB_FLAG_IDENTITY_UPDATE;
		}
		%immutable;
		PyObject* profile;
		%mutable;
	}
} player;

%{
	PyObject* player_profile_get(player_t* player) {
		PyObject* profile = (PyObject*)player->attachment;
		Py_INCREF(profile);
		return profile;
	}

	PyObject* api_player_in(player_t* player) {
		PyObject* result = SWIG_NewPointerObj(SWIG_as_voidptr(player), SWIGTYPE_p_player, 0 );
		return result;
	}

	player_t* api_player_out(PyObject* player) {
		player_t* native_obj;
		void* p;
		int res = SWIG_ConvertPtr(player, &p, SWIGTYPE_p_player, 0 );
		if (!SWIG_IsOK(res)) {
			log_error("SWIG", "Can't convert api player");
			return native_obj;
		}
		native_obj = (player_t*) p;
		return native_obj;
	}
%}
