snake: dist/snake.o dist/position.o dist/game.o
	clang -lncurses dist/snake.o dist/position.o dist/game.o -o snake
dist/snake.o: src/snake.c src/snake.h src/position.h
	clang -c src/snake.c -o dist/snake.o
dist/position.o: src/position.c src/position.h
	clang -c src/position.c -o dist/position.o
dist/game.o: src/game.c src/game.h src/snake.h
	clang -c src/game.c -o dist/game.o

test: dist/test.o
	clang dist/test.o -o test
dist/test.o: src/test.c src/position.h
	clang -c src/test.c -o dist/test.o

$(shell [ -d dist ] || mkdir dist)

clean:
	$(shell rm snake test 2>/dev/null)
	$(shell rm -r dist 2>/dev/null)

