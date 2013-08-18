SUBDIRS = init util net crypto jaggrab script script/api game game/packet game/packet/routine game/packet/builder

include $(addsuffix /makefile.mk, $(SUBDIRS))
