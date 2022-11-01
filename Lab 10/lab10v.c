#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<unistd.h>
#include<string.h>

int msg_tag=1729;
int msg=1;

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
    char str[10];
    
    //ensure server and client rudimentary execution
    if(world_size!=2) //server side termination
        {
            if(world_rank==0) //ensures error reason always prints
                printf("Please ensure only 2 processes are running\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }


    //server code
    if(world_rank==1)
    {
        char helloCmpr[10]={'H', 'e', 'l', 'l', 'o', '\0'};
        //ensure connection until connection break
        while(true)
        {
            MPI_Recv(&msg, 1, MPI_INT, 0, msg_tag, MPI_COMM_WORLD, &status);
            //printf("server side, msg value received - %d\n", msg);
            //code
            if(msg!=1)
                break;
            else
            {
                MPI_Recv(&str, 100, MPI_CHAR, 0, msg_tag, MPI_COMM_WORLD, &status);
                printf("%s\n", str);
                if(!strcmp(str, helloCmpr))
                    printf("WORLD");
                else
                    printf("woops");
            }
            //printf("ubroken server\n");
        }
        
        printf("Thank you for using server. Connection ended\n");
    }
    
    //client code, we can only use scanf in world_rank 0 
    if(world_rank==0)
    {
        //ensure connection until break statement given
        while(true)
        {
            printf("Enter 1 if you want to talk to server\nEnter anything else if you want to end contact\nenter value : \n");

            scanf("%d", &msg);
            //printf("sending msg value of - %d\n", msg);
            MPI_Send(&msg, 1, MPI_INT, 1, msg_tag, MPI_COMM_WORLD);
            //code for taking input of client arguments
            if(msg!=1)
                break;
            else
            {
                printf("Enter what you want to do : \n");
                scanf("%s", str);
                printf("read statement - %s\n", str);
                MPI_Send(&str, 100, MPI_CHAR, 1, msg_tag, MPI_COMM_WORLD);
            }
            //printf("unbroken client, repeat\n");
        }
    }

    MPI_Finalize();
    
    return 0;
}

/*********
 * MPI_Isend(&buffer[dest], 1, MPI_INT, dest, msg_tag, MPI_COMM_WORLD, &request);
 * MPI_Wait(&request, &status);
 * MPI_Irecv(&bufrecv, 1, MPI_INT, MASTER, msg_tag, MPI_COMM_WORLD, &request);
 * MPI_Wait(&request, &status);
 * ******/
                