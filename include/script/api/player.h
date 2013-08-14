#ifndef _API_PLAYER_H_
#define _API_PLAYER_H_

#include <Python.h>

#include <script/api/mob.h>

typedef struct player player_t;
typedef struct api_player api_player_t;

struct api_player {
	api_mob_t mob;
	player_t* player;
};

PyObject* build_player_login_args(void* args);

void api_player_init_type(PyObject* module);
PyObject* api_player_create(player_t* player);
void api_player_init(api_player_t* api_player, player_t* player);

#endif /* _API_PLAYER_H_ */
