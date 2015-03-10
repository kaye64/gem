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

import gem.Log

LOG_TAG = "hook"
hooks = {}

def register(hook, callback):
    if hook in hooks:
        if isinstance(hooks[hook], list):
            hooks[hook] = hooks[hook] + [callback]
        else:
            gem.Log.Error(LOG_TAG, "Hooks for ({}) is not a list. Tried to register nonexclusive hook to exclusive slot?")
    else:
        hooks[hook] = [callback]

def register_exclusive(hook, callback):
    hooks[hook] = callback

def dispatch(hook, args):
    if hook in hooks:
        if isinstance(hooks[hook], list):
            for hook_func in hooks[hook]:
                hook_func(*args)
        else:
            gem.Log.Error(LOG_TAG, "Tried to call nonexclusive hook on singly registered slot")

def dispatch_exclusive(hook, args):
    if hook in hooks:
        if isinstance(hooks[hook], list):
            gem.Log.Error(LOG_TAG, "Tried to call exclusive hook on multiply registered slot")
        else:
            return hooks[hook](*args)
