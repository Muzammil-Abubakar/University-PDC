#include <stdio.h>
#include <omp.h>

int main() {
    int sum = 5;

    #pragma omp parallel for default(none) reduction(+:sum) schedule(dynamic,5)
    for (int i = 1; i <= 10; i++) {
        sum ++;
        printf("The current value of sum is: %d with thread: %d\n",sum,omp_get_thread_num());
    }

    printf("Total sum: %d\n", sum);
    return 0;
}
