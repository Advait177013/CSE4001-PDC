#include<stdio.h>
#include<omp.h>
#include<time.h>

int main()
{
    int n=0;
    int mynum=10000;
    printf("\nenter value for upper limit of prime check: ");
    scanf("%d", &mynum);
    clock_t t;
    t=clock();

    #pragma omp parallel for reduction(+:n)
    for(int i=2; i<mynum; i++)
    {
        int p=1;
        for(int j=2; j<(i/2)+1; j++)
        {
            if(i%j==0)
            {
                p=0;
                break;
            }
        }
        n+=p;
    }
    t=clock()-t;
    printf("\nThere are %d primes between 1 and %d", n, mynum);
    double tt=((double)t/CLOCKS_PER_SEC);
    printf("\ntime taken for computation : %f\n", tt);
}
