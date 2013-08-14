import gem
from gem import log

def player_login(player):
    gem.log.info("core", "Player login: (index " + str(player.index) + ")")
