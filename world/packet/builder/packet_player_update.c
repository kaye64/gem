#include <world/packet/builder/packet_player_update.h>

void build_movement_block(game_client_t* game_client, stream_codec_t* codec)
{
	mob_t* mob = &game_client->mob;
	uint8_t other_update_flags = (mob->update_flags & ~(MOB_FLAG_MOVEMENT_UPDATE));
	if (mob->update_flags) {
		codec_put_bits(codec, 1, 1); // We want to update this player
		if (mob->update_flags & MOB_FLAG_REGION_UPDATE) { // We need to load a new region
			location_t* location = &mob->pos;
			codec_put_bits(codec, 2, 3); // Update type 3 = warp to location
			codec_put_bits(codec, 2, location->z);
			codec_put_bits(codec, 1, 1); // discard walking queue
			codec_put_bits(codec, 1, 1); // Signals that this player will have an entry in the update block
			codec_put_bits(codec, 7, local_y(*location));
			codec_put_bits(codec, 7, local_x(*location));
		} else if (mob->update_flags & MOB_FLAG_WALK_UPDATE) {

		} else if (mob->update_flags & MOB_FLAG_RUN_UPDATE) {

		} else {
			codec_put_bits(codec, 2, 0); // Update type 0 = No movement update
		}
	} else {
		codec_put_bits(codec, 1, 0); // No updates
	}
	mob->update_flags &= ~MOB_FLAG_MOVEMENT_UPDATE; // Clear movement specific flags
}

packet_t* packet_build_player_update(game_client_t* game_client)
{
	packet_t* player_update = packet_create(packet_lookup(PACKET_TYPE_OUT, PKT_CL_PLAYER_UPDATE));
	codec_set_bit_access_mode(&player_update->payload, true);
	build_movement_block(game_client, &player_update->payload);
	codec_put_bits(&player_update->payload, 8, 0); // The number of other players to update
	codec_put_bits(&player_update->payload, 11, 2047); // Signals end of movement updates
	codec_set_bit_access_mode(&player_update->payload, false);
	return player_update;
}
