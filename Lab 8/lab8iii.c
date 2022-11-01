#include<mpi.h>
#include<stdio.h>

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
    if(world_rank==0)
        printf("Hello from processor %s, master, of %d procs\n", processor_name, world_size);
    else
        printf("Hello from processor %s, slave, among %d procs\n", processor_name, (world_size-1));

    
    MPI_Finalize();
    return 0;
}