#ifndef snake_h
#define snake_h

#include "position.h"

typedef struct {
    Position* head;
    Position* tail;
    Corner* corners;
} Snake;

void initSnake();
void startGame();
void gameOver();
void moveHead(Position* head);
void moveTail(Position* tail);

Position* initPosition() {
    Position* res = malloc(sizeof(Position));
    res->x=2;
    res->y=1;
    res->dir=KEY_RIGHT;
    res->prev=KEY_RIGHT;
    return res;
}

#endif