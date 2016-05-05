#----------------------------------------------------------------------
# CS415/515 Assignment 2
#----------------------------------------------------------------------

FLAGS = -std=c99 -fopenmp -g -lm
CC = gcc

all: qsort qsort-omp prime prime-omp

qsort: qsort.c
	$(CC) -o $@ $< $(FLAGS)

qsort-omp: qsort-omp.c
	$(CC) -o $@ $< $(FLAGS)

prime: prime.c
	$(CC) -o $@ $< $(FLAGS)

prime-omp: prime-omp.c
	$(CC) -o $@ $< $(FLAGS)

clean:
	rm -rf prime-omp qsort qsort-omp prime
