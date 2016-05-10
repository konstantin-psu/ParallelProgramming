//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University
//------------------------------------------------------------------------- 

// A simple demo program of MPI file input.
//
// Usage: 
//   linux> mpirun -n <#procs> fileIn data.txt
//
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) 
{ 
  int size, rank, buf[2]; 
  MPI_File fh; 
  MPI_Status st;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // open file for read
  MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
  // set starting offset for the read operation
  MPI_File_set_view(fh, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
  // read two integers from the file
  MPI_File_read(fh, buf, 2, MPI_INT, &st);
  MPI_File_close(&fh);

  // print out the two integers
  printf("rank=%d: buf=[%d,%d]\n", rank, buf[0], buf[1]);
  MPI_Finalize();
  return 0;
}
