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
  for (i = 0; i < size; i++)
    array[i] = i + 1;

  for (i = 0; i < size; i++) {
    sum += array[i];
  }
  printf("The sum of the array [1,2,..,%d] is %d\n", size, sum);
}  

