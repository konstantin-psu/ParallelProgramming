//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Jacobi method for solving a Laplace equation.  
//
// Usage: ./jacobi [N]
// 
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#define EPSILON 0.00001 	// convergence tolerance
#define VERBOSE 0 	// printing control
#define DEBUG   0 	// printing control
#define BILLION 1000000000L

uint64_t diffJ, diffG, diffR;
struct timespec start, end;

// Initialize the mesh with a fixed set of boundary conditions.
// 
void init_array(int n, double a[n][n])  {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) 
            a[i][j] = 0;
    }
    for (i = 1; i < n-1; i++) {
        a[n-1][i] = 1.0;
        a[i][n-1] = 1.0;
    }
}

// Display the whole mesh.
// 
void print_array(int n, double a[n][n])  {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%8.4f ", a[i][j]);
        printf("\n");
    }
}

// Jacobi iteration -- return the iteration count.
// 
int jacobi(int n, double x[n][n], double epsilon) {
    double xnew[n][n];	// buffer for new values    
    double delta;		// measure of convergence   
    int cnt = 0;		// iteration counter              
    int i, j;

    do {	
        delta = 0.0;
        for (i = 1; i < n-1; i++) {
            for (j = 1; j < n-1; j++) {
                xnew[i][j] = (x[i-1][j] + x[i][j-1] + x[i+1][j] + x[i][j+1]) / 4.0;
                delta = fmax(delta, fabs(xnew[i][j] - x[i][j]));
            }
        }	
        for (i = 1; i < n-1; i++) {
            for (j = 1; j < n-1; j++) {
                x[i][j] = xnew[i][j];
            }
        }
        cnt++;
        if (VERBOSE) {
            printf("Iter %d: (delta=%6.4f)\n", cnt, delta);
            print_array(n, x);
        }
    } while (delta > epsilon);
    return cnt;
}

// Gauss_seidel natural index ordering
// 
int gauss(int n, double x[n][n], double epsilon) {
    double delta;		// measure of convergence   
    int cnt = 0;		// iteration counter              
    int i, j;
    double prev = 0;

    int c = 0;
    do {	
        delta = 0.0;
        for (i = 1; i < n-1; i++) {
            for (j = 1; j < n-1; j++) {
                prev = x[i][j];
                x[i][j] = (x[i-1][j] + x[i][j-1] + x[i+1][j] + x[i][j+1]) / 4.0;
                delta = fmax(delta, fabs(x[i][j] - prev));
            }
        }	
        cnt++;
        if (VERBOSE) {
            printf("Iter %d: (delta=%6.4f)\n", cnt, delta);
            print_array(n, x);
        }
    } while (delta > epsilon);
    //     c++;
    // } while (c < 40);
    return cnt;
}

// Gauss_seidel red-black ordering
// 
int red_black(int n, double x[n][n], double epsilon) {
    double delta;		// measure of convergence   
    int cnt = 0;		// iteration counter              
    int i, j;
    double prev = 0;

    int c = 0;
    do {	
        delta = 0.0;
        // Red
        for (i = 1; i < n-1; i = i + 2) {
            for (j = 1; j < n-1; j = j + 2) {
                    prev = x[i][j];
                    x[i][j] = (x[i-1][j] + x[i][j-1] + x[i+1][j] + x[i][j+1]) / 4.0;
                    delta = fmax(delta, fabs(x[i][j] - prev));
            }
        }	

        for (i = 2; i < n-1; i = i + 2) {
            for (j = 2; j < n-1; j = j + 2) {
                    prev = x[i][j];
                    x[i][j] = (x[i-1][j] + x[i][j-1] + x[i+1][j] + x[i][j+1]) / 4.0;
                    delta = fmax(delta, fabs(x[i][j] - prev));
            }
        }	

        // Black
        for (i = 1; i < n-1; i = i + 2) {
            for (j = 2; j < n-1; j = j + 2) {
                    prev = x[i][j];
                    x[i][j] = (x[i-1][j] + x[i][j-1] + x[i+1][j] + x[i][j+1]) / 4.0;
                    delta = fmax(delta, fabs(x[i][j] - prev));
            }
        }	

        for (i = 2; i < n-1; i = i + 2) {
            for (j = 1; j < n-1; j = j + 2) {
                    prev = x[i][j];
                    x[i][j] = (x[i-1][j] + x[i][j-1] + x[i+1][j] + x[i][j+1]) / 4.0;
                    delta = fmax(delta, fabs(x[i][j] - prev));
            }
        }	

        cnt++;
        if (VERBOSE) {
            printf("Iter %d: (delta=%6.4f)\n", cnt, delta);
            print_array(n, x);
        }
    } while (delta > epsilon);
    return cnt;
}

// Main routine.
//
int main(int argc, char **argv) {

    int n = 8;  	   	// mesh size, default 8 x 8
    if (argc > 1) {  	// check command line for overwrite
        if ((n = atoi(argv[1])) < 2) {
            printf("Mesh size must must be greater than 2, use default\n");
            n = 8;
        }
    }

    double a[n][n];	// mesh array
    init_array(n, a);
    if (DEBUG) printf("Initalizing array");
    //print_array(n,a);

    // Jacobi iteration, return value is the total iteration number

    clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */

    int cnt = jacobi(n, a, EPSILON);

    clock_gettime(CLOCK_MONOTONIC, &end); /* mark start time */
    diffJ = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;

    printf("JACOBI: Mesh size: %d x %d, epsilon=%6.4f, total Jacobi iterations: %d\n", n, n, EPSILON, cnt);
    //printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);
    printf("elapsed time = %f nanoseconds\n", (diffJ*1.0)/1000000000ULL);

    init_array(n, a);

    clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */
    cnt = gauss(n, a, EPSILON);
    clock_gettime(CLOCK_MONOTONIC, &end); /* mark start time */
    diffG = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("GAUSS:  Mesh size: %d x %d, epsilon=%6.4f, total Jacobi iterations: %d\n", n, n, EPSILON, cnt);
    printf("elapsed time = %f nanoseconds\n", (diffG*1.0)/1000000000ULL);

    init_array(n, a);
    clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */
    cnt = red_black(n, a, EPSILON);
    clock_gettime(CLOCK_MONOTONIC, &end); /* mark start time */
    diffR = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("Red-Black: Mesh size: %d x %d, epsilon=%6.4f, total Jacobi iterations: %d\n", n, n, EPSILON, cnt);
    printf("elapsed time = %f nanoseconds\n", (diffR*1.0)/1000000000ULL);
    if (DEBUG) print_array(n, a);
}
