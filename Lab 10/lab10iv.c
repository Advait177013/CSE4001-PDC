#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<unistd.h>
#include<math.h>

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
    double buffer[100]={0};
    double calc[2]={0};
    double a=0;
    
    if(world_rank==0)
    {
        srand(time(NULL));
        for(int i=0; i<world_size*3; i++)
        {
            buffer[i]=((float)rand()/(float)(RAND_MAX))*100;
            //printf("%f\n", buffer[i]);
        }
            

        for(int dest=1; dest<world_size; dest++)
        {
            //sends to all
            MPI_Send(&buffer[3*(dest-1)], 2, MPI_DOUBLE, dest, msg_tag, MPI_COMM_WORLD);
        }
        for(int source=1; source<world_size; source++)
        {
            //receives from all
            MPI_Recv(&buffer[3*(source-1)+2], 1, MPI_DOUBLE, source, msg_tag, MPI_COMM_WORLD, &status);
            printf("result from world %d of numbers %f and %f is %f\n", source, buffer[3*(source-1)], buffer[3*(source-1)+1], buffer[3*(source-1)+2]);
        }
        double finish = MPI_Wtime();
        printf("time taken to finish the full set of broadcast %f\n", finish-start);
    }
    else
    {
        MPI_Recv(&calc[0], 2, MPI_DOUBLE, MASTER, msg_tag, MPI_COMM_WORLD, &status);
        if(world_rank==1)
            a=calc[0]+calc[1];
        if(world_rank==2)
            a=calc[0]-calc[1];
        if(world_rank==3)
            a=calc[0]*calc[1];
        if(world_rank==4)
            a=calc[0]/calc[1];
        if(world_rank==5)
            a=pow(calc[0], calc[1]);
        if(world_rank==6)
            a=log(calc[0])/log(calc[1]);
        if(world_rank==7)
            a=fmod(calc[0], calc[1]);
        MPI_Send(&a, 1, MPI_DOUBLE, MASTER, msg_tag, MPI_COMM_WORLD);
        //double finish = MPI_Wtime();
        //printf("done in %f seconds in world %d. My result is %f\n", finish-start, world_rank, a);
    }
    
    MPI_Finalize();
    
    return 0;
}
