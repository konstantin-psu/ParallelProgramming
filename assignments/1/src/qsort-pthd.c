//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//-------------------------------------------------------------------------

// A sequential quicksort program.
// Usage: ./qsort-pthd <N> [<nThreads>]
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

#define SEC(t) (((double)t))

/**
 * Global variables
 */
#define MINSIZE   10                      // threshold for switching to bubblesort
pthread_mutex_t taskLock;                 // Used to lock queue during add/remove task
pthread_mutex_t clientLock;               // Protect numberOfProducers during update
task_t * initialTask;                     // The First task that created by the master thread
int numberOfProducers = 0, numConsumers = 1;  // numberOfProducers = number of quicksorts (terminating conditions)
                                          // numConsumers by default 1
int * array;                              // array to be sorted
queue_t * sharedQueue;                    // Task queue


/**
 * Swap two array elements
 */
void swap(int *array, int i, int j) {
    if (i == j) return;
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

/**
 *
 * Initialize array.
 * - first generate [1,2,...,N]
 * - then perform a random permutation
 *
 */
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


/**
 * Verify that array is sorted
 */
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

/**
 *
 * Pick an arbitrary element as pivot. Rearrange array
 * elements into [smaller one, pivot, larger ones].
 * Return pivot's index.
 *
 */
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

/**
 * QuickSort an array range
 */
void quicksort(int *array, int low, int high) {
    /**
     * BubbleSort performs better on smaller arrays
     */
    if (high - low < MINSIZE) {
        pthread_mutex_lock(&clientLock);
        numberOfProducers--;                    // Exiting - decrease qSort count
        pthread_mutex_unlock(&clientLock);
        bubblesort(array, low, high);
        return;
    }
    int middle = partition(array, low, high);
    if (low < middle) {
        task_t * task = create_task(low, middle - 1);

        /**
         * Protect addition/removal to/from queue
         */
        pthread_mutex_lock(&taskLock);
        add_task(sharedQueue, task);
        pthread_mutex_unlock(&taskLock);
    }
    if (middle < high) {
        pthread_mutex_lock(&clientLock);
        numberOfProducers++;                   // Increase every time quicksort is called
        pthread_mutex_unlock(&clientLock);
        quicksort(array, middle + 1, high);
    }

    /**
     * Protect addition/removal to/from queue
     */
    pthread_mutex_lock(&clientLock);
    numberOfProducers--;                    // Exiting - decrease qSort count
    pthread_mutex_unlock(&clientLock);
}


void consumer(long wid) {
    //printf("Worker wid %ld started on %d\n",wid, sched_getcpu());
    int sorts = 0;                       // count number of quicksort calls in this consumer
    task_t* task = NULL;
    int numberOfProducersLocal = numberOfProducers;

    /**
     * Check if called by the parent thread - if so call qucksort to start filling the queue
     */
    if (wid == numConsumers - 1) {
        sorts ++;
        quicksort(array,initialTask->low,initialTask->high);
        free(initialTask);  // No longer need the initial Task
    }

    /**
     * Loop while the queue is not empty or quicksort(in this case producer is still running)
     */
    while (task || numberOfProducersLocal)
    {
        /**
         * Save number of producers to check in while loop
         */
        pthread_mutex_lock(&taskLock);
        numberOfProducersLocal = numberOfProducers;

        /**
         * Try to get a task
         */
        task = remove_task(sharedQueue);
        pthread_mutex_unlock(&taskLock);

        //  sort it
        if (task) {
            pthread_mutex_lock(&clientLock);
            numberOfProducers++;
            pthread_mutex_unlock(&clientLock);
            sorts++;
            quicksort(array, task->low, task->high);
            free(task); // No longer need the task, this is safe to do since free doesn't reset
                        // the pointer, so this is still a valid check for termination
        }
    }
    //printf("Worker wid %ld participated in %d sorts.\n", wid, sorts);
}

// Main routine for testing quicksort
//
int main(int argc, char **argv) {
    int N;
    time_t start, initT, qsortT, verifyT, fullT;

    start = time(NULL);

    // check command line first
    if (argc < 2) {
        printf("Usage: ./qsort <N> [<numConsumers>]\n");
        exit(0);
    }

    // at least 1 argument
    if (argc >= 2) {
        if ((N = atoi(argv[1])) < 2) {
            printf("<N> must be greater than 2\n");
            exit(0);
        }
    }

    // more than one argument, save second and ignore the rest
    if (argc > 2) {
        if ((numConsumers = atoi(argv[2])) < 1) {
            printf("<N> must be greater than 0\n");
            exit(0);
        }
    }

    // Initialization section
    pthread_t thread[numConsumers];
    array = init_array(N);
    initT = time(NULL);
    sharedQueue = init_queue(0);
    initialTask = create_task(0, N-1);

    // numberOfProducers is a termination conditions, so set it to 1, so other threads
    // don't quit prematurely
    numberOfProducers = 1;

    pthread_mutex_init(&taskLock, NULL);   /* initialize mutex */
    pthread_mutex_init(&clientLock, NULL);   /* initialize mutex */

    for (long k = 0; k < numConsumers -1; k++) {
        pthread_create(&thread[k], NULL, (void *) consumer, (void *) k);
    }

    // the main thread also runs a copy of the consumer() routine;
    // its copy has the last id, numConsumers-1
    consumer(numConsumers - 1);
    //printf("Joining threads\n");
    // the main thread waits for consumer threads to join back
    for (long k = 0; k < numConsumers -1; k++)
        pthread_join(thread[k], NULL);

    pthread_mutex_destroy(&taskLock);
    pthread_mutex_destroy(&clientLock);

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
    free(array);
    free(sharedQueue);
}

