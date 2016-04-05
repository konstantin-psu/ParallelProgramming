CC=gcc
PTH=$(CC) -pthread -g
OMP=$(CC) -fopenmp -g
MPI=mpicc -g
CPL=chpl -g


all: condvar-pthd


condvar-pthd: condvar-pthd.c
	$(PTH) -o  condvar-pthd condvar-pthd.c

clean:
	rm condvar-pthd
