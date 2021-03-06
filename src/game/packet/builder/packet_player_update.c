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
 * packet_player_update.c
 *
 * Defines the player update packet builder
 */
#include <game/packet/builder/packet_player_update.h>

#include <util/jstring_encode.h>

#define CL_FLAG_IDENTITY_UPDATE (1 << 4)
#define CL_FLAG_CHAT_UPDATE (1 << 7)

static codec_t* build_identity_block(player_t* player);

/**
 * Translates our server sided flags into the format the client wants
 */
static uint16_t translate_update_flags(uint16_t flags)
{
	uint16_t new_flags = 0;
	if (flags & MOB_FLAG_IDENTITY_UPDATE) {
		new_flags |= CL_FLAG_IDENTITY_UPDATE;
	}
	if (flags & MOB_FLAG_CHAT_UPDATE) {
		new_flags |= CL_FLAG_CHAT_UPDATE;
	}
	return new_flags;
}

/**
 * Builds a movement update block for a given client. Assumes that
 * the given codec is already in bit access mode
 *  - codec: The codec to write to
 */
static void build_movement_block(player_t* player, codec_t* codec, uint16_t update_flags)
{
	mob_t* mob = &player->mob;
	uint16_t other_update_flags = (update_flags & ~(MOB_FLAG_MOVEMENT_UPDATE));
	if (update_flags) {
		codec_put_bits(codec, 1, 1); // We want to update this player
		if (update_flags & MOB_FLAG_REGION_UPDATE) { // We need to load a new region
			location_t location = mob_position(mob);
			region_local_t local = local_coord(location, mob->region);
			bool discard_walk_queue = waypoint_queue_empty(&mob->waypoint_queue);
			codec_put_bits(codec, 2, 3); // Update type 3 = warp to location
			codec_put_bits(codec, 2, local.z);
			codec_put_bits(codec, 1, (discard_walk_queue ? 1 : 0)); // discard walking queue
			codec_put_bits(codec, 1, (other_update_flags ? 1 : 0)); // Signals that this player will have an entry in the update block
			codec_put_bits(codec, 7, local.y);
			codec_put_bits(codec, 7, local.x);
		} else if (update_flags & MOB_FLAG_RUN_UPDATE) {
			codec_put_bits(codec, 2, 2); // Update type 2 = running
			codec_put_bits(codec, 3, mob->last_direction);
			codec_put_bits(codec, 3, mob->direction);
			codec_put_bits(codec, 1, (other_update_flags ? 1 : 0)); // Signals that this player will have an entry in the update block
		} else if (update_flags & MOB_FLAG_WALK_UPDATE) {
			codec_put_bits(codec, 2, 1); // Update type 1 = walk
			codec_put_bits(codec, 3, mob->direction);
			codec_put_bits(codec, 1, (other_update_flags ? 1 : 0)); // Signals that this player will have an entry in the update block
		} else {
			codec_put_bits(codec, 2, 0); // Update type 0 = No movement update
		}
	} else {
		codec_put_bits(codec, 1, 0); // No updates
	}
}

/**
 * Builds a player update block for a given client
 *  - codec: The codec to write to
 */
static void build_update_block(player_t* player, codec_t* codec, uint16_t update_flags)
{
	mob_t* mob = &player->mob;
	if (!update_flags) {
		return;
	}
	if (update_flags >= 256) {
		update_flags |= 64;
		codec_put16f(codec, update_flags, CODEC_LITTLE);
	} else {
		codec_put8(codec, update_flags);
	}

	if (update_flags & CL_FLAG_CHAT_UPDATE) {
		chat_message_t* message = mob->chat_message;
		codec_put16f(codec, ((message->color & 0xFF) << 8) + (message->effects & 0xFF), CODEC_LITTLE);
		codec_put8(codec, 0);
		codec_put8f(codec, message->length, CODEC_NEGATIVE);
		for (int i = message->length-1; i >= 0; i--) {
			codec_put8(codec, message->message[i]);
		}
	}

	if (update_flags & CL_FLAG_IDENTITY_UPDATE) {
		codec_t* identity_block = build_identity_block(player);
		codec_put8f(codec, codec_len(identity_block), CODEC_NEGATIVE);
		codec_concat(codec, identity_block);
		object_free(identity_block);
	}
}

/**
 * Builds an appearance update block for a given player
 */
