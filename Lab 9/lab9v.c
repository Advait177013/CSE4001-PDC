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

void printarr(int arr[100])
{
    for(int i=0; i<100; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int findmax(int arr[100], int start_span, int end_span)
{
    int loc_mx=0;
    for(int span=start_span; span<end_span; span++)
    {
        //printf("currently checking %d\n", arr[span]);
        loc_mx=arr[span]>loc_mx?arr[span]:loc_mx;
    }
    //printf("findmax ended successfully\n");
    return loc_mx;
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

    //char processor_name[MPI_MAX_PROCESSOR_NAME];
    //int name_len;
    //MPI_Get_processor_name(processor_name, &name_len);
    int full_arr[100];
    int span_chart[100]={0};
    int global_max_scuff=0;
    //int global_max_reduce = 0;
    int local_max=0; 

    srand(time(NULL));
    for(int i=0; i<100; i++)
    {
        int x=rand()%500;
        full_arr[i]=x;
    }

    int extra=100%(world_size-1);
    int base_span;
    int start_span=0;
    int end_span;
    //int dummy_sync = 0;
    if(world_rank==0)
    {
        printarr(full_arr);
        printf("Getting the Maximum of the Array\n");
        double start = MPI_Wtime();

        //sending
        for(int dest=1; dest<=world_size-1; dest++)
        {
            if(dest<=extra)
            {
                base_span = 1 + (100-extra)/(world_size-1);
            }
            else
            {
                base_span = (100-extra)/(world_size-1);
            }
            end_span = start_span + base_span - 1;
            MPI_Send(&start_span, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&end_span, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            //printf("sent span %d to %d to world rank %d\n", start_span, end_span, dest);
            start_span += base_span;
            //printf("send succesful to %d\n", dest);
        }

        //receiving
        for(int source=1; source<=world_size-1; source++)
        {
            MPI_Recv(&local_max, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
            //printf("received successfully from %d\n", source);
            if(local_max>global_max_scuff)
                global_max_scuff=local_max;
        }
    
        //MPI_Reduce(&local_max, &global_max_reduce, 1, MPI_INT, MPI_MAX, MASTER, MPI_COMM_WORLD);
        printf("Finished Finding Max (Scuff) - %d\n", global_max_scuff);
        //printf("Finished Finding Max (Reduce) - %d\n", global_max_reduce);
        double finish = MPI_Wtime();
        printf("done in %f seconds\n", finish-start);

    }

    if(world_rank!=0)
    {
        MPI_Recv(&span_chart[2*world_rank], 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&span_chart[2*world_rank+1], 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
        local_max = findmax(full_arr, span_chart[2*world_rank], span_chart[2*world_rank+1]);
        //printf("local max from world %d is %d\n", world_rank, local_max);
        MPI_Send(&local_max, 1, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        //printf("sucessfull send from world %d\n", world_rank);

    }

    MPI_Finalize();
    return 0;
}