#----------------------------------------------------------------------
# CS415/515 Assignment 1
#----------------------------------------------------------------------

FLAGS = -std=c99 -pthread -g -DVERBOSE -Wall -Werror
CC=gcc

all: qsort-pthd prodcons-pthd

.PHONY: qsort-pthd
qsort-pthd: qsort-pthd.c  task-queue.h task-queue.c
	$(CC) $(FLAGS) -o $@ qsort-pthd.c  task-queue.h task-queue.c

.PHONY: prodcons-pthd
prodcons-pthd: prodcons-pthd.c  task-queue.h task-queue.c
	$(CC) $(FLAGS) -o $@ prodcons-pthd.c  task-queue.h task-queue.c

task-queue: task-queue.h task-queue.c
	$(CC) $(FLAGS) -o $@ task-queue.c task-queue.h

%: task-queue.h task-queue.c %.c 
	$(CC) $(FLAGS) -o $@ task-queue.c $@.c

clean:
	rm -f qsort-pthd prodcons-pthd
