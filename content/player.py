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
import swig.Location
import session

from enum import Enum

LOG_TAG = "player"

class Rights(Enum):
    PLAYER = 0
    MODERATOR = 1
    ADMIN = 2
    SUPERADMIN = 3

class Profile(object):
    def __init__(self, username):
        self.username = username
        self.password = ""
        self.rights = Rights.SUPERADMIN
        self.location = swig.Location.Absolute(3200, 3200, 0)

    def position_update(self, location, warped):
        self.location = location

def player_position_update(player, location, warped):
    profile = player.profile
    profile.position_update(location, warped)
