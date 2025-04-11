#include <stdio.h>
#include <omp.h>

int main(){

    omp_set_num_threads(4);
    int data = 42;
    int flag = 0;

    #pragma omp parallel
    {
        
        #pragma omp for nowait
        for (int i = 0; i < 100; i++)
        {
            data++;
        }

        #pragma omp single
        {
        printf("Data = %d\n", data);
        data=42;
        flag=1;
        #pragma omp flush(data,flag)
        }



        #pragma omp for
        for (int i = 0; i < 100; i++)
        {
            #pragma omp flush(flag,data)
            while (flag==0)
            {
                #pragma omp flush(flag,data)
            }
            
            
            #pragma omp critical
            {
                data++;
            }
            
        }
        #pragma omp single
        printf("Data = %d\n", data);

    }
}