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
#include <omp.h>

#define MINSIZE   10        // threshold for switching to bubblesort
#define SEC(t) (((double)t))
int THREADS = 1;

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
    srand(time(NULL));
    int i;
    for (i = 0; i < N; i++) {
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
void quicksort(int *array, int low, int high) {
    if (high - low < MINSIZE) {
        bubblesort(array, low, high);
        return;
    }
    int middle = partition(array, low, high);
    // each quicksort call assigned to a new thread
        if (low < middle) {
            #pragma omp task firstprivate(array, low, middle)
            quicksort(array, low, middle - 1);
        }
        if (middle < high) {
            #pragma omp task firstprivate(array, middle, high)
            quicksort(array, middle + 1, high);
        }
}

// Main routine for testing quicksort
// 
int main(int argc, char **argv) {
    int *array, N;
    time_t start, initT, qsortT, verifyT, fullT;

    start = time(NULL);
    // check command line first
    if (argc < 2) {
        printf("Usage: ./qsortT <N> [NUM THREADS default is 1]\n");
        exit(0);
    }
    if ((N = atoi(argv[1])) < 2) {
        printf("<N> must be greater than 2\n");
        exit(0);
    }

    if (argc > 2) {
        if ((THREADS = atoi(argv[2])) < 1) {
            THREADS = 1;
        }
    }
    omp_set_num_threads(THREADS);

    array = init_array(N);
    initT = time(NULL);
    //  printf("Sorting started ...\n");
    // Declare first parallel region
    # pragma omp parallel
    {
        // make sure first quicksort executed by only one thread.
        #pragma omp single nowait
        {
            printf("Calling quicksort\n");
            quicksort(array, 0, N - 1);
            //  printf("... completed.\n");
        }
    }
    qsortT =time(NULL);
    verify_array(array, N);
    verifyT =time(NULL);

    fullT = verifyT - start;
    verifyT = verifyT - qsortT;
    qsortT = qsortT - initT;
    initT = initT - start;
    printf("realI    %.3f s\n", SEC(fullT));
    printf("initT    %.3f s ratio %.3f\n", SEC(initT),SEC(initT)/SEC(fullT));
    printf("qsortT   %.3f s ratio %.3f\n", SEC(qsortT),SEC(qsortT)/SEC(fullT));
    printf("verifyT  %.3f s ratio %.3f\n", SEC(verifyT),SEC(verifyT)/SEC(fullT));
}

