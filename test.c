#include "position.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    Position* list[8];
    int i;
    for(i=0;i<8;i++){
        list[i] = initPosition(); 
    }

    for(i=0;i<8;i++){
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

void resize(Position** list) {
        
}
