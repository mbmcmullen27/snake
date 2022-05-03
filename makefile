PY_CFLAGS = $(shell python3-config --cflags --embed)
PY_LDFLAGS = $(shell python3-config --cflags --embed --ldflags) 
DEBUG_FLAG = $(shell [[ ! -z "${DEBUG}" ]] && echo -n "-DDEBUG")
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c,dist/%.o, $(SOURCES))

snake: $(OBJECTS)
	clang $(DEBUG_FLAG) -lncurses $(OBJECTS) $(PY_LDFLAGS) -o snake
dist/snake.o: src/snake.c src/snake.h src/position.h
	clang $(DEBUG_FLAG) -c src/snake.c -o dist/snake.o
dist/position.o: src/position.c src/position.h
	clang -c src/position.c -o dist/position.o
dist/game.o: src/game.c src/game.h src/snake.h src/pyClient.h
	clang $(DEBUG_FLAG) $(PY_CFLAGS) -c src/game.c -o dist/game.o
dist/pyClient.o: src/pyClient.c src/pyClient.h
	clang $(PY_CFLAGS) -c src/pyClient.c -o dist/pyClient.o
dist/main.o: src/main.c src/main.h src/game.h
	clang $(DEBUG_FLAG) $(PY_CFLAGS) -c src/main.c -o dist/main.o

dist/manifest.o: tests/manifest.c tests/manifest.h
	clang -c tests/manifest.c -o dist/manifest.o
test: dist/test.o
	clang dist/test.o dist/manifest.o -o test
dist/test.o: tests/test.c dist/position.o dist/manifest.o
	clang -c tests/test.c -o dist/test.o

$(shell mkdir dist 2>/dev/null)

clean:
	rm snake 
	$(shell rm test 2>/dev/null)
	rm -r dist

