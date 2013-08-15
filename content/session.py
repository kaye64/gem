import gem
from gem import log

tab_interfaces = [ 2423, 3917, 638, 3213, 1644, 5608, 1151, -1, 5065, 5715, 2449, 4445, 147, 6299 ]

def player_login(player):
    gem.log.info("session", "Player login: " + player.username + " (index " + str(player.index) + ")")
    player.send_message("Welcome to Gielinor")
    # Set the player's tab interfaces
    for tab, interface in enumerate(tab_interfaces):
        player.set_tab_interface(tab, interface)

