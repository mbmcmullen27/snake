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
        if(top>1) moveTail(snake.tail);

        refresh();
        usleep(250000); // 50000
        mvprintw(my-1,0,"corners: %i length: %i bottom: %i",top, length, bottom);
        if(top>1) mvprintw(my,0,"top: (%i, %i) bottom: (%i, %i) tail: (%i, %i)    ", 
            corners[top-1]->x, corners[top-1]->y,
            corners[bottom]->x, corners[bottom]->y,
            snake.tail->x, snake.tail->y);
        int key = getch();
        if (key != ERR) snake.head->dir = key;
        if (bottom == length/2) freeCorners(bottom);
    }

}

void freeCorners(int len) {
    mvprintw(my-2,0,"Reduced at %i     ", top-1);

    int i;
    Position** res = malloc(sizeof(Position*)*(length-len));

    for(i = 0; i < len; i++) {
        free(corners[i]);
    }

    for(i = 0;i+len<length; i++) {
        mvprintw(my-2,mx/2,"index: %i     ", i);
        res[i] = corners[i+len];
    }

    free(corners);
    corners = res;
    length = length - len;
    top = top - len;
    bottom = bottom - len;
}

void initSnake() {
    length=4;
    corners = malloc(sizeof(Position*)*length);
    top=0;
    bottom=0;

    Position* head = initPosition();
    Position* tail = initPosition();

    snake.head = head;
    snake.tail = tail;
    pushCorner();
}

Position* initCorner(int x, int y, int dir) {
    Position* res = malloc(sizeof(Position));
    res->x=x;
    res->y=y;
    res->dir=dir;
    res->visited=false;
    return res;
}

void expandCorners() {
    mvprintw(my-2,0,"Expanded at %i", top);
    length*=2;
    Position** temp = realloc(corners, length*sizeof(Position*));   
    corners = temp;
}

void pushCorner() {
    if(top==length-1) expandCorners();
    
    Position* pos = snake.head;
    corners[top] = initCorner(pos->x,pos->y,pos->dir);
    if(corners[bottom]->visited) bottom++;
    top++;
}

void moveHead(Position* head) {
    switch(head->dir) {
        case KEY_RIGHT: // east
            if(head->x < mx && head->prev != KEY_LEFT) {
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
            } else if (head->prev == KEY_LEFT){
                mvaddch(head->y, head->x,'-');    
                mvaddch(head->y,head->x-1,'<');
                head->x--;
            }
            break;
        case KEY_UP: // north
            if(head->y > 0 && head->prev != KEY_DOWN) {
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
            } else if(head->prev == KEY_DOWN) {
                mvaddch(head->y, head->x, '|');
                mvaddch(head->y+1,head->x,'v');
                head->y++;
            }
            break;
        case KEY_LEFT: // west
            if(head->x > 0 && head->prev != KEY_RIGHT) {
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
            } else if(head->prev == KEY_RIGHT){
                mvaddch(head->y, head->x,'-');    
                mvaddch(head->y, head->x+1,'>');
                head->x++;
            }
            break;
        case KEY_DOWN: // south
            if(head->y < my && head->prev != KEY_UP) {
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
            } else if (head->prev == KEY_UP){
                mvaddch(head->y, head->x, '|');
                mvaddch(head->y-1, head->x, '^');
                head->y--;
            }
            break;
        default:
            break;
    }
}

void moveTail(Position* tail) {
    
    mvaddch(tail->y, tail->x, ' ');
    
    if (bottom < length && tail->x == corners[bottom]->x && tail->y == corners[bottom]->y) {
        tail->dir = corners[bottom]->dir;
        corners[bottom]->visited=true;
        if(bottom<top-1){
            bottom++;
        }
    } else if (tail->x == corners[top-1]->x && tail->y == corners[top-1]->y) {
        tail->dir = corners[top-1]->dir;
        corners[bottom]->visited=true;
        bottom++;
    }

    switch(tail->dir) {
        case KEY_RIGHT:
            if(snake.head->x < mx) tail->x++;
            break;
        case KEY_UP:
            if(snake.head->y > 0) tail->y--;
            break;
        case KEY_LEFT:
            if(snake.head->x > 0) tail->x--;
            break;
        case KEY_DOWN:
            if(snake.head->y < my) tail->y++;
            break;
    }
       
}