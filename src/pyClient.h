#ifndef pyClient_h
#define pyClient_h

#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct {
    int out;
    int err;
    int save_out;
    int save_err;
    wchar_t* program;
} PyClient;

void createPod();
void deletePods();
void applyManifest(char*);
int executeScript(char*, int, wchar_t**);
int initPyClient(PyClient*, char* script); 
void closePyClient(PyClient*);

#endif