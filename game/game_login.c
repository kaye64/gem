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
 * game_login.c
 *
 * Defines the player login process
 */
#include <game/game_login.h>

#include <util/log.h>

#define LOG_TAG "game_login"

int player_load(game_service_t* game_service, player_t* player);

/**
 * Performs the login sequence
 * returns: The login response
 */
int game_process_login(game_service_t* game_service, client_t* client, player_t* player)
{
	if (player->login_stage == STAGE_INIT) {
		codec_seek(&player->codec, 0);
		if (!codec_buffer_read(&player->codec, &client->read_buffer, 1)) {
			return LOGIN_PENDING;
		}

		// This is one byte of the hashed form of the user's login name.
		// Probably used to select a login server, but we don't need it.
		/*uint8_t name_hash = */codec_get8(&player->codec);

		codec_seek(&player->codec, 0);
		// Write the 8 ignored bytes
		for (int i = 0; i < 8; i++) {
			codec_put8(&player->codec, 0);
		}

		// Request the login packet
		codec_put8(&player->codec, 0);

		// Send the server's random seed
		player->server_isaac_seed = isaac_next(&game_service->rand_gen);
		player->server_isaac_seed <<= 32;
		player->server_isaac_seed += isaac_next(&game_service->rand_gen);
		codec_put64(&player->codec, player->server_isaac_seed);

		if (!codec_buffer_write(&player->codec, &client->write_buffer)) {
			return LOGIN_PENDING;
		}
		player->login_stage = STAGE_HANDSHAKE;
		return LOGIN_PENDING;
	}

	if (player->login_stage == STAGE_HANDSHAKE) {
		codec_seek(&player->codec, 0);
		// Read the login packet header
		if (!codec_buffer_read(&player->codec, &client->read_buffer, 2)) {
			return LOGIN_PENDING;
		}
		/*int login_type = */codec_get8(&player->codec);
		int login_len = codec_get8(&player->codec);

		if (!codec_buffer_read(&player->codec, &client->read_buffer, login_len)) {
			return LOGIN_PENDING;
		}

		if (codec_get8(&player->codec) != 255) {
			WARN("Client sent invalid login magic");
			return LOGIN_REJECTED;
		}
		uint16_t client_ver = codec_get16(&player->codec);
		uint8_t client_mem_type = codec_get8(&player->codec);

		if (client_ver != CLIENT_REVISION) {
			WARN("Client connected with unexpected revision %d", client_ver);
			return LOGIN_UPDATED;
		}

		if (client_mem_type > 1) {
			WARN("Client connected with invalid memory type %d", client_mem_type);
			return LOGIN_REJECTED;
		}

		player->high_memory = (client_mem_type == 1);

		uint32_t client_archive_crcs[9];
		for (int i = 0; i < 9; i++) {
			// todo: validate these values
			client_archive_crcs[i] = codec_get32(&player->codec);
		}
		player->login_stage = STAGE_SECURE_BLOCK;
		return LOGIN_PENDING;
	}

	if (player->login_stage == STAGE_SECURE_BLOCK) {
		// Decrypt the secure login block
		codec_block_decrypt(&player->codec, game_service->rsa);
		if (codec_get8(&player->codec) != 10) {
			WARN("Corrupt secure login block");
			return LOGIN_REJECTED;
		}

		// Verify isaac seed, create the RNGs
		player->client_isaac_seed = codec_get64(&player->codec);
		if (codec_get64(&player->codec) != player->server_isaac_seed) {
			WARN("Client modified server isaac seed");
			return LOGIN_REJECTED;
		}
		uint32_t seed[4];
		seed[0] = player->client_isaac_seed >> 32;
		seed[1] = player->client_isaac_seed;
		seed[2] = player->server_isaac_seed >> 32;
		seed[3] = player->server_isaac_seed;
		isaac_seed(&player->isaac_in, seed, 4);
		for (int i = 0; i < 4; i++) {
			seed[i] += 50;
		}
		isaac_seed(&player->isaac_out, seed, 4);

		player->client_uid = codec_get32(&player->codec);
		codec_gets(&player->codec, player->username, 32, CODEC_JSTRING);
		codec_gets(&player->codec, player->password, 32, CODEC_JSTRING);

		/* Until we have a profile loader, just accept the client */
		int response = player_load(game_service, player);
		player->login_stage = STAGE_COMPLETE;
		return response;
	}
	return LOGIN_PENDING;
}

/**
 * Loads a player's profile
 * returns: A login response
 */
int player_load(game_service_t* game_service, player_t* player)
{
	player->rights = PLAYER_RIGHTS_SUPER;
	location_t new_location = absolute_coord(3200, 3200, 0);
	mob_warp_to(&player->mob, new_location);
	player->mob.update_flags |= MOB_FLAG_APPEARANCE_UPDATE;
	return LOGIN_OKAY;
}
