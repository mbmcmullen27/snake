#include "../src/position.h"
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h> 
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "manifest.h"

int main() {
    Manifest* list = readDirectory("bundle");
    for(int i = 0; list != NULL; list = list->next) {
        printf("%i-%s\n",i,list->name);
        i++;
    }
}

int positionTest(){
    Position** list = malloc(sizeof(Position) * 8);
    int i;
    for(i=0;i<8;i++){
        list[i] = initPosition(); 
    }

    for(i=0;i<8;i++){
        list[i]->x = list[i]->y = i;
        list[i]->dir = list[i]->x + list[i]->y;
        printf("[%i] - x:%i y:%i d:%i\n", i, list[i]->x, list[i]->y, list[i]->dir);
    }

    list = realloc(list, 20);

    for(i=8;i<20;i++){
        list[i] = initPosition(); 
    }

    for(i=8;i<20;i++){
        list[i]->x = list[i]->y = i;
        list[i]->dir = list[i]->x + list[i]->y;
        printf("[%i] - x:%i y:%i d:%i\n", i, list[i]->x, list[i]->y, list[i]->dir);
    }

    return 0;
}

Position* initPosition() {
    Position* res = malloc(sizeof(Position));    
    res->x=0;
    res->y=0;
    res->dir=0;
    res->prev=0;
    return res;
}
