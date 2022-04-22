#include "snake.h"

Corner* top;
Corner* bottom;

void initSnake(int my) {
    Snake snake;
    Position* head = initPosition();
    Position* tail = initPosition();
    top = bottom = initCorner(0,head->y,KEY_RIGHT);
    bottom->visited = true;

    head->y = my/2;
    tail->y = my/2;
    head->dir = KEY_RIGHT;

    snake.head = head;
    snake.tail = tail;
    return snake;
}

void freeCorner() {
    Corner* temp = bottom;
    bottom=bottom->next;
    free(temp->position);
    free(temp);
}

void pushCorner(Snake* snake) {
    Position* pos = snake->head;
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

void moveTail(Snake* snake) {
    Position* tail = snake->tail;
    Position* head = snake->head;

    mvaddch(tail->y, tail->x, ' ');
    
    if (tail->x == bottom->position->x && tail->y == bottom->position->y) {
        tail->dir = bottom->position->dir;
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