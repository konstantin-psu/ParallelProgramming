CC=gcc
PTH=$(CC) -pthread -g -std=c99


all: condvar-pthd arraysum-pthd mtxmul-pthd mtxmul-pthd2


condvar-pthd: condvar-pthd.c
	$(PTH) -o  condvar-pthd condvar-pthd.c

arraysum-pthd: arraysum-pthd.c
	$(PTH) -o  arraysum-pthd arraysum-pthd.c

mtxmul-pthd: mtxmul-pthd.c
	$(PTH) -o  mtxmul-pthd mtxmul-pthd.c

mtxmul-pthd2: mtxmul-pthd2.c
	$(PTH) -o  mtxmul-pthd2 mtxmul-pthd2.c

clean:
	rm condvar-pthd arraysum-pthd mtxmul-pthd mtxmul-pthd2
