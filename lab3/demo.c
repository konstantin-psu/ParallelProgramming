//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A simple OpenMP demo program.
//
#include <stdio.h>

int main() {
  int x = 2;
  #pragma omp parallel num_threads(2) shared(x)
  {
    if (omp_get_thread_num() == 0)
      x = 5;
    else
      printf("1: Thread %d: x = %d\n", omp_get_thread_num(), x);
    #pragma omp barrier
    printf("2: Thread %d: x = %d\n", omp_get_thread_num(), x);
  }
  return 0;
}
