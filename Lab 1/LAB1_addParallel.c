#include<omp.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int a[8]={1, 2, 3, 4, 5, 6, 7, 8};
	int b[8]={8, 7, 6, 5, 4, 3, 2, 1};
	int c[8];
	
	#pragma omp parallel
	{
		for(int i=0; i<8; i++)
		{
			c[i]=a[i]+b[i];
			printf("%d\n", c[i]);				
		}

	}	
	printf("test");
	return 0;
}
