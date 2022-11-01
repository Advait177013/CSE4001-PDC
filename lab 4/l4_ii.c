#include<stdio.h>
#include<omp.h>

int main()
{
	int a[40]={1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
    int b[40]={5, 6, 7, 8, 5, 6, 7, 8, 5, 6, 7, 8, 5, 6, 7, 8, 5, 6, 7, 8, 5, 6, 7, 8, 5, 6, 7, 8, 5, 6, 7, 8, 5, 6, 7, 8, 5, 6, 7, 8};
    int c[40]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            #pragma omp parallel for
            for(int i=0; i<20; i++)
            {
                c[2*i]+=a[2*i]+b[2*i];
            }
        }

        #pragma omp section
        {
            #pragma omp parallel for
            for(int i=0; i<20; i++)
            {
                c[2*i+1]+=a[2*i+1]*b[2*i+1];
            }
        }

        #pragma omp section
        {
            #pragma omp parallel for
            for(int i=0; i<40; i++)
                c[i]-=b[i];
        }

    }      

    for(int i=0; i<40; i++)
        printf("\nfinal sum = %d", c[i]); 
}
