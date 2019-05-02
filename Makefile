CC=clang
CFLAGS=-Wall -std=c11 -pedantic -Iheaders
LDFLAGS=
LIBS=

SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o)
HEAD=$(wildcard *.h)

asteroids: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LIBS) -o $@

.phony: run
run: asteroids
	./asteroids

%.o: %.c $(HEAD)
	$(CC) $(CFLAGS) $< -c -o $@

.phony: clean
clean:
	rm -rf *.o asteroids
