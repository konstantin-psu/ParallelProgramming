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

CURRENT=laplace-distr

CC = gcc
CHPL = chpl

all:  laplace laplace-shm laplace-distr

current: $(CURRENT)

run: $(CURRENT)
	./$(CURRENT) -nl $(RUN_ARGS)


laplace: laplace.c
	$(CC) -o laplace laplace.c -g -lm

laplace-shm: laplace-shm.chpl
	$(CHPL) -o laplace-shm laplace-shm.chpl

laplace-distr: laplace-distr.chpl
	$(CHPL) -o laplace-distr laplace-distr.chpl

clean:
	rm -f *_real laplace laplace-shm laplace-distr
