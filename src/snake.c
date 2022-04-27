#include "snake.h"

Snake* initSnake(int my) {
    Snake* snake = malloc(sizeof(Snake));
    Position* head = initPosition();
    Position* tail = initPosition();
    snake->top = snake->bottom = initCorner(0,head->y,KEY_RIGHT);
    snake->bottom->visited = true;

    head->y = my/2;
    tail->y = my/2;
    head->dir = KEY_RIGHT;

    snake->head = head;
    snake->tail = tail;

    return snake;
}

void freeCorner(Snake* snake) {
    Corner* temp = snake->bottom;
    snake->bottom=snake->bottom->next;
    free(temp->position);
    free(temp);
}

void freeCorners(Snake* snake) {
    while (snake->bottom!=NULL) {
        freeCorner(snake);
    }
}

void pushCorner(Snake* snake) {
    Position* pos = snake->head;
    Corner* res = initCorner(pos->x,pos->y,pos->dir); 
    snake->top->next = res; 
    snake->top = snake->top->next; 
    if(snake->bottom->visited) snake->bottom=snake->top;
}

void popCorner(Snake* snake) {
    snake->bottom->visited=true;
    if(snake->bottom->next != NULL) {
        freeCorner(snake);
    }
}

void printDir(int dir, char* id, int y, int x) {
    switch(dir) {
        case KEY_RIGHT:
            mvprintw(y, x, "%s: RIGHT   ",id);
            break;
        case KEY_UP:
            mvprintw(y, x, "%s: UP   ",id);
            break;
        case KEY_LEFT:
            mvprintw(y, x, "%s: LEFT    ",id);
            break;
        case KEY_DOWN:
            mvprintw(y, x, "%s: DOWN   ",id);
            break;
        default:
            mvprintw(y, x, "%s: %i",id, dir);
            break;
    }
}

void printPos(char* name, Position* pos, int y, int x) {
    mvprintw(y,x,"%s: (%i, %i)    ", name, pos->x, pos->y);
    printDir(pos->dir,"dir",y, x+18);
}

void printDebug(Snake* snake, int mx, int my) {
    printPos("top", snake->top->position, my-3, 0);
    printPos("bottom", snake->bottom->position, my-2, 0);
    printPos("tail", snake->tail, my-1, 0);
}