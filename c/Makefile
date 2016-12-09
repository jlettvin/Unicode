#!/usr/bin/env make
###############################################################################
MODS=PassFail Endian B64 Unicode Classify
LOPTS=-lbz2
COPTS=-O3 -Wall -Wextra -fPIC -g
SRCS=$(addsuffix .c,$(MODS)) $(addsuffix .h,$(MODS))
OBJS=$(addsuffix .o,$(MODS))
TESTS=$(addprefix test_,$(MODS))

###############################################################################
%.o	:	%.c
	@-gcc $(COPTS) -c -o $@ $<

%.out : %
	@./$< >$@ 2>&1

%.valgrind : %
	@-valgrind --leak-check=full ./$< > $@ 2>&1

###############################################################################
all:	start Makefile lint test valgrind results end


###############################################################################
.PHONY:
start:
	@echo 'vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv'

.PHONY:
clean:
	@-rm -f $(OBJS) $(TESTS) *.out *.valgrind

.PHONY:
results:
	@cat *.out | \
		grep -v "C-style cast" | \
		grep -v "2-space indent" | \
		grep -v "rather than the C type long" | \
		grep -v "should include its header"

.PHONY:
valgrind:	$(addsuffix .valgrind,$(TESTS))

.PHONY:
lint:	$(SRCS)
	@-cpplint $(SRCS) > lint.out 2>&1
	@echo "See file 'lint.out' to review ignored cpplint errors"

.PHONY:
test:	$(addsuffix .out,$(TESTS))

.PHONY:
end:
	@echo '^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^'


###############################################################################
test_PassFail.valgrind:	test_PassFail

test_PassFail.out:	test_PassFail

test_PassFail:	test_PassFail.c PassFail.o
	@-gcc -o $@ $^ $(LOPTS)

PassFail.o:	PassFail.c Makefile


###############################################################################
test_B64.valgrind:	test_B64

test_B64.out:	test_B64

test_B64:	test_B64.c B64.o PassFail.o
	@-gcc -o $@ $^ $(LOPTS)

B64.o:	B64.c Makefile



###############################################################################
test_Endian.valgrind:	test_Endian

test_Endian.out:	test_Endian

test_Endian:	test_Endian.c Endian.o PassFail.o
	@-gcc -o $@ $^ $(LOPTS)

Endian.o:	Endian.c Makefile



###############################################################################
test_Unicode.valgrind:	test_Unicode

test_Unicode.out:	test_Unicode

test_Unicode:	test_Unicode.c Unicode.o B64.o PassFail.o
	@-gcc -o $@ $^ $(LOPTS)

Unicode.o:	Unicode.c Makefile



###############################################################################
test_Classify.valgrind:	test_Classify

test_Classify.out:	test_Classify

test_Classify:	test_Classify.c Classify.o Unicode.o B64.o PassFail.o
	@-gcc -o $@ $^ $(LOPTS)

Classify.o:	Classify.c Makefile
