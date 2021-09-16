#ifndef position_h
#define position_h

typedef struct {
    int x;
    int y;
    int dir;
    int prev;
} Position;

void initPosition(Position*);
void advancePosition(Position*);

#endif