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

import gem
import player

LOG_TAG = "session"

tab_interfaces = [ 2423, 3917, 638, 3213, 1644, 5608, 1151, -1, 5065, 5715, 2449, 4445, 147, 6299 ]
players = {}

def player_authenticate(p):
    profile = player.Profile(p.username)
    return profile

def player_load_profile(p):
    profile = p.get_profile()

    p.rights = profile.rights.value # todo: smarter enum assigns
    p.warp_to(profile.location)

    return True

def player_login(player):
    players[player.index] = player
    gem.log.info(LOG_TAG, "Player login: " + player.username + " (index " + str(player.index) + ")")
    gem.log.info(LOG_TAG, "{} players online".format(len(players)));

    player.send_message("Welcome to Gielinor")
    # Set the player's tab interfaces
    for tab, interface in enumerate(tab_interfaces):
        player.set_tab_interface(tab, interface)

def player_logout(player):
    del players[player.index]
    gem.log.info(LOG_TAG, "Player logout: " + player.username + " (index " + str(player.index) + ")")
