#ifndef position_h
#define position_h

#include <stdbool.h>

typedef struct {
    int x;
    int y;
    int dir;
    int prev;
} Position;

typedef struct Corner{
    struct Corner* next;
    bool visited;
    Position* position;
} Corner;

Position* initPosition();
Corner* initCorner(int x, int y, int dir);

#endif