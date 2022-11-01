#include<mpi.h>
#include<omp.h>
#include<stdio.h>
#include<math.h>

int main()
{
    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    for(int i=0; i<10; i++)
    {
        if(world_rank==0)
            printf("Master Output : 1/%d\n", (int)(pow(2, i)));
        else
            printf("Worker Output : %d\n", (int)(pow(2, i)));
    }

    MPI_Finalize();
    return 0;
}