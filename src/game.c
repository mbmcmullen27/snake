#include "game.h"

void startGame(Game* game){
    nodelay(stdscr, TRUE); 

    drawBorder(game);

    game->dir = opendir("bundle");

    game->snake = initSnake(game->my);
    game->lastHit = 0;
    game->ticks = 0;
    mvaddch(game->my/2,game->mx/2,'*');
}

void drawBorder(Game* game) {
    int height = game->my - game->yoffset;
    int width = game->mx - 1;

    move(0,0);
    hline('-', width);
    vline('|',height);
    addch(A_ALTCHARSET | ACS_ULCORNER);
    move(height,0);
    hline('-', width);
    addch(A_ALTCHARSET | ACS_LLCORNER);
    move(0,width);
    vline('|',height);
    addch(A_ALTCHARSET | ACS_URCORNER);
    mvaddch(height, width, A_ALTCHARSET | ACS_LRCORNER);
}

char* nextManifest(Game* game) {
    if (game->dir) {
        struct dirent *dir;
        if ((dir = readdir(game->dir)) != NULL ) {
            char* name = dir->d_name;
            int len = strlen(name);
            if(!strcmp(&name[len-3],"yml") || !strcmp(&name[len-4],"yaml")){
                return name;
            } else {
                return nextManifest(game);
            }
        }
    }
    return "NONE";
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
        y = rand() % (game->my - game->yoffset);
        character = mvinch(y+1,x+1) & A_CHARTEXT;
    } while (!isspace(character));

    char* nextFile = nextManifest(game);
    if(strcmp(nextFile, "NONE")) applyManifest(nextFile);

#ifdef DEBUG
    drawBorder(game);
    mvprintw(game->my-1,game->mx/2,"mx: %i my: %i food: (%i,%i)     ",game->mx,game->my,x+1,y+1);
    mvprintw(0,2," deploying target: %s ", nextFile);
#endif

    mvaddch(y+1,x+1, '*');
}

void gameOver(Game* game) {
    freeCorners(game->snake);
    nodelay(stdscr, false); 

    int ypos = ((game->my - game->yoffset) / 2) - 5;
    int xpos = ((game->mx - 4) / 2) - 5;

    WINDOW *win = newwin(11,20,ypos,xpos);

    if(game->kubeEnabled) deletePods();

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
        if(game->kubeEnabled) createPod();
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

void printDebug(Game* game) {
#ifdef DEBUG
    Snake* snake = game->snake;
    printPos("top", snake->top->position, game->my-3, 0);
    printPos("bottom", snake->bottom->position, game->my-2, 0);
    printPos("tail", snake->tail, game->my-1, 0);
    mvprintw(game->my-2,game->mx/2, "ticks: %i lastHit: %i   ",game->ticks, game->lastHit);
#endif
}
