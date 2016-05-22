# Makefile for compiling Chapel programs
 


# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

#CURRENT=locales
CURRENT=border

CHPL=chpl

all: locales locale1 locale2 locale3 locale4 dmap1 dmap2 border

run: $(CURRENT)
	./$(CURRENT) -nl $(RUN_ARGS)

locales: locales.chpl
	$(CHPL) -o locales locales.chpl

locale1: locale1.chpl
	$(CHPL) -o locale1 locale1.chpl

locale2: locale2.chpl
	$(CHPL) -o locale2 locale2.chpl

locale3: locale3.chpl
	$(CHPL) -o locale3 locale3.chpl

locale4: locale4.chpl
	$(CHPL) -o locale4 locale4.chpl

dmap1: dmap1.chpl
	$(CHPL) -o dmap1 dmap1.chpl

dmap2: dmap2.chpl
	$(CHPL) -o dmap2 dmap2.chpl

border: border.chpl
	$(CHPL) -o border border.chpl
# %:
# 	chpl -o $@ $@.chpl

clean:
	rm -f *_real locales locale[1-4] dmap[1-4] border
