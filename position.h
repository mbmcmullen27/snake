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

Position* corners[8];

void initPosition(Position*);
void advancePosition(Position*);
void addCorner(Position*);
Position* popCorner();
Position* initCorner(int x, int y, int dir);

#endif