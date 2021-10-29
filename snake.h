#ifndef snake_h
#define snake_h

#include "position.h"

Position** corners;
int top;
int bottom;
int length;

typedef struct {
    Position* head;
    Position* tail;
} Snake;

void initSnake();
void startGame();
void gameOver();
void moveHead(Position* head);
void moveTail(Position* tail);
void addCorner(Position* next);
void expandCorners();
void freeCorners(int len);
Position* popCorner();
void pushCorner();
Position* initCorner(int x, int y, int dir);

Position* initPosition() {
    Position* res = malloc(sizeof(Position));
    res->x=2;
    res->y=1;
    res->dir=KEY_RIGHT;
    res->prev=KEY_RIGHT;
    res->visited=false;
    return res;
}

#endif