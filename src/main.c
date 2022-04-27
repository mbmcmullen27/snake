#include "main.h"

int main() {
    Game game;

    initscr();
    curs_set(0);
    refresh();
    getmaxyx(stdscr,game.my,game.mx);

    keypad(stdscr, TRUE);
    noecho();

    srand(time(NULL));

    startGame(&game);

    int key;

    for(game.ticks = 0;;game.ticks++) {
        moveHead(&game);
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