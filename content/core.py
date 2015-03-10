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

import swig.Log
import swig.Hook
import session
import player
import interface
import hook

LOG_TAG = "core"

def content_init():
    swig.Log.Info(LOG_TAG, "Registering API hooks..")
    hook.register(swig.Hook.STARTUP, startup)
    hook.register(swig.Hook.SHUTDOWN, shutdown)
    hook.register_exclusive(swig.Hook.PLAYER_AUTHENTICATE, session.player_authenticate)
    hook.register_exclusive(swig.Hook.PLAYER_LOAD, session.player_load_profile)
    hook.register(swig.Hook.PLAYER_LOGIN, session.player_login)
    hook.register(swig.Hook.PLAYER_LOGOUT, session.player_logout)
    hook.register(swig.Hook.PLAYER_POSITION, player.player_position_update)
    hook.register(swig.Hook.BUTTON_CLICK, interface.interface_button)

def startup():
    pass

def shutdown():
    pass
