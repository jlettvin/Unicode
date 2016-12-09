#!/usr/bin/env make
#-Werror
SHARE=-fPIC
LOPTS=-lbz2
COPTS=-O3 -Wall -Wextra -g
TESTS=test_B64 test_Unicode test_Classify
SRCS=\
	 passfail.h \
	 passfail.c \
	 B64.h \
	 B64.c \
	 Unicode.h \
	 Unicode.c \
	 Classify.h \
	 Classify.c
OBJS=passfail.o B64.o Unicode.o Classify.o

%.o	:	%.c
	-gcc $(COPTS) -c -o $@ $<

all:	start Makefile test lint valgrind results end

.PHONY:
start:
	@echo 'vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv'

.PHONY:
end:
	@echo '^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^'

.PHONY:
clean:
	@-rm -f $(OBJS) $(TESTS) *.out *.valgrind

.PHONY:
results:
	@cat *.out | grep -v "C-style cast" | grep -v "should include its header"

.PHONY:
valgrind:	test_B64.valgrind test_Unicode.valgrind test_Classify.valgrind

test_B64.valgrind:	test_B64
	@-valgrind --leak-check=full ./$< > $@ 2>&1

test_Unicode.valgrind:	test_Unicode
	@-valgrind --leak-check=full ./$< > $@ 2>&1

test_Classify.valgrind:	test_Classify
	@-valgrind --leak-check=full ./$< > $@ 2>&1

.PHONY:
lint:	$(SRCS)
	-cpplint $(SRCS) > lint.out 2>&1

.PHONY:
test:	test_B64.out test_Unicode.out test_Classify.out

test_B64.out:	test_B64
	@./$< >$@

test_Unicode.out:	test_Unicode
	@./$< >$@

test_Classify.out:	test_Classify
	@./$< >$@

test_B64:	test_B64.c B64.o passfail.o
	-gcc -o $@ $^ $(LOPTS)

test_Unicode:	test_Unicode.c Unicode.o B64.o passfail.o
	-gcc -o $@ $^ $(LOPTS)

test_Classify:	test_Classify.c Classify.o Unicode.o B64.o passfail.o
	-gcc -o $@ $^ $(LOPTS)

passfail.o:	passfail.c Makefile

B64.o:	B64.c Makefile

Unicode.o:	Unicode.c Makefile

Classify.o:	Classify.c Makefile
