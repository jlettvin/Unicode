#!/usr/bin/env make
SHARE=-fPIC
LOPTS=-lbz2
COPTS=-O3 -Wall -Wextra -Werror -DDECOMPRESS -g
TESTS=test_B64
SRCS=\
	 passfail.h \
	 passfail.c \
	 B64.h \
	 B64.c \
	 Unicode.h \
	 Unicode.c
OBJS=passfail.o B64.o Unicode.o
EXES=Unicode

%.o	:	%.c
	@-gcc $(COPTS) -c -o $@ $<

all:	start Makefile Unicode test lint valgrind end

.PHONY:
start:
	@echo 'vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv'

.PHONY:
end:
	@echo '^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^'

.PHONY:
clean:
	@-rm -f $(EXES) $(OBJS) $(TESTS) *.out *.valgrind

.PHONY:
valgrind:	test_B64.valgrind

test_B64.valgrind:	test_B64
	@-valgrind --leak-check=full ./$< > $@ 2>&1

.PHONY:
lint:	$(SRCS)
	@-cpplint $(SRCS) > lint.out 2>&1

.PHONY:
test:	test_B64

test_B64:	test_B64.c B64.o passfail.o
	@-gcc -o $@ $^ $(LOPTS)
	@-./$@

Unicode:	B64.o Unicode.o passfail.o
	@-gcc -o $@ $^ $(LOPTS)
	@-./$@

passfail.o:	passfail.c Makefile

B64.o:	B64.c Makefile

Unicode.o:	Unicode.c Makefile
