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
} Snake;

void initSnake(Snake* snake, int my);
Position* bottomPos();
void startGame();
void gameOver();
void moveHead(Snake* snake, int dir);
void moveTail(Snake* snake, int mx, int my);
void freeCorners();
void printDebug(Snake* snake, int mx, int my);

#endif