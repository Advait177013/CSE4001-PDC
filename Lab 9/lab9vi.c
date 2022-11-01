//recursion pseudo
//place first queen at [rank][0]
//for X in 0:8
    //for Y in 0:8
        //if X,Y == 0
            //place queen
            //invalidate(x, y) - add 1 to all squares
            //recurse
            //remove queen
            //validate(x, y) - sub 1 from all squares
//if placed==8
    //print matrix
/*************************************
 * after more consideration, above method is kinda unfeasible, because of
 * 0s (blank possible spaces for queen) appearing at the end and 
 * messing with stuff later
 * Going back to normal n-Queens
 * **********************************/

/*****************
 * Normal n-Queens algo - 
 * isSafe(x, y, board) -> check diagonals and row/cols of (x, y) of board
 * Recursive bool nQueen(board, current col)
 * -if(col>=boardLength) -> return True (board is full with N Queens)
 * -for(row space in the given column)
 * -if isSafe(row space, col, board)
 * -{
 * -    board(row space, col) = 1
 * -    if(recursive nQueens(board, next col))
 * -        return true
 * -    board(row space, col) = 0 -> backtrack
 * -}
 * 
 * 
 * main()
 * {
 *  if(recursive nQeeun(board, world_rank))
 *      print(solution)
 *  else
 *      print("no sol found")
 * }
 * ***************/
#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2


int myQueens[8][8][8] = {0};
//ideally only need one 8x8 matrix, not 8 sets. but im too lazy to recode
int whether_soln[8]={0};
int synchronizer=0;

bool UpperqueenSoln(int boardNum, int col);
bool queenSoln(int boardNum, int col);
bool checkCols(int boardNum, int row, int col);
bool checkSafe(int boardNum, int row, int col);
void printsols();
void printarray(int boardNum);


int main()
{


    MPI_Status status;

    MPI_Init(NULL, NULL);
    
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if(world_rank==MASTER)
    {
        double start = MPI_Wtime();
        for(int dest=1; dest<world_size; dest++)
        {
            MPI_Send(&myQueens[dest-1][0][0], 64, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);//synchronize and signal start of the program
        }
        printf("printing the first recursive solutions out of 92\n");
        printf("(including rotations and mirrors) of 8 queens for\n");
        printf("first queen placed on diff locations across base row\n\n");

        for(int source=1; source<world_size; source++)
        {
            MPI_Recv(&myQueens[source-1][0][0], 64, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
            MPI_Recv(&whether_soln[source-1], 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);//synchronize receive and whether a solution has been found or not
        }
        printsols();
        double finish = MPI_Wtime();
        printf("done in %f seconds\n", finish-start);
    }
    else
    {
        MPI_Recv(&myQueens[world_rank-1][0][0], 64, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);//sync start
        if(UpperqueenSoln(world_rank-1, world_rank-1))
        {
            whether_soln[world_rank-1]=1;
        }
        MPI_Send(&myQueens[world_rank-1][0][0], 64, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(&whether_soln[world_rank-1], 1, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);//sync
    }

    MPI_Finalize();
    
    return 0;
}

bool UpperqueenSoln(int boardNum, int col)
{
    myQueens[boardNum][0][col]=1;
    return(queenSoln(boardNum, 0));
}

bool queenSoln(int boardNum, int col)
{
    if(col >= 8)
        return true;
    for(int i=0; i<8; i++)
    {
        if(checkSafe(boardNum, i, col))
        {
            if(checkCols(boardNum, i, col))
            {
                if(queenSoln(boardNum, col+1))
                    return true;
            }
            else{   
                myQueens[boardNum][i][col] = 1;
                if(queenSoln(boardNum, col+1))
                    return true;
                /*if(boardNum==7)    
                    printarray(boardNum);
                */myQueens[boardNum][i][col] = 0;   
            }
        }
    }
    return false;
}

bool checkCols(int boardNum, int row, int col)
{
    //across the col set by og queen call
    for(int j=0; j<8; j++)
        if(myQueens[boardNum][j][col])
            return true;

    return false;
}

bool checkSafe(int boardNum, int row, int col)
{
    //edge case check
    if(boardNum==7 && col==7)
        return true;

    //across the row
    for(int i=0; i<8; i++)
        if(myQueens[boardNum][row][i])
            return false;
    
    //upper left diag
    for(int i=row, j=col; i>=0 && j>=0; i--, j--)
        if(myQueens[boardNum][i][j])
            return false;
    
    //lower right diag
    for(int i=row, j=col; i<=7 && j<=7; i++, j++)
        if(myQueens[boardNum][i][j])
            return false;

    //lower left diag
    for(int i=row, j=col; i < 8 && j>=0; i++, j--)
        if(myQueens[boardNum][i][j])
            return false;
    
    //upper right diag
    for(int i=row, j=col; i>=0 && j<8; i--, j++)
        if(myQueens[boardNum][i][j])
            return false;

    return true;
}

void printsols()
{
    for(int i=0; i<8; i++)
    {
        if(whether_soln[i])
            {
                printf("Got Solution at world %d\n\n-----------\n", i);
                printarray(i);
                printf("\n----------\n\n");
            }
        else
            printf("no solution from world %d\n\n----------\n\n", i);
    }
}

void printarray(int boardNum)
{
    printf("\n");
    for(int i=0; i<8; i++)
    {
        printf("\n");
        for(int j=0; j<8; j++)
        {
            printf("%d ", myQueens[boardNum][i][j]);
        }
    }
    printf("\n");
}