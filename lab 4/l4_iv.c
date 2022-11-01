#include<stdio.h>
#include<omp.h>
#include<time.h>
#include<stdbool.h>

int grid[4][4]={{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

bool possible(int x, int y, int n)
{

    for(int i=0; i<4; i++)
    {
        if(grid[x][i]==n)
            return false;
    }

    for(int i=0; i<4; i++)
    {
        if(grid[i][y]==n)
            return false;
    }
    int x0=(x/2)*2;
    int y0=(y/2)*2;

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<2; j++)
        {
            if(grid[x0+i][y0+i]==n)
                return false;
        }
    }

    return true;    
}

void dispmat()
{
    printf("\n");
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
}

void solvesud()
{
    
    for(int x=0; x<4; x++)
    {
        for(int y=0; y<4; y++)
        {
            if(grid[x][y]==0)
            {
                //#pragma omp parallel for
                for(int k=1; k<5; k++)
                {
                    if(possible(x, y, k))
                    {
                        grid[x][y]=k;
                        solvesud();
                        grid[x][y]=0;
                    }
                }
                return;
            }
        }
    }
    dispmat();
}

void takevals()
{
    for(int i=0; i<4; i++)
        scanf("%d %d %d %d", &grid[i][0], &grid[i][1], &grid[i][2], &grid[i][3]);
}

int main()
{
    clock_t t;
    t=clock();

    printf("enter values for 2x2x4 sudoku : \n");
    takevals();
    solvesud();
    t=clock()-t;
    double tt=((double)t/CLOCKS_PER_SEC);
    printf("\ntime taken for computation : %f\n", tt);
}
