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
import session
import interface

def register_hooks():
    gem.log.info("core", "Registering API hooks..")
    gem.register_hook(gem.HOOK_STARTUP, startup)
    gem.register_hook(gem.HOOK_SHUTDOWN, shutdown)
    gem.register_hook(gem.HOOK_PLAYER_LOGIN, session.player_login)
    gem.register_hook(gem.HOOK_PLAYER_LOGOUT, session.player_logout)
    gem.register_hook(gem.HOOK_BUTTON_CLICK, interface.interface_button)

def startup():
    pass

def shutdown():
    pass
