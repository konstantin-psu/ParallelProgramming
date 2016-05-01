//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A sequential prime-finding algorithm.
//
// Usage: ./prime <N>
// 
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main(int argc, char **argv) {
    int N;
    int THREADS = 1;

    /* check command line first */
    if (argc < 2) {
        printf("Usage: ./prime <N>\n");
        exit(0);
    }


    if ((N = atoi(argv[1])) < 2) {
        printf("N must be greater than 1\n");
        exit(0);
    }

    if (argc > 2) {
        if ((THREADS = atoi(argv[2])) < 1) {
            THREADS = 1;
        }
    }

    printf("Finding primes in range 1..%d\n", N);

    int *array = (int *) malloc(sizeof(int) * (N + 1));
    int tid;
    int i;
    int cnt = 0;
    omp_set_num_threads(THREADS);
    //#pragma omp parallel private(tid) shared(i,cnt)

    for (i = 2; i <= N; i++) {
        tid = omp_get_thread_num();
        //printf("thread %d i %d\n", tid, i);
        array[i] = 1;
    }

    int limit = (int) sqrt((double) N);
#pragma omp parallel for shared(i)
    for (int i = 2; i <= limit; i++) {
        tid = omp_get_thread_num();
        //printf("thread %d i %d\n", tid, i);
        if (array[i] == 1) {
            for (int j = i + i; j <= N; j += i)
                array[j] = 0;
        }
    }

    // #pragma omp parallel for num_threads(THREADS) shared(i,cnt)
#pragma omp parallel for reduction(+:cnt) shared(i)
    for (int i = 2; i <= N; i++) {
        if (array[i] == 1)
            cnt += 1;
    }
    printf("Total %d primes found\n", cnt);

}

