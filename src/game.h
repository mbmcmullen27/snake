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
    Snake* snake;
} Game;

void collect();
bool check(Game* game);
void startGame(Game* game);
void gameOver(Game* game);
void moveHead(Game* game, int dir);
void moveTail(Game* game);
void moveUp(Game* game);
void moveDown(Game* game);
void moveLeft(Game* game);
void moveRight(Game* game);

#endif