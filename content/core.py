import gem
from gem import log

def startup():
    gem.log.info("core", "Startup")

def shutdown():
    gem.log.info("core", "Shutting down")
