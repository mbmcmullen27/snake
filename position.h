#ifndef position_h
#define position_h

int mx;
int my;

typedef struct {
    int x;
    int y;
    int dir;
    int prev;
} Position;

Position* initPosition();

#endif