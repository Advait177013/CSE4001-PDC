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
int msg_tag=1729;
int msg=1;

int nextPrime(int msg);
bool isPrime(int msg);

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
    
    if(world_rank!=0)
        {
            //worlds 1-8 start receiving msg from previous
            MPI_Recv(&msg, 1, MPI_INT, world_rank-1, msg_tag, MPI_COMM_WORLD, &status);
            printf("world %d received prime number %d from world %d\n", world_rank, msg, world_rank-1);
        }
    
    msg=nextPrime(msg);

    MPI_Send(&msg, 1, MPI_INT, (world_rank+1)%world_size, msg_tag, MPI_COMM_WORLD);
    printf("world %d send prime number %d to world %d\n", world_rank, msg, (world_rank+1)%world_size);
    if(world_rank==0)
    {
        MPI_Recv(&msg, 1, MPI_INT, world_size-1, msg_tag, MPI_COMM_WORLD, &status);
        printf("world %d received prime number %d from world %d\n", world_rank, msg, world_size-1);
    }

    double finish=MPI_Wtime();
    printf("world %d finished in time %f\n", world_rank, finish-start);

    MPI_Finalize();
    
    return 0;

}

bool isPrime(int n)
{
    if(n<=1)
        return false;
    if(n<=3)
        return true;

    if(n%2 == 0 || n%3 == 0)
        return false;
    
    for(int i=5; i*i<=n; i=i+6)
        if(n%i==0 || n%(i+2)==0)
            return false;
    
    return true;
}

int nextPrime(int msg)
{
    if(msg <=1)
        return 2;
    
    int prime=msg;
    bool found=false;

    while(!found)
    {
        prime++;
        if(isPrime(prime))
            found=true;
    }

    return prime;
}