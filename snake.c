#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "snake.h"

Snake snake;
Corner* top;
Corner* bottom;

int ticks;
int lastHit;

int main() {
    initscr();
    curs_set(0);
    refresh();
    getmaxyx(stdscr,my,mx);

    keypad(stdscr, TRUE);
    noecho();

    srand(time(NULL));

    startGame();

    int key;

    for(ticks = 0;;ticks++) {
        moveHead(snake.head);

        refresh();
        usleep(150000); 
        // mvprintw(my-1,0,"corners: %i length: %i bottom: %i    ",top, length, bottom);
        mvprintw(my-2,mx/2, "ticks: %i lastHit: %i   ",ticks, lastHit);
        mvprintw(my-2,0,"top: (%i, %i) bottom: (%i, %i) tail: (%i, %i)    ", 
            top->position->x, top->position->y,
            bottom->position->x, bottom->position->y,
            snake.tail->x, snake.tail->y);
        key = getch();
        if (key != ERR) snake.head->dir = key;
    }

}

void startGame(){
    nodelay(stdscr, TRUE); 

    // draw border
    move(0,0);
    hline('-', mx-1);
    vline('|',my-4);
    addch(A_ALTCHARSET | ACS_ULCORNER);
    move(my-4,0);
    hline('-', mx-1);
    addch(A_ALTCHARSET | ACS_LLCORNER);
    move(0,mx-1);
    vline('|',my-4);
    addch(A_ALTCHARSET | ACS_URCORNER);
    mvaddch(my-4, mx-1,A_ALTCHARSET | ACS_LRCORNER);

    initSnake();
    lastHit = 0;
    ticks = 0;
    mvaddch(my/2,mx/2,'*');
}

void freeCorners(int len) {
}

void initSnake() {

    Position* head = initPosition();
    Position* tail = initPosition();
    snake.corners = initCorner(0,head->y,KEY_RIGHT);

    head->y = my/2;
    tail->y = my/2;
    head->dir = KEY_RIGHT;
    
    top = bottom = snake.corners;
    bottom->visited = true;

    snake.head = head;
    snake.tail = tail;
}

Corner* initCorner(int x, int y, int dir) {
    Corner* res = malloc(sizeof(Corner));
    res->position = initPosition();
    res->position->x=x;
    res->position->y=y;
    res->position->dir=dir;
    res->visited=false;
    res->next=NULL;
    return res;
}

void pushCorner() {
    Position* pos = snake.head;
    Corner* res = initCorner(pos->x,pos->y,pos->dir); 
    top->next = res; 
    top = top->next; 
    if(bottom->visited) bottom=top;
}

void popCorner() {
    bottom->visited=true;
    if(bottom->next != NULL) {
       bottom=bottom->next;
    }
}

void collect() {
    if (lastHit > ticks - 3) {
        lastHit = lastHit + 3;
    } else {
        lastHit = ticks;
    }

    int x, y;
    char character;
    do {
        x = rand() % (mx-2);
        y = rand() % (my-4);
        character = mvinch(y+1,x+1) & A_CHARTEXT;
    } while (!isspace(character));
    mvprintw(my-3,0,"mx: %i my: %i food: (%i,%i)     ",mx,my,x+1,y+1);
    mvaddch(y+1,x+1, '*');
}

