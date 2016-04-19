//-------------------------------------------------------------------------
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//-------------------------------------------------------------------------

// A producer-consumer program.
// Usage: ./prodcons-pthd [<nThreads>]
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


/**
 * Global variables
 */
pthread_mutex_t taskLock;
pthread_mutex_t producerLock;
pthread_mutex_t clientLock;
pthread_cond_t prodCond;
pthread_cond_t consCond;
int array[100] = {0};
queue_t * sharedQueue;
unsigned int producerStarted = 0;


void producer(long wid) {
    printf("Producer wid %ld started on %d\n",wid, sched_getcpu());
    int taskNumber = 0;
    task_t* task = NULL;
    int r = 0;

    /**
     *
     */
    while (taskNumber < 100)
    {
        if (!task) { taskNumber++; task = create_task(taskNumber, taskNumber);}
        pthread_mutex_lock(&taskLock);
        r = add_task(sharedQueue, task);
        pthread_mutex_unlock(&taskLock);
        if (!r) {
            pthread_mutex_lock(&producerLock);
            pthread_cond_wait(&prodCond, &producerLock);
            pthread_mutex_unlock(&producerLock);
        } else {
            //printf("Producer wid %ld produced %d\n",wid, taskNumber);
            task = NULL;
            pthread_cond_signal(&consCond);
        }
    }
    r = 0;
    //printf("Producer wid %ld finished producing\n Adding final task to the queue\n",wid, taskNumber);
    task = create_task(1, 0);
    while(!r) {
        pthread_mutex_lock(&taskLock);
        r = add_task(sharedQueue, task);
        pthread_mutex_unlock(&taskLock);
        if (r) {
            pthread_cond_signal(&consCond);
        } else {
            pthread_mutex_lock(&producerLock);
            pthread_cond_wait(&prodCond, &taskLock);
            pthread_mutex_unlock(&producerLock);
        }
    }
    printf("Producer wid %ld produced in %d tasks.\n", wid, taskNumber);
}

void consumer(long wid) {
    printf("Consumer wid %ld started on %d\n",wid, sched_getcpu());
    int taskNumber = 0;
    task_t* task = NULL;
    if (!producerStarted) return;

    while(1) {
        pthread_mutex_lock(&taskLock);
        task = remove_task(sharedQueue);
        pthread_mutex_unlock(&taskLock);
        if (task) {
            pthread_cond_signal(&prodCond);
            if (task->low != task->high) {
                //printf("Consumer wid %ld received final task.\n", wid);
                add_task(sharedQueue,task);
                pthread_cond_signal(&consCond);
                printf("Consumer wid %ld consumed %d tasks in total.\n", wid, taskNumber);
                array[wid] = taskNumber;
                return;
            }
            //printf("Consumer wid %ld is consuming %d\n",wid, task->low);
            taskNumber++;
            free(task);
            task = NULL;
        } else {
            pthread_mutex_lock(&clientLock);
            pthread_cond_wait(&consCond, &clientLock);
            pthread_mutex_unlock(&clientLock);
        }
    }
}

// Main routine for testing producer consumer
//
int main(int argc, char **argv) {
    int i = 0, numThreads = 2;

    // more than one argument, save second and ignore the rest
    if (argc >= 2) {
        if ((i = atoi(argv[1])) < 1) {
            printf("<numThreads> must be greater than 0\n");
            exit(0);
        }
    }
    if (i > 1)
        numThreads += i - 1;

    // Initialization section
    pthread_t thread[numThreads];
    sharedQueue = init_queue(20);

    pthread_mutex_init(&taskLock, NULL);   /* initialize mutex */
    pthread_mutex_init(&clientLock, NULL);   /* initialize mutex */
    pthread_mutex_init(&producerLock, NULL);   /* initialize mutex */

    pthread_cond_init(&prodCond, NULL);
    pthread_cond_init(&consCond, NULL);

    producerStarted = 0;

    for (long k = 0; k < numThreads-1; k++) {
        pthread_create(&thread[k], NULL, (void *) consumer, (void *) k);
    }

    // the main thread also runs a producer() routine;
    // its copy has the last id, numThreads-1

    sleep(1); // wait for all threads to initialize

    //producer(numThreads - 1);
    printf("Joining threads\n");
    // the main thread waits for consumer threads to join back
    for (long k = 0; k < numThreads-1; k++)
        pthread_join(thread[k], NULL);

    pthread_mutex_destroy(&taskLock);
    pthread_mutex_destroy(&clientLock);
    pthread_mutex_destroy(&producerLock);

    int sum = 0;
    for (i = 0; i < numThreads - 1; i++) {
        sum += array[i];
    }

    printf("Total tasks consumed: %d\n", sum);
}

