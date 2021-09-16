#ifndef snake_h
#define snake_h

typedef struct {
    Position* head;
    Position* tail;
    int direction;
} Snake;

void initSnake();
void moveHead();
void moveTail();


#endif