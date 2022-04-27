#include "game.h"

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
        moveHead(&snake, snake.head->dir);
        if(ticks>5 && (ticks > lastHit+3)) moveTail(&snake, mx, my);

        refresh();
        usleep(150000); 

        mvprintw(my-2,mx/2, "ticks: %i lastHit: %i   ",ticks, lastHit);
        printDebug(&snake, mx, my);
        
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

    initSnake(&snake, my);
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
    freeCorners(&snake);
    nodelay(stdscr, false); 

    WINDOW *win = newwin(11,20,((my-4)/2)-5,((mx-4)/2)-5);

    deletePods();

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
        createPod();
    } else if (!isspace(current)){
        gameOver();
        return false; // unreachable
    } 

    return true;
}

void moveRight(Snake* snake){
    Position* head = snake->head;
    if (head->prev == KEY_UP) {
        mvaddch(head->y, head->x, A_ALTCHARSET | ACS_ULCORNER);
        pushCorner(snake);
    } else if (head->prev == KEY_DOWN){
        mvaddch(head->y, head->x, A_ALTCHARSET | ACS_LLCORNER);
        pushCorner(snake);
    } else {
        mvaddch(head->y, head->x,'-');    
    }
    head->x++;
    if(check()) {
        mvaddch(head->y, head->x,'>');
        head->prev = KEY_RIGHT;
    }
}

void moveLeft(Snake* snake) {
    Position* head = snake->head;
    if (head->prev == KEY_UP) {
        mvaddch(head->y, head->x,A_ALTCHARSET | ACS_URCORNER);
        pushCorner(snake);
    } else if (head->prev == KEY_DOWN){
        mvaddch(head->y, head->x,A_ALTCHARSET | ACS_LRCORNER);
        pushCorner(snake);
    } else {
        mvaddch(head->y, head->x, '-');
    }
    head->x--;
    if (check()) {
        mvaddch(head->y,head->x,'<');
        head->prev = KEY_LEFT;
    }
}

void moveUp(Snake* snake) {
    Position* head = snake->head;
    if (head->prev == KEY_LEFT) {
        mvaddch(head->y, head->x,A_ALTCHARSET | ACS_LLCORNER);
        pushCorner(snake);
    } else if (head->prev == KEY_RIGHT){
        mvaddch(head->y, head->x,A_ALTCHARSET | ACS_LRCORNER);
        pushCorner(snake);
    } else {
        mvaddch(head->y, head->x, '|');
    }
    head->y--;
    if (check()) {
        mvaddch(head->y, head->x, '^');
        head->prev = KEY_UP;
    }
}

void moveDown(Snake* snake) {
    Position* head = snake->head;
    if (head->prev == KEY_LEFT) {
        mvaddch(head->y, head->x,A_ALTCHARSET | ACS_ULCORNER);
        pushCorner(snake);
    } else if (head->prev == KEY_RIGHT){
        mvaddch(head->y, head->x,A_ALTCHARSET | ACS_URCORNER);
        pushCorner(snake);
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
                moveLeft(snake);
            } else {
                moveRight(snake);
            }
            break;
        case 'w':
        case 'k':
            head->dir=KEY_UP;
        case KEY_UP: // north
            if(head->prev == KEY_DOWN) {
                moveDown(snake);
            } else {
                moveUp(snake);
            }
            break;
        case 'a':
        case 'h':
            head->dir=KEY_LEFT;
        case KEY_LEFT: // west
            if(head->prev == KEY_RIGHT) {
                moveRight(snake);
            } else {
                moveLeft(snake);
            }
            break;
        case 's':
        case 'j':
            head->dir=KEY_DOWN;
        case KEY_DOWN: // south
            if(head->prev == KEY_UP) {
                moveUp(snake);
            } else {
                moveDown(snake);
            }
            break;
        default:
            moveHead(snake, snake->head->prev);
            break;
    }
}

void moveTail(Snake* snake, int mx, int my) {
    Position* tail = snake->tail;
    Position* head = snake->head;

    mvaddch(tail->y, tail->x, ' ');

    Position bottom = *bottomPos(snake);
    
    if (tail->x == bottom.x && tail->y == bottom.y) {
        tail->dir = bottom.dir;
        popCorner();
    } 

    switch(tail->dir) {
        case KEY_RIGHT:
            if(head->x < mx) tail->x++;
            break;
        case KEY_UP:
            if(head->y > 0) tail->y--;
            break;
        case KEY_LEFT:
            if(head->x > 0) tail->x--;
            break;
        case KEY_DOWN:
            if(head->y < my) tail->y++;
            break;
    }
}
