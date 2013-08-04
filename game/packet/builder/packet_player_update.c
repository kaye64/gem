/**
 * packet_player_update.c
 *
 * Defines the player update packet builder
 */
#include <game/packet/builder/packet_player_update.h>

#include <util/jstring_encode.h>

#define CL_FLAG_APPEARANCE_UPDATE (1 << 4)
#define CL_FLAG_CHAT_UPDATE (1 << 7)

static codec_t* build_appearance_block(player_t* player);

/**
 * Translates our server sided flags into the format the client wants
 */
static uint16_t translate_update_flags(uint16_t flags)
{
	uint16_t new_flags = 0;
	if (flags & MOB_FLAG_APPEARANCE_UPDATE) {
		new_flags |= CL_FLAG_APPEARANCE_UPDATE;
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

	if (update_flags & CL_FLAG_APPEARANCE_UPDATE) {
		codec_t* appearance_block = build_appearance_block(player);
		codec_put8f(codec, codec_len(appearance_block), CODEC_NEGATIVE);
		codec_concat(codec, appearance_block);
		object_free(appearance_block);
	}
}

/**
 * Builds an appearance update block for a given player
 */
static codec_t* build_appearance_block(player_t* player)
{
	codec_t* appearance_block = object_new(codec);

	codec_put8(appearance_block, 0); // gender
	codec_put8(appearance_block, 0); // head icon

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

	/**
	 * These colors have no relation to any other color indices.
	 * This value is just translated to an actual color in a lookup table,
	 * so there are a set number of colors for each body part.
	 */
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

	codec_put64(appearance_block, jstring_encode(player->username));
	codec_put8(appearance_block, 3); // combat level
	codec_put16(appearance_block, 0);
	return appearance_block;
}

/**
 * Adds a new player to the update block
 */
static void add_new_player(player_t* this_player, player_t* new_player, codec_t* main_codec, codec_t* update_codec)
{
	uint16_t update_flags = translate_update_flags(new_player->mob.update_flags | MOB_FLAG_APPEARANCE_UPDATE); // force an appearence update for new players
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

	entity_tracker_t* tracker = &player->known_players;
	list_t* known_players = &tracker->entities;
	list_t* new_players = &tracker->in_entities;
	
	uint8_t num_players = list_count(known_players);

	codec_put_bits(main_block, 8, num_players); // The number of other players to update

	/* update all the observing players */
	list_node_t* node_iter = list_front(known_players);
	entity_list_node_t* node = NULL;
	while (node_iter != NULL) {
		node = container_of(node_iter, entity_list_node_t, node);
		node_iter = node_iter->next;

		if (entity_tracker_is_removing(tracker, node->entity)) {
			codec_put_bits(main_block, 1, 1);
			codec_put_bits(main_block, 2, 3);
		} else {
			player_t* this_player = player_for_entity(node->entity);
			uint16_t update_flags = translate_update_flags(this_player->mob.update_flags);
			build_movement_block(this_player, main_block, this_player->mob.update_flags & ~(MOB_FLAG_REGION_UPDATE));
			build_update_block(this_player, update_block, update_flags);
		}
	}

	/* add new players */
	node_iter = list_front(new_players);
	node = NULL;
	while (node_iter != NULL) {
		node = container_of(node_iter, entity_list_node_t, node);
		node_iter = node_iter->next;
		
		player_t* this_player = player_for_entity(node->entity);
		add_new_player(player, this_player, main_block, update_block);
	}

	if (codec_len(update_block) > 0) {
		codec_put_bits(main_block, 11, 2047); // Signals end of movement updates
		codec_set_bit_access_mode(main_block, false);
		codec_concat(main_block, update_block);
	} else {
		codec_set_bit_access_mode(main_block, false);
	}
	object_free(update_block);
	return player_update;
}
