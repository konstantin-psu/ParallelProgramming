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
#include <pthread.h>
#include <unistd.h>


/**
 * Global variables
 */
#define MINSIZE   10                      // threshold for switching to bubblesort
pthread_mutex_t taskLock;                 // Used to lock queue during add/remove task
pthread_cond_t cond;
int test = 0;



void consumer(long wid) {
    pthread_mutex_lock(&taskLock);
    test = 1;
    sleep(20);
    pthread_mutex_unlock(&taskLock);
}

void producer(long wid) {
    test = 2;
    printf("test is %d\n", test);
}

// Main routine for testing quicksort
//
int main(int argc, char **argv) {
    pthread_t cons;

    pthread_mutex_init(&taskLock, NULL);   /* initialize mutex */
    pthread_cond_init(&cond, NULL);

    pthread_create(&cons, NULL, (void *) consumer, (void *) 0);

    // the main thread also runs a copy of the consumer() routine;
    // its copy has the last id, numThreads-1

    sleep(1); // wait for all threads to initialize

    producer(1);
    printf("Joining threads\n");
    // the main thread waits for consumer threads to join back
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&taskLock);
}

