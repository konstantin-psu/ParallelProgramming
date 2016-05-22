# Makefile for compiling Chapel programs
#

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

CH=chpl

FLAGS = -std=c99 -pthread -g -DVERBOSE -Wall -Werror
CC=gcc

#CURRENT=task-queue
#CURRENT=prodcons
CURRENT=sum-chpl

run: $(CURRENT)
	./$(CURRENT) -nl $(RUN_ARGS)

all: hello prodcons task-queue sum-pthd sum-chpl

hello: hello.chpl
	$(CH) -o hello hello.chpl

prodcons: prodcons.chpl
	$(CH) -o prodcons prodcons.chpl

task-queue: task-queue.chpl
	$(CH) -o task-queue task-queue.chpl

sum-chpl: sum-chpl.chpl
	$(CH) -o sum-chpl sum-chpl.chpl

sum-pthd: sum-pthd.c
	$(CC) $(FLAGS) -o sum-pthd sum-pthd.c



clean:
	rm -f hello hello_real prodcons prodcons_real task-queue task-queue_real sum-pthd sum-chpl sum-chpl_real
