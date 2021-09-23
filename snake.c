#include <curses.h>
#include <unistd.h>
#include <stdlib.h>

#include "snake.h"

Snake snake;

int main() {

    initscr();
    curs_set(0);
    refresh();
    getmaxyx(stdscr,my,mx);
    mx--;
    my--;

    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); 
    noecho();

    initSnake();

    for(;;) {
        moveHead(snake.head);
        moveTail(snake.tail);
        refresh();
        usleep(50000);
        mvprintw(my-1,0,"corners: %i length: %i",top, length);
        if(top>0) mvprintw(my,0,"top: (%i, %i)", corners[top-1]->x, corners[top-1]->y);
        int key = getch();
        if (key != ERR) snake.head->dir = key;
    }

}

void initSnake(){
    length=4;
    corners = malloc(sizeof(Position*)*length);
    top=0;

    Position* head = initPosition();
    Position* tail = initPosition();
        
    snake.head = head;
    snake.tail = tail;
}

Position* initCorner(int x, int y, int dir) {
    Position* res = malloc(sizeof(Position));
    res->x=x;
    res->y=y;
    res->dir=dir;
    return res;
}

void expandCorners(){
    mvprintw(my,mx/2,"Expanding at %i", top);
    length*=2;
    Position** temp = realloc(corners, length*sizeof(Position*));   
    corners = temp;
}

void pushCorner(){
    if(top==length-1) expandCorners();
    
    Position* pos = snake.head;
    corners[top] = initCorner(pos->x,pos->y,pos->dir);
    top++;
}

void moveHead(Position* head) {
    switch(head->dir) {
        case KEY_RIGHT: // east
            if(head->x < mx) {
                if (head->prev == KEY_UP) {
                    mvaddch(head->y, head->x, A_ALTCHARSET | ACS_ULCORNER);
                    pushCorner();
                } else if (head->prev == KEY_DOWN){
                    mvaddch(head->y, head->x, A_ALTCHARSET | ACS_LLCORNER);
                    pushCorner();
                } else {
                    mvaddch(head->y, head->x,'-');    
                }
                mvaddch(head->y, head->x+1,'>');
                head->x++;
                head->prev = KEY_RIGHT;
            } else {
                // out of bounds
            }
            break;
        case KEY_UP: // north
            if(head->y > 0) {
                if (head->prev == KEY_LEFT) {
                    mvaddch(head->y, head->x,A_ALTCHARSET | ACS_LLCORNER);
                    pushCorner();
                } else if (head->prev == KEY_RIGHT){
                    mvaddch(head->y, head->x,A_ALTCHARSET | ACS_LRCORNER);
                    pushCorner();
                } else {
                    mvaddch(head->y, head->x, '|');
                }
                mvaddch(head->y-1, head->x, '^');
                head->y--;
                head->prev = KEY_UP;
            } else {
                // out of bounds
            }
            break;
        case KEY_LEFT: // west
            if(head->x > 0) {
                if (head->prev == KEY_UP) {
                    mvaddch(head->y, head->x,A_ALTCHARSET | ACS_URCORNER);
                    pushCorner();
                } else if (head->prev == KEY_DOWN){
                    mvaddch(head->y, head->x,A_ALTCHARSET | ACS_LRCORNER);
                    pushCorner();
                } else {
                    mvaddch(head->y, head->x, '-');
                }
                mvaddch(head->y,head->x-1,'<');
                head->x--;
                head->prev = KEY_LEFT;
            } else {
                // out of bounds
            }
            break;
        case KEY_DOWN: // south
            if(head->y < my) {
                if (head->prev == KEY_LEFT) {
                    mvaddch(head->y, head->x,A_ALTCHARSET | ACS_ULCORNER);
                    pushCorner();
                } else if (head->prev == KEY_RIGHT){
                    mvaddch(head->y, head->x,A_ALTCHARSET | ACS_URCORNER);
                    pushCorner();
                } else {
                    mvaddch(head->y, head->x, '|');
                }
                mvaddch(head->y+1,head->x,'v');
                head->y++;
                head->prev = KEY_DOWN;
            } else {
                // out of bounds
            }
            break;
        default:
            break;
    }
}

void moveTail(Position* tail) {

}