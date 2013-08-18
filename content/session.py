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

tab_interfaces = [ 2423, 3917, 638, 3213, 1644, 5608, 1151, -1, 5065, 5715, 2449, 4445, 147, 6299 ]

def player_login(player):
    gem.log.info("session", "Player login: " + player.username + " (index " + str(player.index) + ")")
    player.send_message("Welcome to Gielinor")
    # Set the player's tab interfaces
    for tab, interface in enumerate(tab_interfaces):
        player.set_tab_interface(tab, interface)


def player_logout(player):
    gem.log.info("session", "Player logout: " + player.username + " (index " + str(player.index) + ")")
