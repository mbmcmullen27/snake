snake: dist/main.o dist/snake.o dist/position.o dist/game.o dist/pyClient.o dist/manifest.o
	clang $(shell [[ ! -z "${DEBUG}" ]] && echo -n "-DDEBUG") -lncurses dist/main.o dist/snake.o dist/position.o dist/game.o dist/pyClient.o dist/manifest.o $(shell python3-config --cflags --embed --ldflags) -o snake
dist/snake.o: src/snake.c src/snake.h src/position.h
	clang $(shell [[ ! -z "${DEBUG}" ]] && echo -n "-DDEBUG") -c src/snake.c -o dist/snake.o
dist/position.o: src/position.c src/position.h
	clang -c src/position.c -o dist/position.o
dist/game.o: src/game.c src/game.h src/snake.h src/pyClient.h
	clang $(shell [[ ! -z "${DEBUG}" ]] && echo -n "-DDEBUG") $(shell python3-config --cflags --embed) -c src/game.c -o dist/game.o
dist/pyClient.o: src/pyClient.c src/pyClient.h
	clang $(shell python3-config --cflags --embed) -c src/pyClient.c -o dist/pyClient.o
dist/manifest.o: src/manifest.c src/manifest.h
	clang -c src/manifest.c -o dist/manifest.o
dist/main.o: src/main.c src/main.h src/game.h
	clang $(shell [[ ! -z "${DEBUG}" ]] && echo -n "-DDEBUG") $(shell python3-config --cflags --embed) -c src/main.c -o dist/main.o

test: dist/test.o
	clang dist/test.o dist/manifest.o -o test
dist/test.o: src/test.c dist/position.o dist/manifest.o
	clang -c src/test.c -o dist/test.o

$(shell mkdir dist 2>/dev/null)

clean:
	rm snake 
	$(shell rm test 2>/dev/null)
	rm -r dist

