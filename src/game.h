#ifndef game_h
#define game_h

#include <curses.h>

#include "pyClient.h"
#include "snake.h"

typedef struct {
    Snake* snake;
    int ticks;
    int lastHit;
    int mx;
    int my;
} Game;

void startGame(Game* game);
void moveHead(Game* game);
void moveTail(Game* game);
void collect(Game* game);
void gameOver(Game* game);
bool check(Game* game);
void printDir(int, char*, int, int);
void printPos(char*, Position*, int, int);

#endif