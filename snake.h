#ifndef snake_h
#define snake_h

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "position.h"

typedef struct {
    Position* head;
    Position* tail;
} Snake;

void initSnake(int my);
void startGame();
void gameOver();
void moveHead(Snake* snake, int dir);
void moveTail(Position* tail);

#endif