void gameOver() {
    bottom = snake.corners;
    while (bottom!=NULL) {
        Corner* temp = bottom;
        bottom = bottom->next;
        free(temp);
    }

    nodelay(stdscr, false); 

    WINDOW *win = newwin(11,20,((my-4)/2)-5,((mx-4)/2)-5);

    box(win, '|', '=');
    mvwprintw(win, 3,5, "GAME OVER");
    mvwprintw(win, 5,4, "Play again?");
    mvwprintw(win, 7,8, "Y/N");
    touchwin(win);
    wrefresh(win);

    int selection;
    do {
        selection = getch();
        if (selection == 'y') {
            clear();
            refresh();
            startGame();
        } else if (selection == 'n') {
            clear();
            refresh();
            endwin();
            exit(0);
        }
    } while (selection != 'y' && selection != 'n');
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

void printPos(Position* pos, int y, int x) {

}

bool check() {
    Position* head = snake.head;
    printDir(head->dir,"dir", my-3, mx/2);
    printDir(head->prev,"prev", my-3, (mx/2)+12);
    char current = mvinch(head->y, head->x) & A_CHARTEXT;
    if ((current) == '*'){
        collect();
    } else if (!isspace(current)){
        gameOver();
        return false; // unreachable
    } 
    
    if(ticks>5 && (ticks > lastHit+3)) moveTail(snake.tail);

    return true;
}

void moveRight(){
    Position* head = snake.head;
    if (head->prev == KEY_UP) {
        mvaddch(head->y, head->x, A_ALTCHARSET | ACS_ULCORNER);
        pushCorner();
    } else if (head->prev == KEY_DOWN){
        mvaddch(head->y, head->x, A_ALTCHARSET | ACS_LLCORNER);
        pushCorner();
    } else {
        mvaddch(head->y, head->x,'-');    
    }
    head->x++;
    if(check()) {
        mvaddch(head->y, head->x,'>');
        head->prev = KEY_RIGHT;
    }
}

void moveLeft() {
    Position* head = snake.head;
    if (head->prev == KEY_UP) {
        mvaddch(head->y, head->x,A_ALTCHARSET | ACS_URCORNER);
        pushCorner();
    } else if (head->prev == KEY_DOWN){
        mvaddch(head->y, head->x,A_ALTCHARSET | ACS_LRCORNER);
        pushCorner();
    } else {
        mvaddch(head->y, head->x, '-');
    }
    head->x--;
    if (check()) {
        mvaddch(head->y,head->x,'<');
        head->prev = KEY_LEFT;
    }
}

void moveUp() {
    Position* head = snake.head;
    if (head->prev == KEY_LEFT) {
        mvaddch(head->y, head->x,A_ALTCHARSET | ACS_LLCORNER);
        pushCorner();
    } else if (head->prev == KEY_RIGHT){
        mvaddch(head->y, head->x,A_ALTCHARSET | ACS_LRCORNER);
        pushCorner();
    } else {
        mvaddch(head->y, head->x, '|');
    }
    head->y--;
    if (check()) {
        mvaddch(head->y, head->x, '^');
        head->prev = KEY_UP;
    }
}

void moveDown() {
    Position* head = snake.head;
    if (head->prev == KEY_LEFT) {
        mvaddch(head->y, head->x,A_ALTCHARSET | ACS_ULCORNER);
        pushCorner();
    } else if (head->prev == KEY_RIGHT){
        mvaddch(head->y, head->x,A_ALTCHARSET | ACS_URCORNER);
        pushCorner();
    } else {
        mvaddch(head->y, head->x, '|');
    }
    head->y++;
    if (check()) {
        mvaddch(head->y,head->x,'v');
        head->prev = KEY_DOWN;
    }
}

void moveHead(Position* head) {
    switch(head->dir) {
        case KEY_RIGHT: // east
            if(head->prev == KEY_LEFT) {
                moveLeft();
            } else {
                moveRight();
            }
            break;
        case KEY_UP: // north
            if(head->prev == KEY_DOWN) {
                moveDown();
            } else {
                moveUp();
            }
            break;
        case KEY_LEFT: // west
            if(head->prev == KEY_RIGHT) {
                moveRight();
            } else {
                moveLeft();
            }
            break;
        case KEY_DOWN: // south
            if(head->prev == KEY_UP) {
                moveUp();
            } else {
                moveDown();
            }
            break;
        default:
            break;
    }

}

void moveTail(Position* tail) {
    
    mvaddch(tail->y, tail->x, ' ');
    
    if (tail->x == bottom->position->x && 
            tail->y == bottom->position->y) {
        tail->dir = bottom->position->dir;
        popCorner();
    } 
    // else if (tail->x == top->position->x &&
    //         tail->y == top->position->y) {
    //     tail->dir = top->position->dir;
    //     bottom->visited=true;
    //     bottom=bottom->next;

    // }

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