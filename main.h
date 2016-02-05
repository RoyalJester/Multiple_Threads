#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>

#ifndef MAIN_H
#define MAIN_H

typedef struct{
	int produced;
	int t;
	int *consumed;
	int *total_p;
	int *q;
	int *p_action;
	int *c_action;
	int *maxsize;
	int *candycount;
	int *f;
	int *e;
	sem_t *barrier;
	sem_t *mutex;
	sem_t *empty;
	sem_t *full;
} DATA;

#endif