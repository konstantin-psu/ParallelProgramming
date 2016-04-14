CC=gcc
PTH=$(CC) -pthread -g
OMP=$(CC) -fopenmp -g
MPI=mpicc -g
CPL=chpl -g


all: sum sum1 sum2 sum-mpi sum-omp sum-pthd

sum:
	$(CC) -o sum sum.c

sum1: sum1.chpl
	$(CPL) -o sum1 sum1.chpl

sum2: sum2.chpl
	$(CPL) -o sum2 sum2.chpl

sum-mpi: sum-mpi.c
	$(MPI) -o sum-mpi sum-mpi.c

sum-omp:
	$(OMP) -o sum-omp sum-omp.c

sum-pthd: sum-pthd.c
	$(PTH) -o sum-pthd sum-pthd.c

clean:
	rm sum sum1 sum2 sum-mpi sum-omp sum-pthd

run-mpi: sum-mpi
	mpirun -n 4 ./sum-mpi

run-sum1: sum1
	./sum1 -nl 1 # running the program over 1 locale

run-sum2: sum2
	./sum2 -nl 1 # running the program over 1 locale

run-sum1N: sum1
	./sum1 --N=$(NCH) -nl 1 # running the program over 1 locale

run-sum2N: sum2
	./sum2 --N=$(NCH) -nl 1 # running the program over 1 locale
