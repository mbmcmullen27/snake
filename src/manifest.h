#ifndef manifest_h
#define manifest_h

#include <dirent.h> 
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Manifest {
    struct Manifest* next;
    char* name;
} Manifest;

Manifest* readDirectory(char*);
Manifest* initManifest(char*);
void printList(Manifest*);

#endif