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
        int key = getch();
        if (key != ERR) snake.head->dir = key;
    }

}

void initSnake(){
    Position* head = malloc(sizeof (Position));
    Position* tail = malloc(sizeof (Position));

    initPosition(head);
    initPosition(tail);
    
    snake.head = head;
    snake.tail = tail;

    snake.direction = head->dir; 
}

void initPosition(Position* pos) {
    pos->x=0;
    pos->y=0;
    pos->dir=KEY_RIGHT;
    pos->prev=KEY_RIGHT;
}

void moveHead(Position* head) {
    switch(head->dir) {
        case KEY_RIGHT: // east
            if(head->x < mx) {
                if (head->prev == KEY_UP) {
                    mvaddch(head->y, head->x, A_ALTCHARSET | ACS_ULCORNER);
                } else if (head->prev == KEY_DOWN){
                    mvaddch(head->y, head->x, A_ALTCHARSET | ACS_LLCORNER);
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
                } else if (head->prev == KEY_RIGHT){
                    mvaddch(head->y, head->x,A_ALTCHARSET | ACS_LRCORNER);
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
                } else if (head->prev == KEY_DOWN){
                    mvaddch(head->y, head->x,A_ALTCHARSET | ACS_LRCORNER);
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
                } else if (head->prev == KEY_RIGHT){
                    mvaddch(head->y, head->x,A_ALTCHARSET | ACS_URCORNER);
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