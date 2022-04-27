#include "game.h"

int main() {
    initscr();
    curs_set(0);
    refresh();

    Game game;
    getmaxyx(stdscr,game.my,game.mx);

    keypad(stdscr, TRUE);
    noecho();

    srand(time(NULL));

    startGame(&game);

    int key;

    for(game.ticks = 0;;game.ticks++) {
        moveHead(&game, game.snake->head->dir);
        if(game.ticks>5 && (game.ticks > game.lastHit+3)) 
            moveTail(&game);

        refresh();
        usleep(150000); 

        mvprintw(game.my-2,game.mx/2, "ticks: %i lastHit: %i   ",game.ticks, game.lastHit);
        printDebug(game.snake, game.mx, game.my);
        
        key = getch();
        if (key != ERR) game.snake->head->dir = key;
    }
}

void startGame(Game* game){
    nodelay(stdscr, TRUE); 

    // draw border
    move(0,0);
    hline('-', game->mx-1);
    vline('|',game->my-4);
    addch(A_ALTCHARSET | ACS_ULCORNER);
    move(game->my-4,0);
    hline('-', game->mx-1);
    addch(A_ALTCHARSET | ACS_LLCORNER);
    move(0,game->mx-1);
    vline('|',game->my-4);
    addch(A_ALTCHARSET | ACS_URCORNER);
    mvaddch(game->my-4, game->mx-1,A_ALTCHARSET | ACS_LRCORNER);

    game->snake = initSnake(game->my);
    game->lastHit = 0;
    game->ticks = 0;
    mvaddch(game->my/2,game->mx/2,'*');
}

void collect(Game* game) {
    if (game->lastHit > game->ticks - 3) {
        game->lastHit = game->lastHit + 3;
    } else {
        game->lastHit = game->ticks;
    }

    int x, y;
    char character;
    do {
        x = rand() % (game->mx-2);
        y = rand() % (game->my-4);
        character = mvinch(y+1,x+1) & A_CHARTEXT;
    } while (!isspace(character));
    mvprintw(game->my-1,game->mx/2,"mx: %i my: %i food: (%i,%i)     ",game->mx,game->my,x+1,y+1);
    mvaddch(y+1,x+1, '*');
}

void gameOver(Game* game) {
    freeCorners(game->snake);
    nodelay(stdscr, false); 

    WINDOW *win = newwin(11,20,((game->my-4)/2)-5,((game->mx-4)/2)-5);

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
            startGame(game);
        } else if (selection == 'n') {
            clear();
            refresh();
            endwin();
            exit(0);
        }
    } while (selection != 'y' && selection != 'n');
}

bool check(Game* game) {
    Position* head = game->snake->head;
    printDir(head->dir,"dir", game->my-3, game->mx/2);
    printDir(head->prev,"prev", game->my-3, (game->mx/2)+12);
    char current = mvinch(head->y, head->x) & A_CHARTEXT;
    if ((current) == '*'){
        collect(game);
        createPod();
    } else if (!isspace(current)){
        gameOver(game);
        return false; // unreachable
    } 

    return true;
}

void moveRight(Game* game){
    Snake* snake = game->snake;
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
    if(check(game)) {
        mvaddch(head->y, head->x,'>');
        head->prev = KEY_RIGHT;
    }
}

void moveLeft(Game* game) {
    Snake* snake = game->snake;
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
    if (check(game)) {
        mvaddch(head->y,head->x,'<');
        head->prev = KEY_LEFT;
    }
}

void moveUp(Game* game) {
    Snake* snake = game->snake;
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
    if (check(game)) {
        mvaddch(head->y, head->x, '^');
        head->prev = KEY_UP;
    }
}

void moveDown(Game* game) {
    Snake* snake = game->snake;
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
    if (check(game)) {
        mvaddch(head->y,head->x,'v');
        head->prev = KEY_DOWN;
    }
}

void moveHead(Game* game, int dir) {
    Snake* snake = game->snake;
    Position* head = snake->head;
    switch(dir) {
        case 'd':
        case 'l':
            head->dir=KEY_RIGHT;
        case KEY_RIGHT: // east
            if(head->prev == KEY_LEFT) {
                moveLeft(game);
            } else {
                moveRight(game);
            }
            break;
        case 'w':
        case 'k':
            head->dir=KEY_UP;
        case KEY_UP: // north
            if(head->prev == KEY_DOWN) {
                moveDown(game);
            } else {
                moveUp(game);
            }
            break;
        case 'a':
        case 'h':
            head->dir=KEY_LEFT;
        case KEY_LEFT: // west
            if(head->prev == KEY_RIGHT) {
                moveRight(game);
            } else {
                moveLeft(game);
            }
            break;
        case 's':
        case 'j':
            head->dir=KEY_DOWN;
        case KEY_DOWN: // south
            if(head->prev == KEY_UP) {
                moveUp(game);
            } else {
                moveDown(game);
            }
            break;
        default:
            moveHead(game, snake->head->prev);
            break;
    }
}

void moveTail(Game* game) {
    Snake* snake = game->snake;
    Position* tail = snake->tail;
    Position* head = snake->head;
    Position* bottom = snake->bottom->position;

    mvaddch(tail->y, tail->x, ' ');

    if (tail->x == bottom->x && tail->y == bottom->y) {
        tail->dir = bottom->dir;
        popCorner(snake);
    } 

    switch(tail->dir) {
        case KEY_RIGHT:
            if(head->x < game->mx) tail->x++;
            break;
        case KEY_UP:
            if(head->y > 0) tail->y--;
            break;
        case KEY_LEFT:
            if(head->x > 0) tail->x--;
            break;
        case KEY_DOWN:
            if(head->y < game->my) tail->y++;
            break;
    }
}
