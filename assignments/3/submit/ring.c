//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University
//------------------------------------------------------------------------- 

// A simple demo program of MPI file input.
//
// Usage: 
//   linux> mpirun -n <#procs> fileIn data.txt
//
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int size, rank, buf[2], message, tag =201;
    MPI_File fh;
    MPI_Status st;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int nextRank = (rank + 1) % size;
    int prevRank = (rank + size - 1) % size;
    if (rank == 0) {
        message = size;
        MPI_Send(&message,1,MPI_INT,nextRank,tag,MPI_COMM_WORLD);
        printf("Process %d sent to %d value: %d\n", rank, nextRank, message);
    }

    while (1)
    {
        MPI_Recv(&message, 1, MPI_INT, prevRank, tag, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        if (message != 0) {
            --message;
            printf("Process %d decremented value: %d\n", rank, message);
        }

        MPI_Send(&message, 1, MPI_INT, nextRank, tag, MPI_COMM_WORLD);

        if (0 == message) {
            printf("Process %d exiting\n", rank);
            break;
        }
    }

    MPI_Finalize();
    return 0;
}
