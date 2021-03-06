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

from gem.api import Log

LOG_TAG = "interface"

def interface_button(player, button):
    Log.Debug(LOG_TAG, "clicked button " + str(button))
    if button == 2458: # Why does Python not have a switch statement? :-(
        player.force_logout()
    elif button == 152:
        player.running = False
    elif button == 153:
        player.running = True
