#ifndef snake_h
#define snake_h

#include <unistd.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "position.h"

typedef struct {
    Position* head;
    Position* tail;
    Corner* top;
    Corner* bottom;
} Snake;

void initSnake(Snake* snake, int my);
Position* bottomPos(Snake* snake);
void startGame();
void gameOver();
void freeCorners(Snake* snake);
void printDebug(Snake* snake, int mx, int my);
void pushCorner(Snake* snake);
void popCorner(Snake* snake);

#endif