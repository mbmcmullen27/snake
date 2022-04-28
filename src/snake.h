#ifndef snake_h
#define snake_h

#include <unistd.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "position.h"

typedef struct {
    Position* head;
    Position* tail;
    Corner* top;
    Corner* bottom;
} Snake;

Snake* initSnake(int my);
void freeCorner(Snake*);
void freeCorners(Snake*);
void pushCorner(Snake*);
void popCorner(Snake*);
void printDir(int, char*, int, int);
void printPos(char*, Position*, int, int);

#endif