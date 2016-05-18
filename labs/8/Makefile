# Makefile for compiling Chapel programs
#
CHPL=chpl

.PHONY: locales
locales: locales.chpl
	$(CHPL) -o locales locales.chpl

.PHONY: locale1
locale1: locale1.chpl
	$(CHPL) -o locale1 locale1.chpl
# %:
# 	chpl -o $@ $@.chpl

clean:
	rm -f locales locales_real locale1 locale1_real
