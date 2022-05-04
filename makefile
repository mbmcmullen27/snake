PY_CFLAGS = $(shell python3-config --cflags --embed)
PY_LDFLAGS = $(shell python3-config --cflags --embed --ldflags) 
DEBUG_FLAG = $(shell [[ ! -z "${DEBUG}" ]] && echo -n "-DDEBUG")
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c,dist/%.o, $(SOURCES))
SHELL = bash
CC = clang

snake: $(OBJECTS)
	$(CC) $(DEBUG_FLAG) -lncurses $(OBJECTS) $(PY_LDFLAGS) -o snake
dist/snake.o: src/snake.c src/snake.h src/position.h
	$(CC) $(DEBUG_FLAG) -c src/snake.c -o dist/snake.o
dist/position.o: src/position.c src/position.h
	$(CC) -c src/position.c -o dist/position.o
dist/game.o: src/game.c src/game.h src/snake.h src/pyClient.h
	$(CC) $(DEBUG_FLAG) $(PY_CFLAGS) -c src/game.c -o dist/game.o
dist/pyClient.o: src/pyClient.c src/pyClient.h
	$(CC) $(PY_CFLAGS) -c src/pyClient.c -o dist/pyClient.o
dist/main.o: src/main.c src/main.h src/game.h
	$(CC) $(DEBUG_FLAG) $(PY_CFLAGS) -c src/main.c -o dist/main.o

dist/manifest.o: tests/manifest.c tests/manifest.h
	$(CC) -c tests/manifest.c -o dist/manifest.o
test: dist/test.o
	$(CC) dist/test.o dist/manifest.o -o test
dist/test.o: tests/test.c dist/position.o dist/manifest.o
	$(CC) -c tests/test.c -o dist/test.o

$(shell mkdir dist 2>/dev/null)

clean:
	rm snake 
	$(shell rm test 2>/dev/null)
	rm -r dist

