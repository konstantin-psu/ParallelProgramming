CC=gcc
PTH=$(CC) -pthread -g
OMP=$(CC) -fopenmp -g
MPI=mpicc -g
CPL=chpl -g


all: sum sum1 sum2 sum-mpi sum-omp sum-pthd

sum:
	$(CC) -o sum sum.c

sum1:
	$(CPL) -o sum1 sum1.chpl

sum2:
	$(CPL) -o sum2 sum2.chpl

sum-mpi:
	$(MPI) -o sum-mpi sum-mpi.c

sum-omp:
	$(OMP) -o sum-omp sum-omp.c

sum-pthd:
	$(PTH) -o sum-pthd sum-pthd.c

clean:
	rm sum sum1 sum2 sum1_real sum2_real sum-mpi sum-omp sum-pthd
