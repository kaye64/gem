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
