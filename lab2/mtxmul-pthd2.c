//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University
//------------------------------------------------------------------------- 

// Matrix multiplication algorithm.  
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>		// for getting cpu id
#include <unistd.h>             // for getting nprocs
#define N 16

int a[N][N], b[N][N], c[N][N];
int numThreads = 1;             // default number of threads 
pthread_mutex_t sumLock;   
int idx = 0;

void init_array()  {
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = i + j;
      b[i][j] = N - j;
    }
  }
}

void print_array()  {
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++)
      printf("%4d ", c[i][j]);
    printf("\n");
  }
}

void slave(long tid) {
    //printf("Thread %ld started on %d\n", tid, sched_getcpu());
    int j, k, f;

    pthread_mutex_lock(&sumLock);    	// read and increment idx
    f = idx++;                        	
    pthread_mutex_unlock(&sumLock);     
     
    while (f < N) {
        for (j = 0; j < N; j++) {
            c[idx][j] = 0.;
            for (k = 0; k < N; k++) {
                c[idx][j] += a[idx][k] * b[k][j];
            }
        }
        pthread_mutex_lock(&sumLock);    	// read and increment idx
        f = idx++;                        	
        pthread_mutex_unlock(&sumLock);     
    }
} 

int main(int argc, char **argv) {
  // check command-line for user input
  if (argc > 1) {
    if ((numThreads=atoi(argv[1])) < 1) {
      printf ("<numThreads> must be greater than 0\n");
      exit(0);
    }
  }

  long i;

  init_array();
  pthread_t thread[numThreads];
  pthread_mutex_init(&sumLock, NULL);   // initialize mutex 
  int nprocs = sysconf(_SC_NPROCESSORS_ONLN);

  for (i = 0; i <numThreads; i++) {
    pthread_create(&thread[i], NULL, (void*)slave, (void*)i);
  }
  for (long i = 0; i < numThreads; i++) {          // join threads 
    pthread_join(thread[i], NULL);
  }

  print_array();
}
