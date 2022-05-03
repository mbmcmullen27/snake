#include "position.h"

Position* initPosition() {
    Position* res = malloc(sizeof(Position));
    res->x=2;
    res->y=1;
    res->dir=KEY_RIGHT;
    res->prev=KEY_RIGHT;
    return res;
}

Corner* initCorner(int x, int y, int dir) {
    Corner* res = malloc(sizeof(Corner));
    res->position = initPosition();
    res->position->x=x;
    res->position->y=y;
    res->position->dir=dir;
    res->visited=false;
    res->next=NULL;
    return res;
}
