//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University
//------------------------------------------------------------------------- 

// Matrix multiplication algorithm.  
//
#include <stdio.h>

#define N 16

int a[N][N], b[N][N], c[N][N];

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

int main(int argc, char **argv) {
  int i, j, k;

  init_array();

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
  	c[i][j] = 0.;
  	for (k = 0; k < N; k++) {
  	  c[i][j] += a[i][k] * b[k][j];
  	}
    }
  }
  print_array();
}
