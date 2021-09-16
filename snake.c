#include <curses.h>
#include <unistd.h>

#include "position.h"

int main() {
    Position head;
    Position tail;
    int mx;
    int my;

    initscr();
    curs_set(0);
    refresh();
    getmaxyx(stdscr,my,mx);
    mx--;
    my--;

    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); 
    noecho();

    initPosition(&head);
    initPosition(&tail);
    
    for(;;) {
        switch(head.dir) {
            case KEY_RIGHT: // east
                if(head.x < mx) {
                    if (head.prev == KEY_UP) {
                        mvaddch(head.y, head.x, A_ALTCHARSET | ACS_ULCORNER);
                    } else if (head.prev == KEY_DOWN){
                        mvaddch(head.y, head.x, A_ALTCHARSET | ACS_LLCORNER);
                    } else {
                        mvaddch(head.y, head.x,'-');    
                    }
                    mvaddch(head.y, head.x+1,'>');
                    head.x++;
                    head.prev = KEY_RIGHT;
                } else {
                    // out of bounds
                }
                break;
            case KEY_UP: // north
                if(head.y > 0) {
                    if (head.prev == KEY_LEFT) {
                        mvaddch(head.y, head.x,A_ALTCHARSET | ACS_LLCORNER);
                    } else if (head.prev == KEY_RIGHT){
                        mvaddch(head.y, head.x,A_ALTCHARSET | ACS_LRCORNER);
                    } else {
                        mvaddch(head.y, head.x, '|');
                    }
                    mvaddch(head.y-1, head.x, '^');
                    head.y--;
                    head.prev = KEY_UP;
                } else {
                    // out of bounds
                }
                break;
            case KEY_LEFT: // west
                if(head.x > 0) {
                    if (head.prev == KEY_UP) {
                        mvaddch(head.y, head.x,A_ALTCHARSET | ACS_URCORNER);
                    } else if (head.prev == KEY_DOWN){
                        mvaddch(head.y, head.x,A_ALTCHARSET | ACS_LRCORNER);
                    } else {
                        mvaddch(head.y, head.x, '-');
                    }
                    mvaddch(head.y,head.x-1,'<');
                    head.x--;
                    head.prev = KEY_LEFT;
                } else {
                    // out of bounds
                }
                break;
            case KEY_DOWN: // south
                if(head.y < my) {
                    if (head.prev == KEY_LEFT) {
                        mvaddch(head.y, head.x,A_ALTCHARSET | ACS_ULCORNER);
                    } else if (head.prev == KEY_RIGHT){
                        mvaddch(head.y, head.x,A_ALTCHARSET | ACS_URCORNER);
                    } else {
                        mvaddch(head.y, head.x, '|');
                    }
                    mvaddch(head.y+1,head.x,'v');
                    head.y++;
                    head.prev = KEY_DOWN;
                } else {
                    // out of bounds
                }
                break;
            default:
                break;
        }
        refresh();
        usleep(50000);
        int key = getch();
        if (key != ERR) head.dir = key;
    }

}

void initPosition(Position* pos) {
    pos->x=0;
    pos->y=0;
    pos->dir=KEY_RIGHT;
    pos->prev=KEY_RIGHT;
}

void moveHead(int direction) {

}
 