#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "snake.h"

Snake snake;
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
        mvprintw(my-1,0,"corners: %i length: %i bottom: %i    ",top, length, bottom);
        mvprintw(my-2,mx/2, "ticks: %i lastHit: %i   ",ticks, lastHit);
        if(top>1) mvprintw(my,0,"top: (%i, %i) bottom: (%i, %i) tail: (%i, %i)    ", 
            corners[top-1]->x, corners[top-1]->y,
            corners[bottom]->x, corners[bottom]->y,
            snake.tail->x, snake.tail->y);
        key = getch();
        if (key != ERR) snake.head->dir = key;
        if (bottom == length/2) freeCorners(bottom);
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
    mvprintw(my-2,0,"Reduced at %i     ", top-1);

    int i;
    Position** res = malloc(sizeof(Position*)*(length-len));

    for(i = 0; i < len; i++) {
        free(corners[i]);
    }

    for(i = 0;i+len<length; i++) {
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

    head->y = my/2;
    tail->y = my/2;

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
    for(int i = bottom; i < top; i++) {
        free(corners[i]);
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

void printDir(int dir, char* id, int offset) {
    switch(dir) {
        case KEY_RIGHT:
            mvprintw(my-3, mx/2 + offset, "%s: RIGHT   ",id);
            break;
        case KEY_UP:
            mvprintw(my-3, mx/2 + offset, "%s: UP   ",id);
            break;
        case KEY_LEFT:
            mvprintw(my-3, mx/2 + offset, "%s: LEFT    ",id);
            break;
        case KEY_DOWN:
            mvprintw(my-3, mx/2 + offset, "%s: DOWN   ",id);
            break;
        default:
            mvprintw(my-3, mx/2 + offset, "%s: %i",id, dir);
            break;
    }
}

bool check() {
    Position* head = snake.head;
    printDir(head->dir,"dir", 0);
    printDir(head->prev,"prev", 12);
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