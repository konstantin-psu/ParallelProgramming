//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A simple array-sum program.
//
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int *array, i, size, sum = 0; 
  int *array2;

  // check command-line for user input
  if (argc < 2) {
    printf ("Usage: ./arraysum <arraySize>\n");
    exit(0);
  }
  if ((size = atoi(argv[1])) < 1) {
    printf ("<arraySize> must be greater than 0\n");
    exit(0);
  }
  
  // initialize array to [1,2,..,size]
  array = (int *) malloc(sizeof(int) * size);
  array2 = (int *) malloc(sizeof(int) * size);
      for (i = 0; i < size; i++)
        array[i] = i + 1;

  // # pragma omp parallel num_threads(8) reduction (+:sum)
  // # pragma for shared(array) private(i)
  // # pragma omp for
  int tid;
  # pragma omp parallel for num_threads(4) private(tid)
  for (i = 0; i < size; i++) {
      tid = omp_get_thread_num();
      //printf("%d tid %d || ",i, tid);
      printf("tid %d || ", tid);
      sum = sum + array[i];
  }
  printf("\n");
  printf("  PARALLEL: The sum of the array [1,2,..,%d] is %d\n", size, sum);

  sum = 0;
  for (i = 0; i < size; i++)
    array2[i] = i + 1;

  for (i = 0; i < size; i++) {
    sum += array2[i];
  }

  printf("SEQUENTIAL: The sum of the array [1,2,..,%d] is %d\n", size, sum);

  free(array);
  free(array2);
  return;
}

