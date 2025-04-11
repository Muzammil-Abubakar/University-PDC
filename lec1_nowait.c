#include <stdio.h>
#include <unistd.h>     // for sleep() and usleep()
#include <omp.h>

int main() {
    // Enable nested parallelism if needed (optional)

    omp_set_num_threads(10);
    omp_set_nested(1);

    printf("=== OpenMP nowait Demonstration ===\n\n");

    #pragma omp parallel
    {
        int tid = omp_get_thread_num()+1;

        // ---------- FIRST PARALLEL LOOP (with implicit barrier) ----------
        #pragma omp for
        for (int i = 0; i < 10; i++) {
            printf("[Loop 1] Thread %d processing iteration %d\n", tid, i);
            usleep(100000);  // Simulate work (100ms)
        }

        // This message will only appear after all threads complete Loop 1
        printf("[After Loop 1] Thread %d reached the implicit barrier.\n", tid);

        // ---------- SECOND PARALLEL LOOP (with nowait) ----------
        #pragma omp for nowait
        for (int i = 0; i < 10; i++) {
            printf("[Loop 2 - nowait] Thread %d processing iteration %d\n", tid, i);
            usleep(150000);  // Simulate more work (150ms)
        }

        // Because of nowait, threads do not wait here
        printf("[After Loop 2 - nowait] Thread %d continues immediately without waiting.\n", tid);

        // ---------- THIRD PARALLEL LOOP (with implicit barrier again) ----------
        #pragma omp for
        for (int i = 0; i < 10; i++) {
            printf("[Loop 3] Thread %d processing iteration %d\n", tid, i);
            usleep(100000);  // Simulate work
        }

        printf("[After Loop 3] Thread %d reached the implicit barrier.\n", tid);
    }

    // Final message outside the parallel region
    printf("\n=== Explanation ===\n");
    printf("1. Loop 1 and Loop 3 use the default behavior — they include an implicit barrier.\n");
    printf("   -> All threads must finish before any move on.\n");
    printf("2. Loop 2 uses 'nowait', which removes the barrier.\n");
    printf("   -> Threads that finish early go ahead to the next section without waiting.\n");
    printf("This is useful for performance when the next task doesn't depend on all threads finishing the previous one.\n");

    return 0;
}
