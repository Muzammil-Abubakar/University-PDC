#include <stdio.h>
#include <omp.h>

int count = 0; // Master thread 
#pragma omp threadprivate(count)
int shared_count = 0;
int parallel_count=0;


int main() {
    omp_set_num_threads(8);
    int n = 40;

    #pragma omp parallel private(parallel_count)
    {
        parallel_count=0;
        // Experiment with different scheduling options: static, dynamic, guided
        #pragma omp for schedule(static,5)
        for (int i = 0; i < n; i++) {
            count++;
            parallel_count++;
            shared_count++;
            if (omp_get_thread_num()==0)
            {
                printf("The Value of Parallel Count is: %d\n", parallel_count);
            }
            
        }
        printf("By Thread: %d\t Count: %d AND Parallel Count: %d\n", omp_get_thread_num(),count, parallel_count);

        #pragma omp barrier
        #pragma omp single
        {
            printf("The value of Shared Count: %d\n", shared_count);
        }

    }
    return 0;
}