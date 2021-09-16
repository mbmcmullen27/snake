snake: snake.o
	clang snake.o -o snake -lncurses
snake.o: snake.c snake.h position.h
	clang -c snake.c -o snake.o
clean:
	rm *.o snake