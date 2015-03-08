CFLAGS = -g -std=gnu99 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Lrunite/
INCLUDE_DIRS = -Iinclude/ -Irunite/include/ $(shell python3-config --includes)
OUT = gem
LIB_DIRS = -L
LIBS = -lrunite -lev -lz -lpthread -lgmp $(shell python3-config --libs)
SUBDIRS = src/init src/util src/net src/crypto src/jaggrab src/script src/game
API_BINDING_OUT = content/swig

OBJECTS :=

include $(addsuffix /makefile.mk, $(SUBDIRS))

all: runite toolbelt $(OUT)

$(OUT): runite $(OBJECTS)
	gcc $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)

runite/librunite.a: runite

runite:
	make -C runite/

toolbelt:
	make -C toolbelt/

%.o: %.c
	gcc -c $(CFLAGS) $(LIBS) $(INCLUDE_DIRS) -o $@ $^

%_wrap.c: %.i
	swig -Wall $(INCLUDE_DIRS) -outdir $(API_BINDING_OUT) -python $^

clean:
	make -C runite/ clean
	make -C toolbelt/ clean
	-rm -f $(OUT) $(OBJECTS)

.PHONY: all clean runite toolbelt
