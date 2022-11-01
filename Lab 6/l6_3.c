#include<stdio.h>
#include<omp.h>
#include<time.h>
#define N 100000
#define CHUNKSIZE 100
int main ()
{
int i, chunk;
float a[N], b[N], c[N];
/* Some initializations */ 
for (i=0; i < N; i++)
	a[i] = b[i] = i * 1.0;
	
chunk = CHUNKSIZE;
//clock_t start = clock();
double start1 = omp_get_wtime();
#pragma omp parallel for shared(a,b,c,chunk) private(i) schedule(dynamic/*static*/,chunk)
for (i=0; i < N; i++)
{
	c[i] = a[i] + b[i];
}
//clock_t end = clock();
//double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
double end1 = omp_get_wtime();
 
printf("for loop took %f seconds to execute \n", end1-start1);
}
