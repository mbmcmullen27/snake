#include <curses.h>

#include "pyClient.h"
#include "snake.h"

void startGame();
void collect();
void gameOver();
bool check();
void printDir(int, char*, int, int);
void printPos(char*, Position*, int, int);