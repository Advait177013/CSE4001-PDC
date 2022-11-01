//implementing MPI_Bcast with MPI_Send/MPI_Recv
//then comparing with MPI_Bcast

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
    
    if(world_rank==0)
    {
        srand(time(NULL));
        for(int i=0; i<world_size; i++)
            buffer[i]=rand()%100;

        for(int dest=1; dest<world_size; dest++)
        {
            //sends to all (including itself in MPI_Bcast)
            MPI_Send(&buffer[0], world_size, MPI_INT, dest, msg_tag, MPI_COMM_WORLD);
        }
        double finish = MPI_Wtime();
        printf("time taken to finish the full set of scuffcast %f\n", finish-start);
        printf("master here, starting real MPI_Bcast. Check by value updating to 0\n");
        for(int i=0; i<world_size; i++)
                buffer[i]=0;
    }
    else
    {
        MPI_Recv(&buffer[0], world_size, MPI_INT, MASTER, msg_tag, MPI_COMM_WORLD, &status);
        double finish = MPI_Wtime();
        printf("done in %f seconds in world %d, received the scuffcast. My number is %d\n", finish-start, world_rank, buffer[world_rank]);
    }
    
    double start1=MPI_Wtime();

    MPI_Bcast(&buffer[0], world_size, MPI_INT, MASTER, MPI_COMM_WORLD);

    double finish1=MPI_Wtime();

    printf("i am world %d, my buffer value is %d, and i took %f secs\n", world_rank, buffer[world_rank], finish1-start1);
    MPI_Finalize();
    
    return 0;

}