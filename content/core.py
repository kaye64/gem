import gem
from gem import log
import session

def register_hooks():
    gem.log.info("core", "Registering API hooks..")
    gem.register_hook(gem.HOOK_STARTUP, startup)
    gem.register_hook(gem.HOOK_SHUTDOWN, shutdown)
    gem.register_hook(gem.HOOK_PLAYER_LOGIN, session.player_login)

def startup():
    pass

def shutdown():
    pass
