/**
 * packet_button_click.c
 *
 * Defines the packet routine for the Idle packet
 */
#include <game/packet/routine/packet_button_click.h>

#include <script/hook.h>
#include <script/api/player.h>
#include <util/log.h>

#define LOG_TAG "packet_button_click"

/**
 * Acts on the player button click packet
 */
void packet_button_click(player_t* player, packet_t* packet)
{
	int button_id = codec_get16(&packet->payload);
	button_click_args_t args = { .player = player, .button = button_id };
	hook_notify(SCRIPT_HOOK_BUTTON_CLICK, (void*)&args);
}
