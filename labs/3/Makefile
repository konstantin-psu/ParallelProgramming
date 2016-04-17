CC=gcc
PTH=$(CC) -pthread -g
OMP=$(CC) -fopenmp -g
MPI=mpicc -g
CPL=chpl -g


all:  demo arraysum mtxmul

demo: demo.c
	$(OMP) -o demo demo.c

arraysum: arraysum.c
	$(OMP) -o arraysum arraysum.c

mtxmul: mtxmul.c
	$(OMP) -o mtxmul mtxmul.c

clean:
	rm -f demo mtxmul arraysum *.o

run-demo: demo
	./demo

run-mtxmul: mtxmul
	./mtxmul

run-arraysum: arraysum
	./arraysum

