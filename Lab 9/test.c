#include<mpi.h>
#include<omp.h>
#include<stdio.h>
#include<math.h>
//#include<rand.h>
#include<stdbool.h>

int main()
    {
    int num_elements_per_proc = 10;
    int rand1_nums[10] = {2, 4, 3, 1, 5, 4, 6, 7, 5, 3};

    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Sum the numbers locally
    int local_sum = 0;
    int i;
    for (i = 0; i < num_elements_per_proc; i++) {
    local_sum += rand1_nums[i];
    }

    // Print the random numbers on each process
    printf("Local sum for process %d - %d, avg = %d\n",
        world_rank, local_sum, local_sum / num_elements_per_proc);

    // Reduce all of the local sums into the global sum
    int global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0,
            MPI_COMM_WORLD);

    // Print the result
    if (world_rank == 0) {
    printf("Total sum = %d, avg = %d\n", global_sum,
            global_sum / (world_size * num_elements_per_proc));
    }

    MPI_Finalize();

    return 0;
}