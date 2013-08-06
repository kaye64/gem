/**
 * packet_login_window.c
 *
 * Defines the login window packet builder
 */
#include <game/packet/builder/packet_login_window.h>

/**
 * Constructs a login window packet
 */
packet_t* packet_build_login_window(player_t* player)
{
	packet_t* login_window = packet_create(packet_lookup(PACKET_TYPE_OUT, PKT_CL_LOGIN_WINDOW));
	codec_put8f(&login_window->payload, 0, CODEC_NEGATIVE); // days since recovery changed
	codec_put16f(&login_window->payload, 0, CODEC_OFS128); // num messages
	codec_put8(&login_window->payload, 0); // membership warning (??)
	codec_put32f(&login_window->payload, 0x7F000001, CODEC_MIXED_A); // last ip (127.0.0.1)
	codec_put16(&login_window->payload, 0); // last login
	return login_window;
}
