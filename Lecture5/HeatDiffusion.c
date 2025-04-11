#include "lib_file.c"
#define X_SIZE 1000
#define Y_SIZE 1000
#define Cx 0.125
#define Cy 0.11
#define CMin 200
#define CMax 800
#define TIMESTEPS 4000
#define PRINT_STEPS 200

float (*old)[Y_SIZE];
float (*new)[Y_SIZE];

int Count;
int Remainder;

void* Temp(void* arg) {
    long int threadId = (long int)arg;
    int halfX = X_SIZE / 2; // Calculate only for the left half
    int start = threadId * (halfX - 1) / numWorker + (threadId < Remainder ? threadId : Remainder);
    int end = start + (halfX - 1) / numWorker - 1 + (threadId < Remainder ? 1 : 0);

    for (int block = 1; block <= TIMESTEPS; block++) {
        for (int j = start; j <= end; j++) {
            for (int k = 1; k < Y_SIZE - 1; k++) {
                if (j > 0 && j < halfX) {
                    new[j][k] = old[j][k] + Cx * (old[j + 1][k] + old[j - 1][k] - 2 * old[j][k]) + Cy * (old[j][k + 1] + old[j][k - 1] - 2 * old[j][k]);
                    // Mirror the calculated value to the right half
                    new[X_SIZE - 1 - j][k] = new[j][k];
                }
            }
        }

        barrier();
        if (threadId == 0) {
            float (*temp)[Y_SIZE] = old;
            old = new;
            new = temp;

            if (block % PRINT_STEPS == 0) {
                printf("\nThe Temperature values at points:[1,1]=%f [150,150]=%f [400,400]=%f [500,500]=%f [750,750]=%f [900,900]=%f", old[1][1], old[150][150], old[400][400], old[500][500], old[750][750], old[900][900]);
            }
        }
        barrier();
    }
    return NULL;
}


int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Syntax: %s <numProcesors>\nExiting Program...\n", argv[0]);
        exit(1);
    }

    numWorker = atoi(argv[1]);
    if (numWorker < 1 || numWorker > MAX_WORKER) {
        fprintf(stderr, "Number of workers have to be between 1 and %d\nExiting Program...\n", MAX_WORKER);
        exit(1);
    }

    old = malloc(X_SIZE * sizeof(*old));
    new = malloc(X_SIZE * sizeof(*new));
    for (int x = 0; x < X_SIZE; x++) {
        for (int y = 0; y < Y_SIZE; y++) {
            old[x][y] = (x >= CMin - 1 && x <= CMax - 1 && y >= CMin - 1 && y <= CMax - 1) ? 500.0 : 0.0;
        }
    }

    pthread_t *threads;
    threads = (pthread_t*)malloc(sizeof(pthread_t)*numWorker);
    pthread_attr_t attr;
 
    //initialization
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&go, NULL);	

	//set global thread attributes
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    Count = (X_SIZE - 1) / numWorker;
    Remainder = (X_SIZE - 1) % numWorker;

    T1 = get_clk();

    for (long int i = 0; i < numWorker; i++) {
        pthread_create(&threads[i], &attr, Temp, (void*)i);
    }

    for (int i = 0; i < numWorker; i++) {
        pthread_join(threads[i], NULL);
    }

    T2 = get_clk();
    printf("\nTime = %lf nanoseconds (%.9f sec)\n", (T2-T1)*1000000000.0, T2-T1);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&go);
    pthread_attr_destroy(&attr);	
    free(old);
    free(new);
    free(threads);

    return 0;
}