#include "pyClient.h"

#ifndef CTRL
#define CTRL(c) ((c) & 0x1f)
#endif

int ticks;
int lastHit;
int mx, my;

Snake snake;

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

    snake = initSnake();
    lastHit = 0;
    ticks = 0;
    mvaddch(my/2,mx/2,'*');
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
