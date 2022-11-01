#include<mpi.h>
#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<stdbool.h>
//#include<iostream>

//using namespace std;

#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

void printmat(int arr[100][100], int world_size)
{
    printf("na");
    for(int i=0; i<world_size; i++)
        printf("\tC%d", i);
    printf("\n");
    for(int i=0; i<world_size; i++)
    {
        printf("R%d", i);
        for(int j=0; j<world_size; j++)
        {
            printf("\t%d", arr[i][j]);
        }
        printf("\n");
    }
}

void addtomat(int arr[100][100], int world_size, int world_rank, int to_add)
{  
    printf("executing from inside world - %d\n", world_rank);
    for(int j=0; j<world_size; j++)
    {
        arr[world_rank][j]+=to_add;
    }
}

int main()
{
    //MPI add number to matrix

    MPI_Status status;

    MPI_Init(NULL, NULL);
    
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int full_arr[100][100];
    int added_arr[100][100];
    int to_add;       

    srand(time(NULL));
    for(int i=0; i<world_size-1; i++)
    {
        for(int j=0; j<world_size-1; j++)
        {
            int x=rand()%100;
            full_arr[i][j]=x;
            added_arr[i][j]=0;
        }
    }

    to_add=rand()%10+1;
    if(world_rank==0)
    {
        printmat(full_arr, world_size-1);
        printf("Adding a randomly generated number to the matrix - %d\n", to_add);
        double start = MPI_Wtime();

        for(int dest=1; dest<=world_size-1; dest++)
        {
            MPI_Send(&full_arr[dest-1][0], (world_size-1), MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
        }

        for(int source=1; source<=world_size-1; source++)
        {
            MPI_Recv(&added_arr[source-1][0], (world_size-1), MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
        }
        
        printf("Finished Adding\n");
        printmat(added_arr, world_size-1);
        double finish = MPI_Wtime();
        printf("done in %f seconds\n", finish-start);
    }

    if(world_rank!=0)
    {
        MPI_Recv(&full_arr, (world_size-1), MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        for(int i=0; i<world_size; i++)
        {
            added_arr[world_rank-1][i]=full_arr[world_rank-1][i]+to_add;
        }
        MPI_Send(&added_arr[world_rank-1][0], (world_size-1), MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}