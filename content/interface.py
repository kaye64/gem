import gem

def interface_button(player, button):
    gem.log.debug("interface", "clicked button " + str(button))
    if button == 2458: # Why does Python not have a switch statement? :-(
        player.logout()
    elif button == 152:
        player.set_running(False)
    elif button == 153:
        player.set_running(True)
