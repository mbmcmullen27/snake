snake: snake.o
	clang snake.o -o snake -lncurses
snake.o: snake.c snake.h position.h
	clang -c snake.c -o snake.o

test: test.o
	clang test.o -o test
test.o: test.c position.h
	clang -c test.c -o test.o

clean:
	rm *.o snake test