#!/usr/bin/env make
###############################################################################
MODULE=Unicode
VERSION=0.0.1
MODS=PassFail Endian B64 Unicode Classify

timestamp=echo `date '+%Y/%m/%d %H:%M:%S'` $(1)

CURDIR=.
OOPTS=-O0 -Wall -Wextra -fPIC
GOPTS=$(OOPTS) -g -coverage -pg
COPTS=$(OOPTS)
CSRCS=$(addsuffix .c,$(MODS))
HSRCS=$(addsuffix .h,$(MODS))

SRCS=$(CSRCS) $(HSRCS)
OBJS=$(addsuffix .o,$(MODS))
TESTS=$(addprefix test_,$(MODS))
GCOVS=$(addsuffix .gcov,$(CSRCS))

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
# Doing gcov after library avoids putting coverage objects in the library.
all:	start Makefile doxygen lint test valgrind results $(LIBRARY) gcov end
	@$(call timestamp,$@)


.PHONY:
clean:
	@-rm -f $(TESTS) *.o *.out *.valgrind *gcov* *gcda* *gcno* *.prof *.dylib
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
gcov: $(GCOVS)

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
PassFail.c.gcov: PassFail.c
	@-gcc $(GOPTS) -c -o test_$(basename $<).o test_$<
	@-gcc $(GOPTS) -c -o      $(basename $<).o      $<
	@-gcc $(GOPTS)    -o gcov_test_$(basename $<) \
		test_$(basename $<).o \
		$(basename $<).o
	@-./gcov_test_$(basename $<)
	@-gcov $< >$@.out 2>&1
	@echo "no Mac gprof: -gprof gcov_test_$(basename $<) gmon.out > $<.prof"
	@$(call timestamp,$@)

test_PassFail.valgrind:	test_PassFail

test_PassFail.out:	test_PassFail

test_PassFail:	test_PassFail.c PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

PassFail.o:	PassFail.c Makefile


###############################################################################
B64.c.gcov: B64.c
	@-gcc $(GOPTS) -c -o test_$(basename $<).o test_$<
	@-gcc $(GOPTS) -c -o      $(basename $<).o      $<
	@-gcc $(GOPTS)    -o gcov_test_$(basename $<) \
		test_$(basename $<).o \
		$(basename $<).o \
		PassFail.o
	@-./gcov_test_$(basename $<)
	@-gcov $< >$@.out 2>&1
	@echo "no Mac gprof: -gprof gcov_test_$(basename $<) gmon.out > $<.prof"
	@$(call timestamp,$@)

test_B64.valgrind:	test_B64

test_B64.out:	test_B64

test_B64:	test_B64.c B64.o PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

B64.o:	B64.c Makefile



###############################################################################
Endian.c.gcov: Endian.c
	@-gcc $(GOPTS) -c -o test_$(basename $<).o test_$<
	@-gcc $(GOPTS) -c -o      $(basename $<).o      $<
	@-gcc $(GOPTS)    -o gcov_test_$(basename $<) \
		test_$(basename $<).o \
		$(basename $<).o \
		PassFail.o
	@-./gcov_test_$(basename $<)
	@-gcov $< >$@.out 2>&1
	@echo "no Mac gprof: -gprof gcov_test_$(basename $<) gmon.out > $<.prof"
	@$(call timestamp,$@)

test_Endian.valgrind:	test_Endian

test_Endian.out:	test_Endian

test_Endian:	test_Endian.c Endian.o PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

Endian.o:	Endian.c Makefile



###############################################################################
Unicode.c.gcov: Unicode.c
	@-gcc $(GOPTS) -c -o test_$(basename $<).o test_$<
	@-gcc $(GOPTS) -c -o      $(basename $<).o      $<
	@-gcc $(GOPTS)    -o gcov_test_$(basename $<) \
		test_$(basename $<).o \
		$(basename $<).o \
		B64.o PassFail.o
	@-./gcov_test_$(basename $<)
	@-gcov $< >$@.out 2>&1
	@echo "no Mac gprof: -gprof gcov_test_$(basename $<) gmon.out > $<.prof"
	@$(call timestamp,$@)

test_Unicode.valgrind:	test_Unicode

test_Unicode.out:	test_Unicode

test_Unicode:	test_Unicode.c Unicode.o B64.o PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

Unicode.o:	Unicode.c Makefile



###############################################################################
Classify.c.gcov: Classify.c
	@-gcc $(GOPTS) -c -o test_$(basename $<).o test_$<
	@-gcc $(GOPTS) -c -o      $(basename $<).o      $<
	@-gcc $(GOPTS)    -o gcov_test_$(basename $<) \
		test_$(basename $<).o \
		$(basename $<).o \
		PassFail.o B64.o Unicode.o
	@-./gcov_test_$(basename $<)
	@-gcov $< >$@.out 2>&1
	@echo "no Mac gprof: -gprof gcov_test_$(basename $<) gmon.out > $<.prof"
	@$(call timestamp,$@)

test_Classify.valgrind:	test_Classify

test_Classify.out:	test_Classify

test_Classify:	test_Classify.c Classify.o Unicode.o B64.o PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

Classify.o:	Classify.c Makefile
