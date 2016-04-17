//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//-------------------------------------------------------------------------

// A sequential quicksort program.
// Usage: ./qsort <N> [<nThreads>]
// 
//
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>		// for getting cpu id
#include "task-queue.h"
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t tLock;
pthread_mutex_t iLock;
task_t * initialTask;
#define MINSIZE   10        // threshold for switching to bubblesort

int recursionLevel = 0, numThreads = 1;
int * array;

queue_t * sharedQueue;


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
void quicksort(int *array, int low, int high) {
    if (high - low < MINSIZE) {
        pthread_mutex_lock(&iLock);
        recursionLevel--;
        pthread_mutex_unlock(&iLock);
        bubblesort(array, low, high);
        return;
    }
    int middle = partition(array, low, high);
    if (low < middle) {
        task_t * task = create_task(low, middle - 1);
        pthread_mutex_lock(&tLock);
        add_task(sharedQueue, task);
        pthread_mutex_unlock(&tLock);
    }
    if (middle < high) {
        pthread_mutex_lock(&iLock);
        recursionLevel++;
        pthread_mutex_unlock(&iLock);
        quicksort(array, middle + 1, high);
    }
    pthread_mutex_lock(&iLock);
    recursionLevel--;
    pthread_mutex_unlock(&iLock);
}


void worker(long wid) {
    printf("Worker wid %ld started on %d\n",wid, sched_getcpu());
    int sorts = 0;
    task_t* task = NULL;
    int localRecurs = recursionLevel;
    if (wid == numThreads - 1) {
        sorts ++;
        quicksort(array,initialTask->low,initialTask->high);
        free(initialTask);
        task = NULL;
    }

    while (task || localRecurs)
    {
        pthread_mutex_lock(&tLock);
        localRecurs = recursionLevel;
        task = remove_task(sharedQueue);
        pthread_mutex_unlock(&tLock);
        if (task) {
            pthread_mutex_lock(&iLock);
            recursionLevel++;
            pthread_mutex_unlock(&iLock);
            sorts++;
            quicksort(array, task->low, task->high);
            free(task);
        }
    }
    printf("Worker wid %ld participated in %d sorts.\n", wid, sorts);
}

// Main routine for testing quicksort
//
int main(int argc, char **argv) {
    int N;

    int * test = (int *)malloc(sizeof(int));
    if (test) {printf("test allocated\n");}
    free(test);
    if (test) {printf("test deleted but not reset\n");}

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

    if (argc > 2) {
        if ((numThreads = atoi(argv[2])) < 1) {
            printf("<N> must be greater than 0\n");
            exit(0);
        }
    }

    pthread_t thread[numThreads];
    array = init_array(N);
    sharedQueue = init_queue(0);
    initialTask = create_task(0, N-1);
    recursionLevel = 1;

    pthread_mutex_init(&tLock, NULL);   /* initialize mutex */
    pthread_mutex_init(&iLock, NULL);   /* initialize mutex */

    for (long k = 0; k < numThreads-1; k++) {
        pthread_create(&thread[k], NULL, (void *) worker, (void *) k);
    }

    // the main thread also runs a copy of the worker() routine;
    // its copy has the last id, numThreads-1

    sleep(1); // wait for all threads to initialize

    worker(numThreads-1);
    printf("Joining threads\n");
    // the main thread waits for worker threads to join back
    for (long k = 0; k < numThreads-1; k++)
        pthread_join(thread[k], NULL);

    pthread_mutex_destroy(&tLock);
    pthread_mutex_destroy(&iLock);

    verify_array(array, N);

}

