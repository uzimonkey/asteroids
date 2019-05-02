CC=clang
CFLAGS=-Wall -std=c11 -pedantic
LDFLAGS=
LIBS=

SRC=$(wildcard *.c)
HEAD=$(wildcard *.h)


ifeq ($(MAKECMDGOALS),debug)
PROFILE=debug
endif

ifeq ($(MAKECMDGOALS),)
PROFILE=debug
endif

ifeq ($(MAKECMDGOALS),release)
PROFILE=release
endif


ifeq ($(PROFILE),debug)
CFLAGS+=-DDEBUG -g
endif

ifeq ($(PROFILE),release)
CFLAGS+=-O3
endif


OBJ:=$(SRC:%.c=$(PROFILE)/%.o)

release: asteroids
debug: asteroids
asteroids: $(OBJ) FORCE
	$(CC) $(LDFLAGS) $(OBJ) $(LIBS) -o asteroids

FORCE:

.phony: run
run: debug
	./asteroids

$(PROFILE)/%.o: %.c $(HEAD)
	@mkdir -p $(PROFILE)
	$(CC) $(CFLAGS) $< -c -o $@

.phony: clean
clean:
	rm -rf debug/ release/ asteroids

.SECONDEXPANSION:
