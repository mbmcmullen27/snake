#include "main.h"

int main() {
    initscr();
    curs_set(0);
    refresh();

    Game game;
    game.kubeEnabled = false;
    getmaxyx(stdscr,game.my,game.mx);

    keypad(stdscr, TRUE);
    noecho();

    srand(time(NULL));

    mainMenu(&game);
    startGame(&game);

    int key;

    for(game.ticks = 0;;game.ticks++) {
        moveHead(&game, game.snake->head->dir);
        if(game.ticks>5 && (game.ticks > game.lastHit+3)) 
            moveTail(&game);

        refresh();
        usleep(150000); 

        mvprintw(game.my-2,game.mx/2, "ticks: %i lastHit: %i   ",game.ticks, game.lastHit);
        printDebug(&game);
        
        key = getch();
        if (key != ERR) game.snake->head->dir = key;
    }
}

void mainMenu(Game* game) {
    nodelay(stdscr, false); 
    WINDOW *win = newwin(13,60,((game->my-4)/2)-5,((game->mx-4)/2)-25);

    box(win, '|', '=');
    mvwprintw(win, 3,22, "!!! WARNING !!!");
    mvwprintw(win, 5,10, "This game reads your local ~/.kube/config");
    mvwprintw(win, 6,8, "and deploys things to the selected cluster...");
    mvwprintw(win, 8,9, "Do you want to continue with this enabled?");
    mvwprintw(win, 10,29, "Y/N/Q");
    touchwin(win);
    wrefresh(win);

    int selection;
    do {
        selection = getch();
        if (selection == 'y') {
            game->kubeEnabled=true;
            clear();
            refresh();
        } else if (selection == 'n') {
            game->kubeEnabled=false;
            clear();
            refresh();
        } else if (selection == 'q') {
            clear();
            refresh();
            endwin();
            exit(0);
        }
    } while (selection != 'y' && selection != 'n');
}