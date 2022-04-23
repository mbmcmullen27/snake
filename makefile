snake: dist/snake.o dist/position.o dist/game.o dist/pyClient.o
	clang -lncurses dist/snake.o dist/position.o dist/game.o dist/pyClient.o $(shell python3-config --cflags --embed --ldflags) -o snake
dist/snake.o: src/snake.c src/snake.h src/position.h
	clang -c src/snake.c -o dist/snake.o
dist/position.o: src/position.c src/position.h
	clang -c src/position.c -o dist/position.o
dist/game.o: src/game.c src/game.h src/snake.h src/pyClient.h
	clang $(shell python3-config --cflags --embed) -c src/game.c -o dist/game.o
dist/pyClient.o: src/pyClient.c src/pyClient.h
	clang $(shell python3-config --cflags --embed) -c src/pyClient.c -o dist/pyClient.o

test: dist/test.o
	clang dist/test.o -o test
dist/test.o: src/test.c src/position.h
	clang -c src/test.c -o dist/test.o

$(shell mkdir dist 2>/dev/null)

clean:
	rm snake 
	$(shell rm test 2>/dev/null)
	rm -r dist

