snake: snake.o position.o pyClient.o
	clang -lncurses snake.o position.o $(python3-config --embed --ldflags) -o snake
snake.o: snake.c snake.h position.h
	clang -c snake.c -o snake.o
position.o: position.c position.h
	clang -c position.c -o position.o
pyClient.o: pyClient.c pyClient.h
	clang $(python3-config --cflags --embed) -c pyClient.c -o pyClient.o

test: test.o
	clang test.o -o test
test.o: test.c position.h
	clang -c test.c -o test.o

clean:
	rm *.o snake test