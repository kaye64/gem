import gem

def interface_button(player, button):
    gem.log.debug("interface", "clicked button " + str(button))
    if button == 2458: # Why does Python not have a switch statement? :-(
        player.logout()
