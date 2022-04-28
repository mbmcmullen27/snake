#ifndef game_h
#define game_h

#include <curses.h>

#include "pyClient.h"
#include "snake.h"

typedef struct {
    int ticks;
    int lastHit;
    int mx;
    int my;
    int yoffset;
    bool kubeEnabled;
    Snake* snake;
} Game;

void collect();
bool check(Game*);
void startGame(Game*);
void gameOver(Game*);
void moveHead(Game*, int dir);
void moveTail(Game*);
void moveUp(Game*);
void moveDown(Game*);
void moveLeft(Game*);
void moveRight(Game*);
void printDebug(Game*);

#endif