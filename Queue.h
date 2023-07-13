#ifndef _CODA_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <iostream>
#include <signal.h>
#include <iomanip>
#define LIMIT 4
#define _CODA_H

class Queue{
int shm_id;
int* shm_ptr;
int* queue[LIMIT];
int* tail = 0;
int* i = 0;
public:

Queue();
void gainMemory();
bool enqueue(int newElement);
int dequeue();
int printHead();
void printQueue();
};
#endif
