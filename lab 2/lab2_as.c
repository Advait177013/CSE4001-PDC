#include <omp.h>
#include<stdio.h>

int main ()

{ 
	int a=2, b=2;
    printf("\na=%d", a);
    printf("\nb=%d", b);
    printf("\nParallel adding subtracting");

	#pragma omp parallel

	{ 
        int i=omp_get_thread_num();
        if(i%2)
        {
            a=a+b;
            printf("\na=%d", a);
        }
        else
        {
            b=b-a;
            printf("\nb=%d", b);
        }
        
	}


}
