#include<stdio.h>
#include<omp.h>

int main()
{
int x=0;
#pragma omp parallel for shared(x)

for(int i=0; i<omp_get_num_threads(); i++)
{
	//#pragma omp critical
	x=x+1;
	printf("Thread Num : %d - X value %d\n", omp_get_thread_num(), x);
}
		
}
