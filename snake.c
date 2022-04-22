#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "snake.h"

#ifndef CTRL
#define CTRL(c) ((c) & 0x1f)
#endif

Snake snake;
Corner* top;
Corner* bottom;

int ticks;
int lastHit;
int mx, my;

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
        moveHead(&snake, snake.head->dir);
        if(ticks>5 && (ticks > lastHit+3)) moveTail(snake.tail);

        refresh();
        usleep(150000); 

        mvprintw(my-2,mx/2, "ticks: %i lastHit: %i   ",ticks, lastHit);
        printPos("top", top->position, my-3, 0);
        printPos("bottom", bottom->position, my-2, 0);
        printPos("tail", snake.tail, my-1, 0);
        
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

void initSnake() {
    Position* head = initPosition();
    Position* tail = initPosition();
    top = bottom = initCorner(0,head->y,KEY_RIGHT);
    bottom->visited = true;

    head->y = my/2;
    tail->y = my/2;
    head->dir = KEY_RIGHT;

    snake.head = head;
    snake.tail = tail;
}

void freeCorner() {
    Corner* temp = bottom;
    bottom=bottom->next;
    free(temp->position);
    free(temp);
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
        freeCorner();
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
    mvprintw(my-1,mx/2,"mx: %i my: %i food: (%i,%i)     ",mx,my,x+1,y+1);
    mvaddch(y+1,x+1, '*');
}

void gameOver() {
    while (bottom!=NULL) {
        freeCorner();
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

void printPos(char* name, Position* pos, int y, int x) {
    mvprintw(y,x,"%s: (%i, %i)    ", name, pos->x, pos->y);
    printDir(pos->dir,"dir",y, x+18);
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

void moveHead(Snake* snake, int dir) {
    Position* head = snake->head;
    switch(dir) {
        case 'd':
        case 'l':
            head->dir=KEY_RIGHT;
        case KEY_RIGHT: // east
            if(head->prev == KEY_LEFT) {
                moveLeft();
            } else {
                moveRight();
            }
            break;
        case 'w':
        case 'k':
            head->dir=KEY_UP;
        case KEY_UP: // north
            if(head->prev == KEY_DOWN) {
                moveDown();
            } else {
                moveUp();
            }
            break;
        case 'a':
        case 'h':
            head->dir=KEY_LEFT;
        case KEY_LEFT: // west
            if(head->prev == KEY_RIGHT) {
                moveRight();
            } else {
                moveLeft();
            }
            break;
        case 's':
        case 'j':
            head->dir=KEY_DOWN;
        case KEY_DOWN: // south
            if(head->prev == KEY_UP) {
                moveUp();
            } else {
                moveDown();
            }
            break;
        default:
            moveHead(snake, snake->head->prev);
            break;
    }
}

void moveTail(Position* tail) {
    mvaddch(tail->y, tail->x, ' ');
    
    if (tail->x == bottom->position->x && tail->y == bottom->position->y) {
        tail->dir = bottom->position->dir;
        popCorner();
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