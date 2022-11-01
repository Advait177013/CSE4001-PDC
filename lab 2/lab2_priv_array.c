#include <omp.h>
#include<stdio.h>

int main ()

{ 
	int a[4], b[4], c[4];

	/* Fork a team of threads with each thread having a private tid variable */

	#pragma omp parallel private(a, b, c)

	{ /* Obtain and print thread id */
        int i=omp_get_thread_num();
        a[i]=i;
        b[i]=i*2;
        c[i]=a[i]+b[i];
        printf("\nThread num %d, a=%d, b=%d, c=%d", i, a[i], b[i], c[i]);

	}

    for(int i=0; i<4; i++)
    {
        printf("\nOutside Parallel : a=%d, b=%d, c=%d\n", a[i], b[i], c[i]);
    }

	/* All threads join master thread and terminate */

}
