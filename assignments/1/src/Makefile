#----------------------------------------------------------------------
# CS415/515 Assignment 1
#----------------------------------------------------------------------

FLAGS = -std=c99 -pthread -g -DVERBOSE -Wall -Werror
CC=gcc

.PHONY: qsort
qsort: qsort.c  task-queue.h task-queue.c
	$(CC) $(FLAGS) -o $@ qsort.c  task-queue.h task-queue.c

task-queue: task-queue.h task-queue.c
	$(CC) $(FLAGS) -o $@ task-queue.c task-queue.h

%: task-queue.h task-queue.c %.c 
	$(CC) $(FLAGS) -o $@ task-queue.c $@.c
