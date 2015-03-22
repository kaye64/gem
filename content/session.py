#  This file is part of Gem.
#
#  Gem is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Gem is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Gem.  If not, see <http://www.gnu.org/licenses/\>.

from gem.api import Entity
from gem.api import Log
from gem.api import Item
import gem.profile
import player

LOG_TAG = "session"

tab_interfaces = [ 2423, 3917, 638, 3213, 1644, 5608, 1151, -1, 5065, 5715, 2449, 4445, 147, 6299 ]
players = {}

def player_authenticate(p):
    profile = gem.profile.Profile(p.username)
    return profile

def player_load_profile(p):
    profile = p.profile

    p.rights = profile.rights.value # todo: smarter enum assigns
    p.warp_to(profile.location)

    # load appearance
    p.identity.username = profile.username

    p.identity.anim_idle = 0x328
    p.identity.anim_spot_rotate = 0x337
    p.identity.anim_walk = 0x333
    p.identity.anim_rotate_180 = 0x334
    p.identity.anim_rotate_ccw = 0x335
    p.identity.anim_rotate_cw = 0x336
    p.identity.anim_run = 0x338

    p.identity.appearance.gender = profile.appearance.gender
    p.identity.appearance.head_icon = profile.appearance.head_icon
    p.identity.appearance.model_torso = profile.appearance.model_torso
    p.identity.appearance.model_arms = profile.appearance.model_arms
    p.identity.appearance.model_legs = profile.appearance.model_legs
    p.identity.appearance.model_head = profile.appearance.model_head
    p.identity.appearance.model_hands = profile.appearance.model_hands
    p.identity.appearance.model_feet = profile.appearance.model_feet
    p.identity.appearance.model_beard = profile.appearance.model_beard
    p.identity.appearance.color_hair = profile.appearance.color_hair
    p.identity.appearance.color_torso = profile.appearance.color_torso
    p.identity.appearance.color_leg = profile.appearance.color_leg
    p.identity.appearance.color_feet = profile.appearance.color_feet
    p.identity.appearance.color_skin = profile.appearance.color_skin
    p.flush_identity()

    return True

def player_login(player):
    players[player.index] = player
    Log.Session(LOG_TAG, "Player login: " + player.username + " (index " + str(player.index) + ")")
    Log.Info(LOG_TAG, "{} players online".format(len(players)))

    player.send_message("Welcome to Gielinor")
    # Set the player's tab interfaces
    for tab, interface in enumerate(tab_interfaces):
        player.set_tab_interface(tab, interface)

def player_logout(player):
    del players[player.index]
    Log.Session(LOG_TAG, "Player logout: " + player.username + " (index " + str(player.index) + ")")
