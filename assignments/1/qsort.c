//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A sequential quicksort program.
//
// Usage: ./qsort <N>
// 
//
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "task-queue.h"
#include <pthread.h>

pthread_mutex_t pLock;
pthread_mutex_t bLock;
#define MINSIZE   10        // threshold for switching to bubblesort

int thread_count = 0;

struct args {
    int low;
    int high;
    int *array;
};


// Swap two array elements 
//
void swap(int *array, int i, int j) {
    if (i == j) return;
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}


// Initialize array.
// - first generate [1,2,...,N]
// - then perform a random permutation
//
int *init_array(int N) {
    int *array = (int *) malloc(sizeof(int) * N);
    for (int i = 0; i < N; i++) {
        array[i] = i + 1;
    }
    //srand(time(NULL));
    srand(1);
    for (int i = 0; i < N; i++) {
        int j = (rand() * 1. / RAND_MAX) * (N - 1);
        swap(array, i, j);
    }
    printf("Initialized array to a random permutation of [1..%d]\n", N);
    return array;
}

// Verify that array is sorted
//
void verify_array(int *array, int N) {
    for (int i = 0; i < N - 1; i++)
        if (array[i] > array[i + 1]) {
            printf("FAILED: array[%d]=%d, array[%d]=%d\n",
                   i, array[i], i + 1, array[i + 1]);
            return;
        }
    printf("Result verified!\n");
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
void quicksort(void *qargs) {
    printf("Initializing quicksort\n");
    struct args *thisArgs = (struct args *) qargs;
    int low = thisArgs->low;
    int high = thisArgs->high;

    pthread_mutex_lock(&bLock);
    int *array = thisArgs->array;
    pthread_mutex_unlock(&bLock);

    printf("Created arguments quicksort\n");
    if (high - low < MINSIZE) {
        printf("Bubble sorting %d %d\n", low, high);
        bubblesort(array, low, high);
        return;
    }
    printf("Partitioning quicksort\n");
    thread_count++;
    int middle = partition(array, low, high);
    printf("enter low %d, middle %d, high %d\n", low, middle, high);
    printf("threads created so far %d\n", thread_count);
    printf("Partitioning done quicksort\n");
    pthread_t l;
    pthread_t h;
    struct args rArgs;
    struct args lArgs;
    if (low < middle) {
        pthread_mutex_lock(&bLock);
        lArgs.array = array;
        pthread_mutex_unlock(&bLock);
        lArgs.low = low;
        lArgs.high = middle - 1;
        printf("left low %d, high %d\n", lArgs.low, lArgs.high);
        //quicksort(&lArgs);
        if (pthread_create(&l, NULL, (void *) quicksort, &lArgs))
            printf("ERROR *************************************************************");
    }
    if (middle < high) {
        pthread_mutex_lock(&bLock);
        rArgs.array = array;
        pthread_mutex_unlock(&bLock);
        rArgs.low = middle + 1;
        rArgs.high = high;
        printf("right low %d, high %d\n", rArgs.low, rArgs.high);
        //quicksort(&rArgs);
        if (pthread_create(&l, NULL, (void *) quicksort, &rArgs))
            printf("ERROR *************************************************************");
    }
    pthread_join(l, NULL);
    pthread_join(h, NULL);

}

// Main routine for testing quicksort
// 
int main(int argc, char **argv) {
    int *array, N, NUM_THREADS = 0;

    // check command line first
    if (argc < 2) {
        printf("Usage: ./qsort <N> [<numThreads>]\n");
        exit(0);
    }

    if (argc >= 2) {
        if ((N = atoi(argv[1])) < 2) {
            printf("<N> must be greater than 2\n");
            exit(0);
        }
    }

    if (argc >= 2) {
        if ((NUM_THREADS = atoi(argv[2])) < 2) {
            printf("<N> must be greater than 2\n");
            exit(0);
        }
    }

//    array = init_array(N);
//    struct args sort_args;
//    pthread_t tid;
//    sort_args.low = 0;
//    sort_args.high = N - 1;
//    sort_args.array = array;
//    //  printf("Sorting started ...\n");
//    pthread_create(&tid, NULL, (void *) quicksort, (void *) (&sort_args));
//    // quicksort(array, 0, N-1);
//    pthread_join(tid, NULL);
//    //  printf("... completed.\n");
//    int i;
//    for (i = 0; i < N; i++) {
//        printf("%d ", *(array + i));
//    }
//    printf("\n");
//
    verify_array(array, N);
}

