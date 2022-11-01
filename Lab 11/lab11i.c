
#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<unistd.h>

#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

int main()
{

    MPI_Status status;
    MPI_Request request = MPI_REQUEST_NULL;

    MPI_Init(NULL, NULL);
    
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    double start = MPI_Wtime();

    int msg_tag=1729;
    int buffer[100]={0};
    if(world_rank==MASTER)
    {
        for(int i=0; i<100; i++)
            buffer[i]=i;
    }
    double start1=MPI_Wtime();

    MPI_Bcast(buffer, 100, MPI_INT, MASTER, MPI_COMM_WORLD);

    double finish1=MPI_Wtime();

    printf("i am world %d, my buffer value is %d, and i took %f secs\n", world_rank, buffer[world_rank], finish1-start1);
    MPI_Finalize();
    
    return 0;

}