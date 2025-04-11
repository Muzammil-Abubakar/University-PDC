#include "lib_file.c"

long int points_per_thread;
long int inside_circle = 0;  // Shared counter for points inside the circle


// Function executed by each thread
void *compute_pi(void *arg) {
    long int local_count = 0;
    long int tid = (long int) arg;
    unsigned int seed = time(NULL) ^ tid; // Unique seed per thread

    global_barrier(tid, &T1);
    for (int i = 0; i < points_per_thread; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        
        if (x * x + y * y <= 1.0) {
            local_count++;  // Point is inside the quarter-circle
        }
    }
    
    pthread_mutex_lock(&lock);
    inside_circle += local_count;  // Update global count safely
    pthread_mutex_unlock(&lock);
    //global_barrier(tid, &T2);
    T2 = get_clk();

    threadsData[tid] = T2 - T1;

    printf("Thread %ld completed %ld points in %.2lf seconds\n", tid, points_per_thread, T2 - T1);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Syntax: %s <numProcesors> <numPoints>\nExiting Program...\n", argv[0]);
        exit(1);
    }

    numWorker = atoi(argv[1]);
    if (numWorker < 1 || numWorker > MAX_WORKER) {
        fprintf(stderr, "Number of workers have to be between 1 and %d\nExiting Program...\n", MAX_WORKER);
        exit(1);
    }

    char *endptr;
    long int numPoints = strtol(argv[2], &endptr, 10);
    if (numPoints < 1 || numPoints > LONG_MAX) {
        fprintf(stderr, "Number of Points have to be between 1 and %ld\nExiting Program...\n", LONG_MAX);
        exit(1);
    }
 
    pthread_t *threads;
    threads = (pthread_t*)malloc(sizeof(pthread_t)*numWorker);
    pthread_attr_t attr;
 
    threadsData = (double*)malloc(sizeof(double)*numWorker);

    //initialization
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&go, NULL);	

	//set global thread attributes
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    points_per_thread = numPoints / numWorker;

    // Create threads
    for (long int i = 0; i < numWorker; i++) {
        threadsData[i] = 0.0;
        pthread_create(&threads[i], &attr, compute_pi, (void *) i);
    }
    
    // Join threads
    for (int i = 0; i < numWorker; i++) {
        pthread_join(threads[i], NULL);
    }
    
    //double min=DBL_MAX, max=0.0, sum=0.0, avg=0.0;
    //for (int i = 0; i < numWorker; i++) {
    //    MAX(max,threadsData[i]);
    //    MIN(min,threadsData[i]);
    //    sum +=threadsData[i];
    //}
    //avg = sum/numWorker;
    //printf("MAX:%lf\nMIN:%lf\nMEAN:%lf\n", max, min, avg);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&go);
    pthread_attr_destroy(&attr);	
    free(threads);
    free(threadsData);

    // Compute final estimation of Pi
    double pi_estimate = 4.0 * inside_circle / numPoints;
    printf("Estimated Pi: %f\n", pi_estimate);
    
    return 0;
}