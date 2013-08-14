import gem
from gem import log

def player_login(player):
    loc = player.location;
    gem.log.info("session", "Player login: (index " + str(player.index) + ")")
    gem.log.info("session", "location: " + str(loc.abs_x) + ", " + str(loc.abs_y))
