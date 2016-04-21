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
#include <sched.h>		// for getting cpu id
#include "task-queue.h"
#include <pthread.h>


/**
 * Global variables
 */
pthread_mutex_t taskLock;
pthread_mutex_t producerLock;
pthread_mutex_t clientLock;
pthread_cond_t prodCond;
pthread_cond_t consCond;
int * array;
queue_t * sharedQueue;
unsigned int producerStarted = 0;
int numConsumers = 1;


void producer(long wid) {
    printf("Producer wid %ld started on %d\n",wid, sched_getcpu());
    int taskNumber = 0;
    task_t* task = NULL;
    int r = 0;

    while (1)
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
            printf("Producer wid %ld produced %d\n",wid, taskNumber);
            task = NULL;
            pthread_cond_signal(&consCond);
            if ( taskNumber == 100) break;
        }
    }

    //printf("Producer wid %ld finished producing\n Adding final task to the queue\n",wid, taskNumber);
    task = create_task(1, 0);
    r = 0;
    while(!r) {
        pthread_mutex_lock(&taskLock);
        r = add_task(sharedQueue, task);
        pthread_mutex_unlock(&taskLock);
        if (r) {
            pthread_cond_signal(&consCond);
            //printf("Producer wid %ld EXIT TASK ADDED\n",wid);
        } else {
            pthread_mutex_lock(&producerLock);
            pthread_cond_wait(&prodCond, &producerLock);
            pthread_mutex_unlock(&producerLock);
        }
    }
    //printf("Producer wid %ld produced %d tasks. EXIT\n", wid, taskNumber);
}

void consumer(long wid) {
    printf("Consumer wid %ld started on %d\n",wid, sched_getcpu());
    int taskNumber = 0;
    task_t* task = NULL;
    if (!producerStarted) {
        printf("No producer detected\n");
        return;
    }

    while(1) {
        pthread_mutex_lock(&taskLock);
        task = remove_task(sharedQueue);
        pthread_mutex_unlock(&taskLock);
        if (task) {
            pthread_cond_signal(&prodCond);
            if (task->low != task->high) {
                //printf("Consumer wid %ld received final task.\n", wid);
                pthread_mutex_lock(&taskLock);
                add_task(sharedQueue,task);
                pthread_cond_signal(&consCond);
                pthread_mutex_unlock(&taskLock);
                //printf("Consumer wid %ld consumed %d tasks in total.\n", wid, taskNumber);
                array[wid] = taskNumber;
                return;
            }
            printf("Consumer wid %ld performed task %d\n",wid, task->low);
            taskNumber++;
            free(task);
            task = NULL;
        } else {
            pthread_mutex_lock(&taskLock);
            //printf("Consumer wid %ld queue is empty, sleeping.\n", wid);
            pthread_cond_wait(&consCond, &taskLock);
            //printf("Consumer wid %ld queue is no longer empty, continuing.\n", wid);
            pthread_mutex_unlock(&taskLock);
        }
    }
}

// Main routine for testing producer consumer
//
int main(int argc, char **argv) {
    int i = 0;

    // more than one argument, save second and ignore the rest
    if (argc >= 2) {
        if ((numConsumers = atoi(argv[1])) < 1) {
            printf("<numConsumers> must be greater than 0\n");
            exit(0);
        }
    }

    // Initialization section
    array = (int *)malloc(sizeof(int)*numConsumers);
    pthread_t thread[numConsumers];
    sharedQueue = init_queue(20);

    pthread_mutex_init(&taskLock, NULL);   /* initialize mutex */
    pthread_mutex_init(&clientLock, NULL);   /* initialize mutex */
    pthread_mutex_init(&producerLock, NULL);   /* initialize mutex */

    pthread_cond_init(&prodCond, NULL);
    pthread_cond_init(&consCond, NULL);

    producerStarted = 1;

    for (long k = 0; k < numConsumers; k++) {
        pthread_create(&thread[k], NULL, (void *) consumer, (void *) k);
    }

    // the main thread also runs a producer() routine;
    // its copy has the last id, numConsumers-1

    //sleep(1); // wait for all threads to initialize

    producer(numConsumers);
    printf("Joining threads\n");
    // the main thread waits for consumer threads to join back
    for (long k = 0; k < numConsumers; k++)
        pthread_join(thread[k], NULL);

    pthread_mutex_destroy(&taskLock);
    pthread_mutex_destroy(&clientLock);
    pthread_mutex_destroy(&producerLock);
    pthread_cond_destroy(&consCond);
    pthread_cond_destroy(&prodCond);

    free(remove_task(sharedQueue));
    free(sharedQueue);
    int sum = 0;
    for (i = 0; i < numConsumers; i++) {
        if ( i == numConsumers - 1)
            printf("[%d]:%d", i, array[i]);
        else
            printf("[%d]:%d ", i, array[i]);
        sum += array[i];
    }

    printf(", total tasks = %d\n", sum);
    free(array);
}
