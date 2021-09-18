#ifndef snake_h
#define snake_h

#include "position.h"

Position* corners[8];

typedef struct {
    Position* head;
    Position* tail;
} Snake;

void initSnake();
void moveHead(Position* head);
void moveTail(Position* tail);
void addCorner(Position*);
Position* popCorner();
Position* initCorner(int x, int y, int dir);

Position* initPosition() {
    Position* res = malloc(sizeof(Position));
    res->x=0;
    res->y=0;
    res->dir=KEY_RIGHT;
    res->prev=KEY_RIGHT;
    return res;
}

#endif