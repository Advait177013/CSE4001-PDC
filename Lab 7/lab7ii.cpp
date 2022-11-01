#include<omp.h>
#include<stdio.h>
#include<time.h>

using namespace std;

int main()
{
    int n=500;
    int a[n];
    int b[n];
    for(int i=0; i<n; i++)
    {
        a[i]=3^(i%3)+i%3;
        b[i]=4^(i%4)+i%4;
    }
    int c[n];

    clock_t t;
    t=clock();

    #pragma omp unroll full
    for(int i=0; i<n; i++)
    {
        c[i]=a[i]+b[i];
        #pragma omp barrier
    }
    

    t=clock()-t;
    double tt=((double)t/CLOCKS_PER_SEC);
    printf("\ntime taken for computation (barrier): %f\n", tt);
}