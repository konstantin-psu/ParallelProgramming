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
#define DEBUG 0
#define INITSIZE 2
int arraySize = 0;
char * dataFile;


struct Bucket {
    int size;
    int maxSize;
    int *elements;
    int pivot;
};

// Swap two array elements
//
void swap(int *array, int i, int j) {
    if (i == j) return;
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

struct Bucket *initBucketList(int size) {
    struct Bucket *bucket = (struct Bucket *) malloc(size * sizeof(struct Bucket));
    int i;
    int *arr;
    for (i = 0; i < size; i++) {
        bucket[i].size = 0;
        bucket[i].maxSize = INITSIZE;
        arr = (int *) malloc(INITSIZE * sizeof(int));
        bucket[i].elements = arr;
        bucket[i].pivot = -1;
    }
    return bucket;
}

void freeBuckets(struct Bucket *b, int size) {
    int i;
    for (i = 0; i < size; i++) {
        free(b[i].elements);
    }
    free(b);
}

struct Bucket *add(struct Bucket *bucket, int item) {
    if (!(bucket->size < bucket->maxSize)) { // bucket is full -> expand
        if (DEBUG) printf("Bucket is too small expanding\n");
        bucket->maxSize = bucket->maxSize * 2;
        bucket->elements = realloc(bucket->elements, bucket->maxSize * sizeof(int));
    }

    bucket->elements[bucket->size] = item;
    bucket->size++;
    return bucket;
}

void printBucketList(struct Bucket *bucket, int size) {
    int i = 0, j;
    if (DEBUG)
    for (i = 0; i < size; i++) {
        printf("Bucket %d content:\n", i);
        for (j = 0; j < bucket[i].size; j++) {
            printf("%d ", bucket[i].elements[j]);
        }
        printf("\n");

    }
}

void printArray(int *arr, int arraySize) {
    int i = 0;
    if (DEBUG)
    for (i = 0; i < arraySize; i++) {
        printf("%d ", arr[i]);
    }
    if (DEBUG) printf("\n");
}

void fillBuckets(int *array, struct Bucket *buckets, int size) {

    int i = 0, j = 0, pI = 10;
    int added = 0;
    // 1. Add pivots to buckets
    if (size == 1) { //special case
        if (DEBUG) printf("Special case only single thread is participating\n");
        for (i = 0; i < arraySize; i++) {
            add(buckets, array[i]);
        }
        return;
    }
    for (i = 0; i < size - 1; i++) {
        buckets[i].pivot = array[pI];
        pI += 10;
    }
    for (i = 0; i < arraySize; i++) {
        for (j = 0; j < size - 1; j++) {
            if (array[i] < buckets[j].pivot) {
                add(buckets + j, array[i]);
                added = 1;
                break;
            }
        }
        if (!added) {
            add(buckets + (size - 1), array[i]);
        }
        added = 0;
    }
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

int *readData(char *fname, MPI_File *fh, MPI_Status *st, int *buf) {
    int i = 0;
    MPI_Offset size;
    if (DEBUG) printf("Opening file %s\n", fname);
    MPI_File_open(MPI_COMM_SELF, fname, MPI_MODE_RDONLY, MPI_INFO_NULL, fh);
    if (DEBUG) printf("Getting file size\n");
    MPI_File_get_size(*fh, &size);
    if (DEBUG) printf("File size is %d\n", (int) size);
    buf = (int *) malloc(size);
    // set starting offset for the read operation
    if (DEBUG) printf("Setting view\n");
    MPI_File_set_view(*fh, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
    // read two integers from the file
    if (DEBUG) printf("Reading file\n");
    MPI_File_read(*fh, buf, size / 4, MPI_INT, st);

    if (DEBUG == 2)
    for (i = 0; i < size / 4; i++) {
        printf("%d %d\n", i, buf[i]);
    }
    if (DEBUG) printf("Size of int %d\n", sizeof(int));
    arraySize = size / 4;
    MPI_File_close(fh);
    return buf;
}

void quickSortBucket(int * bucket, int size, int rank) {
    if (DEBUG) printf("Process %d rank is sorting bucket:\n", rank);
    printArray(bucket, size);

    if (DEBUG) printf("Process %d quicksort Begin\n", rank);
    quicksort(bucket, 0, size - 1);
    if (DEBUG) printf("Process %d quicksort End, sorted array is\n", rank);
    printArray(bucket, size);
}

void saveData(int *bucket,int size, int msize,  int rank) {

    if (rank >= 0) {
        MPI_File fh;
        MPI_Status st;

        char fname[100];
        snprintf(fname, sizeof(char)*100, "%s%d_%s", "sorted", size, dataFile);
        int i = 0;
        int *buf = bucket;
        // construct output file name
        if (DEBUG) printf("-- %d -- saving data\n", rank);
        MPI_File_open(MPI_COMM_SELF, fname, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
        // set starting offset for the write operation
        MPI_File_set_view(fh, bucket[0]* 4, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
        // write four integers to the file
        MPI_File_write(fh, buf, msize, MPI_INT, &st);
        MPI_File_close(&fh);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Datafile name is required\n");
        return 0;
    }


    int size, rank, *buf, tag = 201;
    double startTime, readTime, sortStartTime, sortEndTime, saveTime, endTime, pureRead;
    int msize = 0;
    int prevSize = 0;
    unsigned message;
    MPI_File fh;
    MPI_Status st;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    int nextRank = (rank + 1) % size;
    int prevRank = (rank + size - 1) % size;
    struct Bucket *bucket;
    dataFile = argv[1];
    if (rank == 0) {
        startTime = MPI_Wtime();
        if (DEBUG) printf("Time start: %f\n", startTime);

        int bufSize = 0;
        int toSort = 10 * size;
        /*
         * 1. Read all data
         * 2. sort first 10*size elements, and selects elements at positions 10, 20, 10(size-1)
         *      as pivots
         * 3. Partition data into P bucket
         */
        // quicksort(array, 0, N-1);
        message = size;
//        MPI_Send(&message,1,MPI_INT,nextRank,tag,MPI_COMM_WORLD);
        if (DEBUG) printf("Process %d reading %s\n", rank, argv[1]);
        buf = readData(argv[1], &fh, &st, buf);
        pureRead = MPI_Wtime();
        quicksort(buf, 0, toSort - 1);
        bucket = initBucketList(size);
        //printBucketList(bucket, size);
        if (DEBUG) printf("Array content so far\n");
        printArray(buf, arraySize);
        if (DEBUG) printf("Fill bucket\n");
        fillBuckets(buf, bucket, size);
        if (DEBUG) printf("Print bucket again\n");
        //printBucketList(bucket, size);
        readTime = MPI_Wtime();

        int i;
        if (DEBUG) printf("%d, world size is %d\n", rank, size);
        for (i = 1; i < size ;i++)
        {
            MPI_Send(&bucket[i].size,1,MPI_INT, i, tag, MPI_COMM_WORLD);
            MPI_Send(bucket[i].elements, bucket[i].size, MPI_INT, i, tag, MPI_COMM_WORLD);
            MPI_Send(&bucket[i-1].size,1,MPI_INT, i, tag, MPI_COMM_WORLD);
            if (DEBUG) printf("-- %d --, message sent to %d\n", rank, i);
        }
        msize = bucket[0].size;
        free(buf);
        buf = bucket[0].elements;
        prevSize = 0;
    }

    if (rank != 0) {
        if (DEBUG) printf("Process %d is waiting for a message with data *******************************\n", rank);
        MPI_Recv(&msize, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        buf = (int *) malloc(msize*sizeof(int));
        MPI_Recv(buf, msize, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (DEBUG) printf("Process %d data  is *******************************\n", rank);
        printArray(buf,msize);
        MPI_Recv(&prevSize, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    if (DEBUG) printf("Process %d is starting to sort it's bucket\n", rank);

    MPI_Barrier(MPI_COMM_WORLD);
    if (!rank) {
        sortStartTime = MPI_Wtime();
    }
    quickSortBucket(buf, msize,  rank);

    MPI_Barrier(MPI_COMM_WORLD);
    if (!rank) {
        sortEndTime = MPI_Wtime();
    }

    if (DEBUG) printf("-- %d -- : calling save data with prev size = %d\n", rank, prevSize);
    saveData(buf, size, msize, rank);

    MPI_Barrier(MPI_COMM_WORLD);
    if (!rank) {
        saveTime = MPI_Wtime();
    }


    if (rank == 0) {
        freeBuckets(bucket, size);
    } else {
        free(buf);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        endTime = MPI_Wtime();
        printf("Time total time %f sortTime %f, saveTime %f, readtime %f, pure read %f\n", endTime - startTime, sortEndTime - sortStartTime, saveTime - sortEndTime, readTime - startTime, pureRead - startTime);
        printf("%f %f %f %f %f\n", endTime - startTime, sortEndTime - sortStartTime, saveTime - sortEndTime, readTime - startTime, pureRead - startTime);
    }
    MPI_Finalize();
    return 0;
}
