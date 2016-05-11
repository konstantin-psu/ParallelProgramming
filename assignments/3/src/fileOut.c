//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University
//------------------------------------------------------------------------- 

// A simple demo program of MPI file output.
//
// Usage: 
//   linux> mpirun -n <#procs> fileOut
//
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
  int cnt=4, buf[4], i, rank;
  char fname[10];
  MPI_File fh;
  MPI_Status st;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // prepare data
  for (i=0; i<cnt; i++) 
    buf[i] = rank*100 + i;
  // construct output file name
  sprintf(fname, "%s.%d", "out", "gen");
  // open file for write
  MPI_File_open(MPI_COMM_SELF, fname, MPI_MODE_CREATE|MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
  // set starting offset for the write operation
  MPI_File_set_view(fh, rank*16, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
  // write four integers to the file
  MPI_File_write(fh, buf, cnt, MPI_INT, &st);
  MPI_File_close(&fh);

  MPI_Finalize();
  return 0; 
}
