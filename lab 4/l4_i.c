#include<stdio.h>
#include<omp.h>

int main()
{
    int a=10;
    int b=5;
    int c=0;

    #pragma omp parallel sections firstprivate(c)
    {
        printf("\nvalue of c : %d", c);
        #pragma omp section
        {
            c=a+b;
            printf("\nthread num : %d", omp_get_thread_num());
            printf("\nvalue of c : %d", c);
        }

        #pragma omp section
        {
            c=a-b;
            printf("\nthread num : %d", omp_get_thread_num());
            printf("\nvalue of c : %d", c);
        }

    }     
    printf("\nvalue of c after end : %d", c); 

}
