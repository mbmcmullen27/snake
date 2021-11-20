snake: snake.o position.o
	clang -o snake -lncurses snake.o position.o 
snake.o: snake.c snake.h position.h
	clang -c snake.c -o snake.o
position.o: position.c position.h
	clang -c position.c -o position.o 

test: test.o
	clang test.o -o test
test.o: test.c position.h
	clang -c test.c -o test.o

clean:
	rm *.o snake test