#include "main.h"

int main() {
    initscr();
    curs_set(0);
    refresh();

    Game game;
#ifdef DEBUG
    game.yoffset = 4;
#else
    game.yoffset = 1;
#endif
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

        printDebug(&game);
        
        key = getch();
        if (key != ERR) game.snake->head->dir = key;
    }
}

void mainMenu(Game* game) {
    nodelay(stdscr, false); 
    if (game->my > 13 && game->mx > 70) {
        int ypos = ((game->my - 4) / 2) - 5;
        int xpos = ((game->mx - 4) / 2) - 25;

        WINDOW *win = newwin(13,60,ypos,xpos);

        box(win, '|', '=');
        mvwprintw(win, 3,22, "!!! WARNING !!!");
        mvwprintw(win, 5,10, "This game reads your local ~/.kube/config");
        mvwprintw(win, 6,8, "and deploys things to the selected cluster...");
        mvwprintw(win, 8,9, "Do you want to continue with this enabled?");
        mvwprintw(win, 10,29, "Y/N/Q");

        touchwin(win);
        wrefresh(win);
    } else {
        WINDOW *win = newwin(5,50,0,0);

        mvwprintw(win, 0,0, "!!! WARNING !!!");
        mvwprintw(win, 1,0, "This game reads your local ~/.kube/config");
        mvwprintw(win, 2,0, "and deploys things to the selected cluster...");
        mvwprintw(win, 3,0, "Do you want to continue with this enabled?");
        mvwprintw(win, 4,0, "Y/N/Q");

        touchwin(win);
        wrefresh(win);
    }

    int selection;
    do {
        selection = getch();
        switch(selection) {
            case'Y' :
                selection = 'y';
            case'y' :
                game->kubeEnabled=true;
                clear();
                refresh();
                break;
            case 'N' :
                selection = 'n';
            case 'n' :
                game->kubeEnabled=false;
                clear();
                refresh();
                break;
            case 'Q':
                selection = 'q';
            case 'q':
                clear();
                refresh();
                endwin();
                exit(0);
        }
#ifdef DEBUG
        mvprintw(game->my-1,game->mx/2,"kube enabled: %i", game->kubeEnabled);
#endif
    } while (selection != 'y' && selection != 'n');
}