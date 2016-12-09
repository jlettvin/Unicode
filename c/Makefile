#!/usr/bin/env make
###############################################################################
MODULE=Unicode
VERSION=0.0.1
MODS=PassFail Endian B64 Unicode Classify

timestamp=echo `date '+%Y/%m/%d %H:%M:%S'` $(1)

CURDIR=.
COPTS=-O3 -Wall -Wextra -fPIC -g
CSRCS=$(addsuffix .c,$(MODS))
HSRCS=$(addsuffix .h,$(MODS))

SRCS=$(CSRCS) $(HSRCS)
OBJS=$(addsuffix .o,$(MODS))
TESTS=$(addprefix test_,$(MODS))
GCOVS=$(addprefix .gcov,$(CSRCS))

MACDYLIB=libjlettvin.dylib
MACDYLIBOPT=

LINUXLIB=libjlettvin.so.$(VERSION)
LINUXLIBOPT=-L. -ljlettvin

LIBRARY=$(MACDYLIB)
LIBOPT=$(MACDYLIBOPT)

###############################################################################
%.o	:	%.c
	@-gcc $(COPTS) -c -o $@ $<
	@$(call timestamp,$@)

%.out : %
	@./$< >$@ 2>&1
	@$(call timestamp,$@)

%.valgrind : %
	@-valgrind --leak-check=full ./$< > $@ 2>&1
	@$(call timestamp,$@)

###############################################################################
all:	start Makefile doxygen lint test valgrind gcov results $(LIBRARY) end
	@$(call timestamp,$@)


.PHONY:
clean:
	@-rm -f $(OBJS) $(TESTS) *.out *.valgrind *.gcov *.dylib
	@$(call timestamp,$@)

###############################################################################
.PHONY:
start:
	@echo 'vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv'
	@$(call timestamp,$@)

.PHONY:
doxygen:
	@doxygen > doxygen.out 2>&1
	@$(call timestamp,$@)

.PHONY:
lint:	$(SRCS)
	@-cpplint $(SRCS) > lint.out 2>&1
	@echo "See file 'lint.out' to review ignored cpplint errors"
	@$(call timestamp,$@)

.PHONY:
test:	$(addsuffix .out,$(TESTS))
	@$(call timestamp,$@)

.PHONY:
valgrind:	$(addsuffix .valgrind,$(TESTS))
	@$(call timestamp,$@)

.PHONY:
gcov:
	@echo "Implement gcov execution"
	@echo "See: https://gcc.gnu.org/onlinedocs/gcc/Gcov-Intro.html"
	@echo "See: https://gcc.gnu.org/onlinedocs/gcc/Gcov.html"

.PHONY:
results:
	@cat *.out | \
		grep -v "C-style cast" | \
		grep -v "2-space indent" | \
		grep -v "rather than the C type long" | \
		grep -v "should include its header"
	@$(call timestamp,$@)

.PHONY:
end:
	@$(call timestamp,$@)
	@echo '^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^'


# shared library ##############################################################

$(MACDYLIB):	$(OBJS)
	@g++ \
		-dynamiclib -undefined suppress -flat_namespace \
		-install_name '$(CURDIR)/$(MACDYLIB)' -current_version $(VERSION) \
		$^ \
		-o $@
	@$(call timestamp,$@)

$(LINUXLIB):	$(OBJS)
	@g++ -shared -Wl,-soname,libjlettvin.so.1 -lc $^ -o $@
	@$(call timestamp,$@)


###############################################################################
test_PassFail.valgrind:	test_PassFail

test_PassFail.out:	test_PassFail

test_PassFail:	test_PassFail.c PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

PassFail.o:	PassFail.c Makefile


###############################################################################
test_B64.valgrind:	test_B64

test_B64.out:	test_B64

test_B64:	test_B64.c B64.o PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

B64.o:	B64.c Makefile



###############################################################################
test_Endian.valgrind:	test_Endian

test_Endian.out:	test_Endian

test_Endian:	test_Endian.c Endian.o PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

Endian.o:	Endian.c Makefile



###############################################################################
test_Unicode.valgrind:	test_Unicode

test_Unicode.out:	test_Unicode

test_Unicode:	test_Unicode.c Unicode.o B64.o PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

Unicode.o:	Unicode.c Makefile



###############################################################################
test_Classify.valgrind:	test_Classify

test_Classify.out:	test_Classify

test_Classify:	test_Classify.c Classify.o Unicode.o B64.o PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

Classify.o:	Classify.c Makefile
