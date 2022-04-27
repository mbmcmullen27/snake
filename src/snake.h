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
void freeCorner(Snake* snake);
void freeCorners(Snake* snake);
void pushCorner(Snake* snake);
void popCorner(Snake* snake);
void printDebug(Snake* snake, int mx, int my);
void printDir(int, char*, int, int);
void printPos(char*, Position*, int, int);

#endif