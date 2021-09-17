#ifndef snake_h
#define snake_h

#include "position.h"

typedef struct {
    Position* head;
    Position* tail;
    int direction;
} Snake;

void initSnake();
void moveHead(Position* head);
void moveTail(Position* tail);

#endif