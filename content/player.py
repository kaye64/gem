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

from gem.api import Location
import session

from enum import Enum

LOG_TAG = "player"

class Rights(Enum):
    PLAYER = 0
    MODERATOR = 1
    ADMIN = 2
    SUPERADMIN = 3

class Appearance(object):
    # Default player appearance
    gender = 0
    head_icon = 0
    model_head = 0
    model_head2 = 3
    model_cape = 0
    model_neck = 0
    model_equip_left = 0
    model_equip_right = 0
    model_torso = 19
    model_arms = 29
    model_legs = 39
    model_hands = 35
    model_feet = 44
    model_beard = 10
    color_hair = 7
    color_torso = 8
    color_leg = 9
    color_feet = 5
    color_skin = 0

    def __init__(self):
        pass

class Profile(object):
    username = ""
    password = ""
    rights = Rights.PLAYER
    location = Location.Absolute(3200, 3200, 0)
    appearance = Appearance()

    def __init__(self, username):
        self.username = username

def player_position_update(player, location, warped):
    profile = player.profile
    profile.location = location
