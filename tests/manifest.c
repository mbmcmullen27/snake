#include "manifest.h"

Manifest* initManifest(char* name) {
    Manifest* res = malloc(sizeof(Manifest));
    res->name = name;
    res->next = NULL;
    return res;
}

void printList(Manifest* list) {
    for(int i = 0; list != NULL; list = list->next) {
        printf("%i-%s\n",i,list->name);
        i++;
    }
}

Manifest* readDirectory(char* dirName){
    DIR *d;
    Manifest* head;
    struct dirent *dir;
    d = opendir(dirName);
    if (d) {
        head = initManifest(readdir(d)->d_name);
        Manifest* current = head;
        while ((dir = readdir(d)) != NULL ) {
            char* name = dir->d_name;
            int len = strlen(name);
            if(!strcmp(&name[len-3],"yml") || !strcmp(&name[len-4],"yaml")){
                current->next = initManifest(name);
                current = current->next;
            }
        }
        closedir(d);
    }
    return head;
}