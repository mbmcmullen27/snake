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

typedef struct Corner{
    struct Corner* next;
    bool visited;
    Position* position;
} Corner;


Position* initPosition();
Corner* initCorner(int x, int y, int dir);
void pushCorner();
void popCorner();
void freeCorners(int len);
void printPos(char* name, Position* pos, int y, int x); 

#endif