import gem
from gem import log

def player_login(player):
    loc = player.location;
    gem.log.info("session", "Player login: (index " + str(player.index) + ")")

