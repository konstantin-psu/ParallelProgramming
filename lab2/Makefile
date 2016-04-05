CC=gcc
PTH=$(CC) -pthread -g -std=c99
OMP=$(CC) -fopenmp -g
MPI=mpicc -g
CPL=chpl -g


all: condvar-pthd arraysum-pthd


condvar-pthd: condvar-pthd.c
	$(PTH) -o  condvar-pthd condvar-pthd.c

arraysum-pthd: arraysum-pthd.c
	$(PTH) -o  arraysum-pthd arraysum-pthd.c
clean:
	rm condvar-pthd arraysum-pthd
