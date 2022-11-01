#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

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
    
    if(world_rank==0)
    {
        srand(time(NULL));
        int buffer[100]={0};
        for(int dest=1; dest<world_size; dest++)
        {
            buffer[dest]=rand()%100;
            if(dest>4)
            {
                MPI_Send(&buffer[dest], 1, MPI_INT, dest, msg_tag, MPI_COMM_WORLD);
            }
            else
            {
                MPI_Isend(&buffer[dest], 1, MPI_INT, dest, msg_tag, MPI_COMM_WORLD, &request);
                MPI_Wait(&request, &status);
            }
            
        }
    }
    else
    {
        int bufrecv;
        if(world_rank>4)
        {
            MPI_Recv(&bufrecv, 1, MPI_INT, MASTER, msg_tag, MPI_COMM_WORLD, &status);
        }
        else
        {
            MPI_Irecv(&bufrecv, 1, MPI_INT, MASTER, msg_tag, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
        }
        double finish = MPI_Wtime();
        printf("done in %f seconds in world %d, received the number %d\n", finish-start, world_rank, bufrecv);
    }



    MPI_Finalize();
    
    return 0;

}