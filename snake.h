#ifndef snake_h
#define snake_h

#include "position.h"

typedef struct {
    Position* head;
    Position* tail;
} Snake;

void initSnake();
void startGame();
void gameOver();
void moveHead(Snake* snake, int dir);
void moveTail(Position* tail);

#endif