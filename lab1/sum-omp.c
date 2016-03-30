// The sum program (OpenMP version).
//
//
#include <stdio.h>
#include <omp.h>

#define N 100 	/* problem domain size */
#define P 10 	/* number of threads */

int sum = 0;

int compute(int i) {
  return i*i;
}

int main(int argc, char **argv) {
  int i; 
  omp_set_num_threads(P);
#pragma omp parallel for reduction(+:sum)
  for (i = 0; i < N; i++)
    sum += compute(i);
  printf("The result sum is %d\n", sum);
}  
