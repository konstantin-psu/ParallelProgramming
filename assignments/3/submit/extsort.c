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
#define DEBUG 0             // DEBUG = 1 enables debug prints
#define INITSIZE 128        // Initial bucket size
#define COLLECTTIME 0       // Measure performance

int arraySize = 0;          // Input array size
char * dataFile;            // Input file name


// Bucket structure
// This approach removes any assumption about data distribution
struct Bucket {
    int size;        // Current number of elements in the bucket
    int maxSize;     // Size of the elements array
    int *elements;   // array to be sorted
    int pivot;       // all elements of the array are less then pivot
};


// Swap two array elements (borrowed from the provided sequential quicksort routine)
//
void swap(int *array, int i, int j) {
    if (i == j) return;
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

// Allocate buckets (size is equal to the size of participating processes, size of the world)
struct Bucket *initBucketsArray(int size) {
    struct Bucket *bucket = (struct Bucket *) malloc(size * sizeof(struct Bucket)); // Allocate bucket
    int i;
    int *arr;
    for (i = 0; i < size; i++) {
        bucket[i].size = 0;                             // Initially empty
        bucket[i].maxSize = INITSIZE;                   // Set max size
        arr = (int *) malloc(INITSIZE * sizeof(int));   // Create and save Initial array
        bucket[i].elements = arr;                       //
        bucket[i].pivot = -1;                           // Some phony pivot value
    }
    return bucket;                                      // return created buckets array
}

// Destroy buckets
void freeBuckets(struct Bucket *b, int size) {
    int i;
    for (i = 0; i < size; i++) {
        free(b[i].elements);
    }
    free(b);
}

// Add an item to the bucket
// If bucket is full, reallocate memory and save the item
// otherwise save the item
struct Bucket *add(struct Bucket *bucket, int item) {
    if (!(bucket->size < bucket->maxSize)) { // bucket is full -> expand
        bucket->maxSize = bucket->maxSize * 2;
        bucket->elements = realloc(bucket->elements, bucket->maxSize * sizeof(int));
    }

    bucket->elements[bucket->size] = item;
    bucket->size++;
    return bucket;
}

// Helper method used for debugging
// Print content of all the buckets
// Intented to use only by the host thread
void printBucketList(struct Bucket *bucket, int size) {
    int i = 0, j;
    for (i = 0; i < size; i++) {
        printf("Bucket %d content:\n", i);
        for (j = 0; j < bucket[i].size; j++) {
            printf("%d ", bucket[i].elements[j]);
        }
        printf("\n");

    }
}

// Helper method used for debugging
// Print and array content
void printArray(int *arr, int arraySize) {
    int i = 0;
    for (i = 0; i < arraySize; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


// Scan through the initial array and distribute it's data to
//  appropriate buckets
void fillBuckets(int *array, struct Bucket *buckets, int size) {
    int i = 0, j = 0, pI = 10;          // pI - pivot interval
    int added = 0;                      // flag

    // save desired pivots
    for (i = 0; i < size - 1; i++) {
        buckets[i].pivot = array[pI];
        pI += 10;
    }

    // distribute data
    for (i = 0; i < arraySize; i++) {
        for (j = 0; j < size - 1; j++) {
            if (array[i] < buckets[j].pivot) {
                add(buckets + j, array[i]);
                added = 1;
                break;
            }
        }
        // if item wasn't added, drop it to the last bucket
        if (!added) {
            add(buckets + (size - 1), array[i]);
        }
        added = 0;
    }
}

// Bubble sort for the base cases (borrowed form provided sequential sort)
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

// Read provided dataFile
int *readData(char *fname, MPI_File *fh, MPI_Status *st, int *buf) {
    int i = 0;
    MPI_Offset size;
    MPI_File_open(MPI_COMM_SELF, fname, MPI_MODE_RDONLY, MPI_INFO_NULL, fh);   // Open the file, only for the local thread
    MPI_File_get_size(*fh, &size);                                             // Read the file size
    buf = (int *) malloc(size);                                                // Create array big enough to hold data
    // set starting offset for the read operation -- not needed
    // MPI_File_set_view(*fh, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
    // read the entire array at once
    MPI_File_read(*fh, buf, size / 4, MPI_INT, st);

    arraySize = size / 4;
    MPI_File_close(fh);
    return buf;
}

// quicksort wrapper
void quickSortBucket(int * bucket, int size, int rank) {
    quicksort(bucket, 0, size - 1);
    if (DEBUG) {
        printf("Sorted by rank %d, size %d\n", rank, size);
        printArray(bucket, size);
    }
}

// Save sorted data by all threads
void saveData(int *bucket, int size, int msize,  int prevSize) {

    MPI_File fh;
    MPI_Status st;

    char fname[100]; // Output file name
    snprintf(fname, sizeof(char)*100, "%s%d_%s", "sorted", size, dataFile);
    int *buf = bucket;
    // construct output file name
    MPI_File_open(MPI_COMM_SELF, fname, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
    // set starting offset for the write operation
    // MPI_File_set_view(fh, bucket[0]* 4, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
    MPI_File_set_view(fh, prevSize * 4, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
    // write four integers to the file
    MPI_File_write(fh, buf, msize, MPI_INT, &st);
    MPI_File_close(&fh);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Datafile name is required\n");
        return 0;
    }


    int size = 1, rank = 0, *buf, tag = 201;
    double startTime=0, initTime=0, sortStartTime=0, sortEndTime=0, saveTime=0, endTime=0, readTime=0, sendTime=0, pivotSort, bucketsInit; // all timer variables
    int msize = 0,prevSize = 0, i = 0;
    MPI_File fh;
    MPI_Status st;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) {
        printf("At least two threads needed\n");
        return 0;
    }

    struct Bucket *bucket;
    dataFile = argv[1];
    if (rank == 0) {
        if (COLLECTTIME) startTime = MPI_Wtime();
        int toSort = 10 * size;
        buf = readData(argv[1], &fh, &st, buf);
        if(DEBUG) {
            printf("Presorted array");
            printArray(buf, arraySize);
        }
        if (COLLECTTIME) readTime = MPI_Wtime();
        quicksort(buf, 0, toSort - 1);
        if (COLLECTTIME) pivotSort = MPI_Wtime();
        bucket = initBucketsArray(size);
        if (COLLECTTIME) bucketsInit = MPI_Wtime();

        fillBuckets(buf, bucket, size);
        if (COLLECTTIME) initTime = MPI_Wtime();
        prevSize = 0;
        for (i = 1; i < size ;i++)
        {
            MPI_Send(&bucket[i].size,1,MPI_INT, i, tag, MPI_COMM_WORLD);                        // Send size of the bucket
            MPI_Send(bucket[i].elements, bucket[i].size, MPI_INT, i, tag, MPI_COMM_WORLD);      // Send the bucket
            MPI_Send(&prevSize,1,MPI_INT, i, tag, MPI_COMM_WORLD);
            prevSize = prevSize + bucket[i].size;
        }
        msize = bucket[0].size;
        free(buf);
        buf = bucket[0].elements;
        prevSize = 0;
        if (COLLECTTIME) sendTime = MPI_Wtime();
    }

    if (rank != 0) {
        MPI_Recv(&msize, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        buf = (int *) malloc(msize*sizeof(int));
        MPI_Recv(buf, msize, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&prevSize, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (DEBUG) {
            printf("Rank %d, received array of size %d at location %d\n", rank , msize, prevSize);
            printArray(buf, msize);
        }
    }


    if (COLLECTTIME) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (!rank) {
            sortStartTime = MPI_Wtime();
        }
    }
    quickSortBucket(buf, msize,  rank);

    if (COLLECTTIME) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (!rank) {
            sortEndTime = MPI_Wtime();
        }
    }

    saveData(buf, size, msize, prevSize);

    if (COLLECTTIME) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (!rank) {
            saveTime = MPI_Wtime();
        }
    }


    if (rank == 0) {
        freeBuckets(bucket, size);
    } else {
        free(buf);
    }

    if (COLLECTTIME) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0) {
            endTime = MPI_Wtime();
            printf("Time total time %f sortTime %f, saveTime %f, initTime %f, bucketsInit %f, initSort %f, readTime %f send %f, IOtotal %f\n",
                   endTime - startTime, sortEndTime - sortStartTime, saveTime - sortEndTime, initTime - bucketsInit, bucketsInit - pivotSort, pivotSort - readTime,
                   readTime - startTime, sendTime - initTime, (readTime - startTime) + (saveTime - sortEndTime));
            printf("%f, %f, %f, %f, %f, %f, %f %f, %f\n",
                   endTime - startTime, sortEndTime - sortStartTime, saveTime - sortEndTime, initTime - bucketsInit, bucketsInit - pivotSort, pivotSort - readTime,
                   readTime - startTime, sendTime - initTime, (readTime - startTime) + (saveTime - sortEndTime));
        }
    }
    MPI_Finalize();
    return 0;
}
