// The sum program (MPI version).
//
//
#include <stdio.h>
#include <mpi.h>

#define N 100 	/* problem domain size */

int sum = 0;

int compute(int i) {
  return i*i;
}

int main(int argc, char **argv) {  
  int rank, size, dest; 
  int i, low, high, psum, sum;

  MPI_Init(&argc, &argv);  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
  MPI_Comm_size(MPI_COMM_WORLD, &size);  
  
  low = (N/size) * rank;    /* a simplistic partition scheme */ 
  high = low + (N/size);	
  psum = 0;
  for (i = low; i < high; i++)
    psum += compute(i);

  dest = 0;
  MPI_Reduce(&psum, &sum, 1, MPI_INT, MPI_SUM, dest, MPI_COMM_WORLD);
  if (rank == dest)
    printf("The result sum is %d\n", sum);

  MPI_Finalize();
  return 0;
}  
