#include "snake.h"

Corner* top;
Corner* bottom;

void initSnake(Snake* snake, int my) {
    Position* head = initPosition();
    Position* tail = initPosition();
    top = bottom = initCorner(0,head->y,KEY_RIGHT);
    bottom->visited = true;

    head->y = my/2;
    tail->y = my/2;
    head->dir = KEY_RIGHT;

    snake->head = head;
    snake->tail = tail;
}

Position* bottomPos() {
    return bottom->position;
}

void freeCorner() {
    Corner* temp = bottom;
    bottom=bottom->next;
    free(temp->position);
    free(temp);
}

void freeCorners() {
    while (bottom!=NULL) {
        freeCorner();
    }
}

void pushCorner(Snake* snake) {
    Position* pos = snake->head;
    Corner* res = initCorner(pos->x,pos->y,pos->dir); 
    top->next = res; 
    top = top->next; 
    if(bottom->visited) bottom=top;
}

void popCorner() {
    bottom->visited=true;
    if(bottom->next != NULL) {
        freeCorner();
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
    printPos("top", top->position, my-3, 0);
    printPos("bottom", bottom->position, my-2, 0);
    printPos("tail", snake->tail, my-1, 0);
}