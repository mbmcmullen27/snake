#ifndef position_h
#define position_h

int mx;
int my;

typedef struct {
    int x;
    int y;
    int dir;
    int prev;
    bool visited;
} Position;

Position* initPosition();

#endif