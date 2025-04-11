#ifndef _LIB_FILE_H
#define _LIB_FILE_H


#ifndef _REENTRANT
#define _RENTRANT
#endif

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <float.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <sys/time.h>
#include <errno.h>

#define MAX_WORKER 64
#define MAX_ARRAY_LENGTH 8388608 // 8MB
#define ONE_MEGA 1.0E06

#ifndef MIN
#define MIN(x,y) (((x)<(y))?(x):(y))
#endif

#ifndef MAX
#define MAX(x,y) (((x)>(y))?(x):(y))
#endif

void *compute_pi(void *arg);
void *Temp(void *tmp);
double get_clk();

#endif