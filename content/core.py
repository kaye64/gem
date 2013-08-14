import gem
from gem import log
import session

def startup():
    gem.log.info("core", "Registering API hooks..")
    gem.register_hook(gem.HOOK_PLAYER_LOGIN, session.player_login)

def shutdown():
    gem.log.info("core", "Shutting down")
