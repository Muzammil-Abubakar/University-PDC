#include "lib_file.h"
#include <linux/time.h>

pthread_mutex_t lock;
pthread_cond_t go;

double *threadsData;
int numArrived = 0;
int numWorker;
double T1, T2;

void barrier(){
	pthread_mutex_lock(&lock);
	numArrived++;
		if (numArrived == numWorker){
			numArrived = 0;
			pthread_cond_broadcast(&go);
		}
		else 
			pthread_cond_wait(&go, &lock);
		pthread_mutex_unlock(&lock);
}

void global_barrier(int id, double *barrierTime){
	id = id; //to remove the warnings
	pthread_mutex_lock(&lock);
	numArrived++;
	if (numArrived == numWorker) {
		numArrived =0;
		*barrierTime = get_clk();
		pthread_cond_broadcast(&go);
	}
	else 
		pthread_cond_wait(&go, &lock);
	pthread_mutex_unlock(&lock);
}

double get_clk(){
	double t;
	struct timespec cur_time;
	clock_gettime(CLOCK_REALTIME, &cur_time);
	t = (double)(cur_time.tv_sec + (cur_time.tv_nsec/1000000000.0));
	return t;
}