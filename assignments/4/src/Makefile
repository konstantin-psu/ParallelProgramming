#----------------------------------------------------------------------
# CS415/515 Assignment 4
#----------------------------------------------------------------------

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

CURRENT=jacobi-shm

CC = gcc
CHPL = chpl

current: $(CURRENT)

all:  jacobi laplace jacobi-shm0 jacobi-shm laplace-shm laplace-distr

run: $(CURRENT)
	./$(CURRENT) -nl $(RUN_ARGS)

jacobi: jacobi.c
	$(CC) -o jacobi jacobi.c -g -lm

laplace: laplace.c
	$(CC) -o laplace laplace.c -g -lm

jacobi-shm0: jacobi-shm0.chpl
	$(CHPL) -o jacobi-shm0 jacobi-shm0.chpl

jacobi-shm: jacobi-shm.chpl
	$(CHPL) -o jacobi-shm jacobi-shm.chpl

laplace-shm: laplace-shm.chpl
	$(CHPL) -o laplace-shm laplace-shm.chpl

laplace-distr: laplace-distr.chpl
	$(CHPL) -o laplace-distr laplace-distr.chpl

clean:
	rm -f *_real jacobi laplace jacobi-shm0 jacobi-shm laplace-shm laplace-distr
