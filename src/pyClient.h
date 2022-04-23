#ifndef pyClient_h
#define pyClient_h

#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void createPod();
void deletePods();
int executeScript(char* script);

#endif