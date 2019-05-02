CC=clang
CFLAGS=-Wall -std=c11 -pedantic -Iheaders
LDFLAGS=
LIBS=

HEADER_PATH=header/
OBJ_PATH=obj

# These are all source files that are needed to compile the program
# as well as all headers non-generated headers
SRC=\
		ecs.c\
		main.c\
		components.h

OBJ:=$(filter %.c,$(SRC))
OBJ:=$(patsubst %.c,%.o,$(OBJ))
OBJ:=$(addprefix $(OBJ_PATH)/,$(OBJ))

asteroids: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LIBS) -o $@

.phony: run
run: asteroids
	./asteroids

makeheaders: makeheaders.c
	$(CC) $^ -o $@

headers: $(SRC) makeheaders
	@mkdir -p headers
	./makeheaders $(SRC)
	mv $(patsubst %.c,%.h,$(filter %.c,$(SRC))) headers/

obj/%.o: %.c headers
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -c -o $@

.phony: clean
clean:
	rm -rf headers/ obj/ asteroids makeheaders
