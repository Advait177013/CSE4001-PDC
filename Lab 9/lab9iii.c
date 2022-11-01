#include<mpi.h>
#include<omp.h>
#include<stdio.h>
#include<math.h>
#include<stdbool.h>
//#include<iostream>

//using namespace std;

struct myb{
    bool i;
    bool j;
    bool k;
    bool l;
};

#define btoa(x) ((x)?"1":"0")

bool chk_circuit(struct myb boo)
{
    // let circuit be ab(and) + cd(or) -> (not)ab(or)cd
    bool ans = !(boo.i&&boo.j)||(boo.k||boo.l);
    printf("%s %s %s %s %s\n", btoa(boo.i), btoa(boo.j), btoa(boo.k), btoa(boo.l), btoa(ans));
    return ans;
}

int main()
{
    //MPI circuit satisfiability

    struct myb full_arr[16];
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<2; j++)
        {
            for(int k=0; k<2; k++)
            {
                for(int l=0; l<2; l++)
                {
                    struct myb bol;
                    bol.i=(bool)i;
                    bol.j=(bool)j;
                    bol.k=(bool)k;
                    bol.l=(bool)l;
                    int val=i*8+j*4+k*2+l*1;
                    full_arr[val]=bol;
                }
            }
        }
    }
    
    bool answers[16];
    int global_sols;
    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if(world_rank==0)
        printf("A B C D op\n");
    int sols=0;
    for(int i=0; i<world_size/*<16?world_size:15*/; i++)
    {
        int o=world_rank+world_size*i;
        answers[o]=chk_circuit(full_arr[o]);
        if(answers[o])
            sols++;
    }

    MPI_Reduce(&sols, &global_sols, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(world_rank==0)
        printf("\nGlobal solutions is : %d\n", global_sols);

    MPI_Finalize();
    return 0;
}