static codec_t* build_identity_block(player_t* player)
{
	codec_t* identity_block = object_new(codec);

	identity_t* iden = &player->identity;

	codec_put8(identity_block, iden->appearance.gender); // gender
	codec_put8(identity_block, iden->appearance.head_icon); // head icon

	/**
	 * If a value here is offset by 512, the value is treated
	 * as an item id, and the item is displayed on the player.
	 *
	 * If a value is offset by 256, the value is treated as
	 * an index from the identity kit (idk.dat).
	 *
	 * If the first value is == 65535, we can send 2 bytes indicating
	 * an npc to use as the player's appearance. The rest of this block
	 * is discarded.
	 *
	 * An actual value is only sent as 2 bytes. If anything other than 0
	 * is sent as a single byte, the client will probably crash.
	 */

	codec_put8(identity_block, 0); // helm
	codec_put8(identity_block, 0); // cape
	codec_put8(identity_block, 0); // amulet
	codec_put8(identity_block, 0); // right hand equip
	codec_put16(identity_block, 256 + iden->appearance.model_torso); // torso
	codec_put8(identity_block, 0); // left hand equip
	codec_put16(identity_block, 256 + iden->appearance.model_arms); // arms
	codec_put16(identity_block, 256 + iden->appearance.model_legs); // legs
	codec_put16(identity_block, 256 + iden->appearance.model_head); // head
	codec_put16(identity_block, 256 + iden->appearance.model_hands); // hands
	codec_put16(identity_block, 256 + iden->appearance.model_feet); // feet
	codec_put16(identity_block, 256 + iden->appearance.model_beard); // beard

	/**
	 * These colors have no relation to any other color indices.
	 * This value is just translated to an actual color in a lookup table,
	 * so there are a set number of colors for each body part.
	 */
	codec_put8(identity_block, iden->appearance.color_hair); // hair color
	codec_put8(identity_block, iden->appearance.color_torso); // torso color
	codec_put8(identity_block, iden->appearance.color_leg); // leg color
	codec_put8(identity_block, iden->appearance.color_feet); // feet color
	codec_put8(identity_block, iden->appearance.color_skin); // skin color

	/* animations */
	codec_put16(identity_block, iden->anim_idle);
	codec_put16(identity_block, iden->anim_spot_rotate);
	codec_put16(identity_block, iden->anim_walk);
	codec_put16(identity_block, iden->anim_rotate_180);
	codec_put16(identity_block, iden->anim_rotate_ccw);
	codec_put16(identity_block, iden->anim_rotate_cw);
	codec_put16(identity_block, iden->anim_run);

	codec_put64(identity_block, jstring_encode(player->username));
	codec_put8(identity_block, iden->combat_level);
	codec_put16(identity_block, 0); // skill level (for minigames)
	return identity_block;
}

/**
 * Adds a new player to the update block
 */
static void add_new_player(player_t* this_player, player_t* new_player, codec_t* main_codec, codec_t* update_codec)
{
	uint16_t update_flags = translate_update_flags(new_player->mob.update_flags | MOB_FLAG_IDENTITY_UPDATE); // force an appearence update for new players
	codec_put_bits(main_codec, 11, new_player->mob.entity.index);
	codec_put_bits(main_codec, 1, 1); // append update block
	build_update_block(new_player, update_codec, update_flags);
	codec_put_bits(main_codec, 1, 1); // discard walking queue

	location_t our_location = mob_position(mob_for_player(this_player));
	location_t other_location = mob_position(mob_for_player(new_player));
	int delta_x = other_location.x - our_location.x;
	int delta_y = other_location.y - our_location.y;
	if(delta_x < 0) {
		delta_x += 32;
	}
	if(delta_y < 0) {
		delta_y += 32;
	}
	codec_put_bits(main_codec, 5, delta_y);
	codec_put_bits(main_codec, 5, delta_x);
}

/**
 * Constructs a periodic player update block for a given player
 */
packet_t* packet_build_player_update(player_t* player)
{
	packet_t* player_update = packet_create(packet_lookup(PACKET_TYPE_OUT, PKT_CL_PLAYER_UPDATE));
	codec_t* main_block = &player_update->payload;
	codec_t* update_block = object_new(codec);
	codec_set_bit_access_mode(main_block, true);

	uint16_t self_update_flags = translate_update_flags(player->mob.update_flags & ~(MOB_FLAG_CHAT_UPDATE));
	build_movement_block(player, main_block, player->mob.update_flags & ~(MOB_FLAG_CHAT_UPDATE));
	build_update_block(player, update_block, self_update_flags);

	entity_tracker_t* tracker = &player->state.known_players;
	list_t* observed_players = &tracker->entities;

	uint8_t num_players = entity_tracker_count_known(tracker);

	codec_put_bits(main_block, 8, num_players); // The number of other players to update

	/* update all the observing players */
	list_node_t* node_iter = list_front(observed_players);
	tracked_entity_t* tracked_entity = NULL;
	while (node_iter != NULL) {
		tracked_entity = container_of(node_iter, tracked_entity_t, node);
		node_iter = node_iter->next;

		if (tracked_entity_is_removing(tracked_entity)) {
			codec_put_bits(main_block, 1, 1);
			codec_put_bits(main_block, 2, 3);
		} else if (!tracked_entity_is_adding(tracked_entity)) {
			player_t* this_player = player_for_entity(tracked_entity->entity);
			uint16_t update_flags = translate_update_flags(this_player->mob.update_flags);
			build_movement_block(this_player, main_block, this_player->mob.update_flags & ~(MOB_FLAG_REGION_UPDATE));
			build_update_block(this_player, update_block, update_flags);
		}
	}

	/* add new players */
	node_iter = list_front(observed_players);
	tracked_entity = NULL;
	while (node_iter != NULL) {
		tracked_entity = container_of(node_iter, tracked_entity_t, node);
		node_iter = node_iter->next;

		if (tracked_entity_is_adding(tracked_entity)) {
			player_t* this_player = player_for_entity(tracked_entity->entity);
			add_new_player(player, this_player, main_block, update_block);
		}
	}

	if (codec_len(update_block) > 0) {
		codec_put_bits(main_block, 11, 2047); // Signals no more players to add
		codec_set_bit_access_mode(main_block, false);
		codec_concat(main_block, update_block);
	} else {
		codec_set_bit_access_mode(main_block, false);
	}
	object_free(update_block);
	return player_update;
}
