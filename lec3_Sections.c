#include <stdio.h>
#include <omp.h>

int main() {
    // Set the number of threads to use
    omp_set_num_threads(3);

    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                printf("Section 1: Thread %d is working on Task 1\n", omp_get_thread_num());
            }

            #pragma omp section
            {
                printf("Section 2: Thread %d is working on Task 2\n", omp_get_thread_num());
            }

            #pragma omp section
            {
                printf("Section 3: Thread %d is working on Task 3\n", omp_get_thread_num());
            }
        }
    }

    return 0;
}
