#include <world/packet/builder/packet_player_update.h>

#define CL_FLAG_APPEARANCE_UPDATE (1 << 4)

stream_codec_t* build_appearance_block(game_client_t* game_client);

/**
 * translate_update_flags
 *
 * Translates our server sided flags into the format the client wants
 *  - flags: Our server flags
 * returns: The client's flags
 */
uint16_t translate_update_flags(uint16_t flags)
{
	uint16_t new_flags = 0;
	if (flags & MOB_FLAG_APPEARANCE_UPDATE) {
		new_flags |= CL_FLAG_APPEARANCE_UPDATE;
	}
	return new_flags;
}

/**
 * build_movement_block
 *
 * Builds a movement update block for a given client. Assumes that
 * the given codec is already in bit access mode
 *  - game_client: The client
 *  - codec: The codec to write to
 */
void build_movement_block(game_client_t* game_client, stream_codec_t* codec)
{
	mob_t* mob = &game_client->mob;
	uint16_t other_update_flags = (mob->update_flags & ~(MOB_FLAG_MOVEMENT_UPDATE));
	if (mob->update_flags) {
		codec_put_bits(codec, 1, 1); // We want to update this player
		if (mob->update_flags & MOB_FLAG_REGION_UPDATE) { // We need to load a new region
			location_t* location = &mob->pos;
			codec_put_bits(codec, 2, 3); // Update type 3 = warp to location
			codec_put_bits(codec, 2, location->z);
			codec_put_bits(codec, 1, 1); // discard walking queue
			codec_put_bits(codec, 1, (other_update_flags ? 1 : 0)); // Signals that this player will have an entry in the update block
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
}

/**
 * build_update_block
 *
 * Builds a player update block for a given client
 *  - game_client: The client
 *  - codec: The codec to write to
 */
void build_update_block(game_client_t* game_client, stream_codec_t* codec)
{
	mob_t* mob = &game_client->mob;
	uint16_t flags = translate_update_flags(mob->update_flags);
	if (!flags) {
		return;
	}
	if (flags >= 256) {
		flags |= 64;
		codec_put16f(codec, flags, CODEC_LITTLE);
	} else {
		codec_put8(codec, flags);
	}

	if (flags & CL_FLAG_APPEARANCE_UPDATE) {
		stream_codec_t* appearance_block = build_appearance_block(game_client);
		codec_put8(codec, -codec_len(appearance_block));
		codec_concat(codec, appearance_block);
		codec_free(appearance_block);
	}
}

/**
 * build_appearance_block
 *
 * Builds an appearance update block for a given player
 *  - game_client: The client
 * returns: A codec containing the appearance block
 */
stream_codec_t* build_appearance_block(game_client_t* game_client)
{
	stream_codec_t* appearance_block = codec_create(NULL);

	codec_put8(appearance_block, 0); // gender
	codec_put8(appearance_block, 0);
	codec_put8(appearance_block, 0); // head
	codec_put8(appearance_block, 0); // cape
	codec_put8(appearance_block, 0); // neck
	codec_put8(appearance_block, 0); // right hand
	codec_put16(appearance_block, 256 + 19); // torso
	codec_put8(appearance_block, 0); // left hand
	codec_put16(appearance_block, 256 + 29); // arms
	codec_put16(appearance_block, 256 + 39); // legs
	codec_put16(appearance_block, 256 + 3); // head
	codec_put16(appearance_block, 256 + 35); // hands
	codec_put16(appearance_block, 256 + 44); // feet
	codec_put16(appearance_block, 256 + 10); // beard
	codec_put8(appearance_block, 7); // hair color
	codec_put8(appearance_block, 8); // torso color
	codec_put8(appearance_block, 9); // leg color
	codec_put8(appearance_block, 5); // feet color
	codec_put8(appearance_block, 0); // skin color

	/* animations */
	codec_put16(appearance_block, 0x328);
	codec_put16(appearance_block, 0x337);
	codec_put16(appearance_block, 0x333);
	codec_put16(appearance_block, 0x334);
	codec_put16(appearance_block, 0x335);
	codec_put16(appearance_block, 0x336);
	codec_put16(appearance_block, 0x338);

	codec_put64(appearance_block, game_client->name_hash); // username hash
	codec_put8(appearance_block, 3); // combat level
	codec_put16(appearance_block, 0);
	return appearance_block;
}

/**
 * packet_build_player_update
 *
 * Builds the periodic player update block for a given player
 *  - game_client: The client
 * returns: The completed packet
 */
packet_t* packet_build_player_update(game_client_t* game_client)
{
	packet_t* player_update = packet_create(packet_lookup(PACKET_TYPE_OUT, PKT_CL_PLAYER_UPDATE));
	stream_codec_t* update_block = codec_create(NULL);
	codec_set_bit_access_mode(&player_update->payload, true);

	build_movement_block(game_client, &player_update->payload);
	build_update_block(game_client, update_block);

	codec_put_bits(&player_update->payload, 8, 0); // The number of other players to update
	codec_put_bits(&player_update->payload, 11, 2047); // Signals end of movement updates

	codec_set_bit_access_mode(&player_update->payload, false);
	codec_concat(&player_update->payload, update_block);
	codec_free(update_block);
	return player_update;
}
