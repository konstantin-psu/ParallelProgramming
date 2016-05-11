//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University
//------------------------------------------------------------------------- 

// A simple demo program of MPI file input.
//
// Usage: 
//   linux> mpirun -n <#procs> extsort <dataInFile>
//
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define MINSIZE   10        // threshold for switching to bubblesort
#define DEBUG 1


struct Bucket {
    int size;
    int *elements;
};

// Swap two array elements
//
void swap(int *array, int i, int j) {
    if (i == j) return;
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

// Bubble sort for the base cases
//
void bubblesort(int *array, int low, int high) {
    if (low >= high)
        return;
    for (int i = low; i <= high; i++)
        for (int j = i + 1; j <= high; j++)
            if (array[i] > array[j])
                swap(array, i, j);
}

// Pick an arbitrary element as pivot. Rearrange array
// elements into [smaller one, pivot, larger ones].
// Return pivot's index.
//
int partition(int *array, int low, int high) {
    int pivot = array[high];    // use highest element as pivot
    int middle = low;
    for (int i = low; i < high; i++)
        if (array[i] < pivot) {
            swap(array, i, middle);
            middle++;
        }
    swap(array, high, middle);
    return middle;
}

// QuickSort an array range
//
void quicksort(int *array, int low, int high) {
    if (high - low < MINSIZE) {
        bubblesort(array, low, high);
        return;
    }
    int middle = partition(array, low, high);
    if (low < middle)
        quicksort(array, low, middle - 1);
    if (middle < high)
        quicksort(array, middle + 1, high);
}

int readData(char *fname, MPI_File *fh, MPI_Status *st, int *buf) {
    int i = 0;
    MPI_Offset size;
    if (DEBUG) printf("Opening file %s\n", fname);
    MPI_File_open(MPI_COMM_WORLD, fname, MPI_MODE_RDONLY, MPI_INFO_NULL, fh);
    if (DEBUG) printf("Getting file size\n");
    MPI_File_get_size(*fh, &size);
    if (DEBUG) printf("File size is %d\n", size);
    buf = (int *) malloc(size);
    // set starting offset for the read operation
    if (DEBUG) printf("Setting view\n");
    MPI_File_set_view(*fh, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
    // read two integers from the file
    if (DEBUG) printf("Reading file\n");
    MPI_File_read(*fh, buf, size / 4, MPI_INT, st);

    for (i = 0; i < size / 4; i++) {
        printf("%d %d\n", i, buf[i]);
    }
    if (DEBUG) printf("Size of int %d\n", sizeof(int));
    return size / 4;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Datafile name is required\n");
        return 0;
    }


    int size, rank, *buf, message, tag = 201;
    MPI_File fh;
    MPI_Status st;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int nextRank = (rank + 1) % size;
    int prevRank = (rank + size - 1) % size;
    if (rank == 0) {
        int bufSize = 0;
        int toSort = 10 * size;
        /*
         * 1. Read all data
         * 2. sort first 10*size elements, and selects elements at positions 10, 20, 10(size-1)
         *      as pivots
         * 3. Partition data into P buckets
         */
        // quicksort(array, 0, N-1);
        message = size;
//        MPI_Send(&message,1,MPI_INT,nextRank,tag,MPI_COMM_WORLD);
        printf("Process %d reading %s\n", rank, argv[1]);
        bufSize = readData(argv[1], &fh, &st, buf);
        quicksort(buf, 0, toSort - 1);
//        printf("Process %d sent to %d value: %d\n", rank, nextRank, message);
    }

//    while (1)
//    {
//        MPI_Recv(&message, 1, MPI_INT, prevRank, tag, MPI_COMM_WORLD,
//                 MPI_STATUS_IGNORE);
//
//        if (message != 0) {
//            --message;
//            printf("Process %d decremented value: %d\n", rank, message);
//        }
//
//        MPI_Send(&message, 1, MPI_INT, nextRank, tag, MPI_COMM_WORLD);
//
//        if (0 == message) {
//            printf("Process %d exiting\n", rank);
//            break;
//        }
//    }

    MPI_Finalize();
    return 0;
}
