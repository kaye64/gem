#ifndef _GAME_LOGIN_H_
#define _GAME_LOGIN_H_

#include <game/game_service.h>

/* Login stages */
#define STAGE_INIT 0
#define STAGE_HANDSHAKE 1
#define STAGE_SECURE_BLOCK 2
#define STAGE_COMPLETE 3
#define STAGE_EXITING 4
#define STAGE_CLEANUP 5

/* Login responses */
#define LOGIN_PENDING 0
#define LOGIN_DELAY 1
#define LOGIN_OKAY 2
#define LOGIN_INVALID_CREDENTIALS 3
#define LOGIN_DISABLED 4
#define LOGIN_DUPLICATE_SESSION 5
#define LOGIN_UPDATED 6
#define LOGIN_SERVER_FULL 7
#define LOGIN_NO_LOGIN_SERVER 8
#define LOGIN_TOO_MANY_CONNECTIONS 9
#define LOGIN_BAD_SESSION_ID 10
#define LOGIN_REJECTED 11
#define LOGIN_MEMBERS_WORLD 12
#define LOGIN_INCOMPLETE 13
#define LOGIN_UPDATING 14
#define LOGIN_UNKNOWN 15 // What does this do?
#define LOGIN_ATTEMPTS_EXCEEDED 16
#define LOGIN_MEMBERS_AREA 17
#define LOGIN_INVALID_LOGIN_SERVER 20
#define LOGIN_INVALID_TRANSFERRING 21

int game_process_login(game_service_t* game_service, client_t* client, player_t* player);

void player_login(game_service_t* game_service, player_t* player);
void player_logout(game_service_t* game_service, player_t* player);

#endif /* _GAME_LOGIN_H_ */
