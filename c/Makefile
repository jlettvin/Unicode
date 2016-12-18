#!/usr/bin/env make
# Makefile Copyright(c)2016 Jonathan D. Lettvin, All Rights Reserved.
###############################################################################

###############################################################################
# This makefile is typically run by the following command:
# 	$ make clean;clear;make
#
# It generates text as it executes demarcated by two lines of 'v' and '^':
#   vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
#   Generated output.
#   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#
# Output is generated in this sequence (see rule "all").
# 	doxygen:	generate documentation from source code comments.
# 	cpplint:  evaluate source for static quality control.
# 	test_*:	execute unit tests on each compiled library object.
# 	valgrind:	identify memory leaks (none due to this library).
# 	library:	generate a shared library for use by client programs.
# 	gcov:		assess coverage for each object.
# 	gprof:		assess efficiency for each object
# 	results:	review quality/compiler/test output for issues.
#
# Rules output target names and whether the controlling rule is "generic".
# Rules output timestamps to enable assessment of make progress.
#
# TODO(jlettvin): summarize gprof output on linux systems.
###############################################################################

# Identity of this repository subdirectory
IDENTITY=Unicode
VERSION=0.0.1
MODULES=PassFail Endian B64 UTF8 Classify Page
PYMODS=Classify Self

# Platform Specificity
UNAME := $(shell uname)

# Useful functions for keeping track of time and modules while running make.
timestamp=echo `date '+%Y/%m/%d %H:%M:%S'` $(1)
pipe:=|
empty:=
space:=$(empty) $(empty)
egreps="$(subst $(space),$(pipe),$(MODULES))"
line=__________________________________________________________________________

# Compiler options
SHARED_OPTS=-std=gnu11 -O3 -Wall -Wextra -fPIC
GOPTS=$(SHARED_OPTS) -O0 -g -coverage -pg
COPTS=$(SHARED_OPTS) -O3

# Useful sets of names
CSRCS=$(addsuffix .c,$(MODULES))
HSRCS=$(addsuffix .h,$(MODULES))
OBJS=$(addsuffix .o,$(MODULES))
TESTS=$(addprefix test_,$(MODULES))
GCOVS=$(addsuffix .gcov,$(CSRCS))
GPROFS=$(addsuffix .gprof,$(CSRCS))
SRCS=$(CSRCS) $(HSRCS)
PYSRCS=$(addsuffix .py,$(PYMODS))
PEP8S=$(addsuffix .pep8,$(PYMODS))
PYFLAKES=$(addsuffix .pyflakes,$(PYMODS))
PYLINTS=$(addsuffix .pylint,$(PYMODS))

# Useful runtime parameters
PEP8IGNORE=--ignore=E122,E128,E201,E202,E203,E221,E241,E266,E272

# Shared library generic information
CURDIR=.
MACDYLIB=libjlettvin.dylib
MACDYLIBOPT=

LINUXLIB=libjlettvin.so.$(VERSION)
LINUXLIBOPT=-L. -ljlettvin

# Platform specific shared library controls
ifeq ($(UNAME), Darwin)
LIBRARY=$(MACDYLIB)
LIBOPT=$(MACDYLIBOPT)
endif
ifeq ($(UNAME), Linux)
LIBRARY=$(LINUXLIB)
LIBOPT=$(LINUXLIBOPT)
endif

# Generic functions
# $(1) == $@
# $(2) == $<
# $(3) == list of .o files needed for link
define coverage
	@-gcc $(GOPTS) -c -o test_$(basename $(2)).o test_$(2)
	@-gcc $(GOPTS) -c -o      $(basename $(2)).o      $(2)
	@-gcc $(GOPTS)    -o gcov_test_$(basename $(2)) \
		test_$(basename $(2)).o \
		$(basename $(2)).o \
		$(3)
	@-./gcov_test_$(basename $(2))
	@-gcov $(2) >$(1).out 2>&1
	@$(call timestamp,$@ coverage)
endef

# Other specific platform funtions
ifeq ($(UNAME), Darwin)
define profile
	@-echo "No Mac gprof (ignoring $(2))" > $(2)
	@$(call timestamp,$@ profile)
endef
endif

ifeq ($(UNAME), Linux)
define profile
	@-gprof -b $(1) gmon.out > $(2) 2>&1
	@$(call timestamp,$@ profile)
endef
endif

###############################################################################
# generic rules

%.pylint : %.py
	@-pylint $< >$(basename $<).pylint 2>&1
	@$(call timestamp,$@)

%.pyflakes : %.py
	@-pyflakes $< >$(basename $<).pyflakes 2>&1
	@$(call timestamp,$@)

%.pep8 : %.py
	@-pep8 $(PEP8IGNORE) $< >$(basename $<).pep8 2>&1
	@$(call timestamp,$@)

# generic translation of c source files to library objects.
%.o	:	%.c
	@echo "\t$@ generic"
	@-gcc $(COPTS) -c -o $@ $<
	@$(call timestamp,$@)

# generic running of test_* executables
%.out : %
	@echo "\t$@ generic"
	@echo "\tGENERIC $@" >> PassFail.out
	@-./$< >$@ 2>&1
	@$(call timestamp,$@)

# generic production of valgrind output
%.valgrind : %
	@echo "\t$@ generic"
	@echo "\tGENERIC $@" >> PassFail.out
	@--valgrind --leak-check=full --show-leak-kinds=all ./$< > $@ 2>&1
	@$(call timestamp,$@)

###############################################################################
# Doing gcov after library avoids putting coverage objects in the library.
all: START graph doxygen lint $(LIBRARY) test valgrind gcov gprof results END
	@echo "\t$@ SUMMARY gcov percentages"
	@echo "TODO grep for percentages"
	@echo "grep '[0-9]%' $(GCOVS)"
	@$(call timestamp,$@)

.PHONY:
clean:
	@-rm -f \
		$(TESTS) \
		*.dylib \
		*gcda* \
		*gcno* \
		*gcov* \
		*gprof* \
		*.o \
		*.out \
		*.prof \
		*.pep8 \
		*.pyc \
		*.pyflakes \
		*.pylint \
		*.so \
		*.valgrind
	@$(call timestamp,$@)

###############################################################################
.PHONY:
START:
	@echo 'vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv'
	@$(call timestamp,$@ $(UNAME))

# https://github.com/lindenb/makefile2graph
# Modified with fputs("    rankdir=LR;\n",out);  ///< jlettvin 20161213
graph: Makefile START
	@make -Bnd | makefile2graph | dot -Tpng -o Makefile.png
	@$(call timestamp,$@)

doxygen:	graph
	@doxygen > doxygen.out 2>&1
	@$(call timestamp,$@)

lint:	$(SRCS) $(PEP8S) $(PYFLAKES) $(PYLINTS) doxygen
	@-cpplint $(SRCS) > lint.out 2>&1
	@echo "See file 'lint.out' to review ignored cpplint errors"
	@$(call timestamp,$@)

test:	$(addsuffix .out,$(TESTS))
	@$(call timestamp,$@)

valgrind:	$(addsuffix .valgrind,$(TESTS)) test
	@$(call timestamp,$@)

gprof: $(GPROFS) gcov

gcov: $(GCOVS) valgrind

results:	gprof
	@echo $(line)
	@echo "\tout out out out out out out out out out out out out out out out "
	@echo "\tPython QC"
	@cat -s *.pep8
	@cat -s *.pyflakes
	@cat -s *.pylint
	@echo "\trelevant .out file content"
	@cat *.out | \
		grep -v "C-style cast" | \
		grep -v "2-space indent" | \
		grep -v "rather than the C type long" | \
		grep -v "should include its header" | \
		grep -v '^[PASS]'
	@echo $(line)
	@echo "\tvalgrind valgrind valgrind valgrind valgrind valgrind valgrind"
	echo "\trelevant .valgrind file content"
	cat *.valgrind | egrep $(egreps)
	echo $(line)
	@$(call timestamp,$@)

END:	results
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
Classify.pep8:	Classify.py

Classify.pyflakes:	Classify.py

Classify.pylint:	Classify.py

Classify.py:	Self.py

Self.pep8:	Self.py

Self.pyflakes:	Self.py

Self.pylint:	Self.py

Self.py:	START

Unicode.h:	Types.h

###############################################################################
PassFail.c.gprof: PassFail.c
	@-$(call profile,gcov_test_$(basename $<),$@)
	@$(call timestamp,$@)

PassFail.c.gcov: PassFail.c
	@-$(call coverage,$@,$<,)
	@$(call timestamp,$@)

test_PassFail.valgrind:	test_PassFail

test_PassFail.out:	test_PassFail

test_PassFail:	test_PassFail.c PassFail.o
	@--gcc -o $@ $^
	@$(call timestamp,$@)

PassFail.o:	PassFail.c Makefile

PassFail.c:	PassFail.h Makefile


###############################################################################
Endian.c.gprof: Endian.c
	@-$(call profile,gcov_test_$(basename $<),$@)
	@$(call timestamp,$@)

Endian.c.gcov: Endian.c
	@-$(call coverage,$@,$<,PassFail.o)
	@$(call timestamp,$@)

test_Endian.valgrind:	test_Endian

test_Endian.out:	test_Endian

test_Endian:	test_Endian.c Endian.o PassFail.o
	@--gcc -o $@ $^
	@$(call timestamp,$@)

Endian.o:	Endian.c Makefile

Endian.c:	Endian.h Makefile

Endian.h:	Unicode.h Makefile


###############################################################################
B64.c.gprof: B64.c
	@-$(call profile,gcov_test_$(basename $<),$@)
	@$(call timestamp,$@)

B64.c.gcov: B64.c
	@-$(call coverage,$@,$<,PassFail.o)
	@$(call timestamp,$@)

test_B64.valgrind:	test_B64

test_B64.out:	test_B64

test_B64:	test_B64.c B64.o PassFail.o
	@--gcc -o $@ $^
	@$(call timestamp,$@)

B64.o:	B64.c Makefile

B64.c:	B64.h Makefile


###############################################################################
UTF8.c.gprof: UTF8.c
	@-$(call profile,gcov_test_$(basename $<),$@)
	@$(call timestamp,$@)

UTF8.c.gcov: UTF8.c
	@-$(call coverage,$@,$<,PassFail.o B64.o)
	@$(call timestamp,$@)

test_UTF8.valgrind:	test_UTF8

test_UTF8.out:	test_UTF8

test_UTF8:	test_UTF8.c UTF8.o PassFail.o
	@--gcc -o $@ $^
	@$(call timestamp,$@)

UTF8.o:	UTF8.c Makefile

UTF8.c:	UTF8.h Unicode.h Makefile


###############################################################################
# Classify

# Self.py implements access to the callers function/method name and __doc__
Classify.py:	Self.py

Classify.c.gprof: Classify.c
	@-$(call profile,gcov_test_$(basename $<),$@)
	@$(call timestamp,$@)

# Classify coverage and profiling use thusly named generic functions
Classify.c.gcov: Classify.c
	@-$(call coverage,$@,$<,PassFail.o B64.o UTF8.o)
	@$(call timestamp,$@)

# Memory leaks are detected by a generic rule
test_Classify.valgrind:	test_Classify

# Here we run the unit tests
test_Classify.out:	test_Classify

# This is how the the unit test is compiled
test_Classify:	test_Classify.c Classify.o PassFail.o
	@--gcc -o $@ $^
	@$(call timestamp,$@)

# Finally, this is how the shared library object is created
Classify.o:	Classify.c Classify.h Makefile

# Classify.py generates Classify.[ch] from DerivedGeneralCategory.txt
Classify.h Classify.c:	Classify.py Makefile
	@-./$<
	@$(call timestamp,$@)


###############################################################################
Page.c.gprof: Page.c
	@-$(call profile,gcov_test_$(basename $<),$@)
	@$(call timestamp,$@)

Page.c.gcov: Page.c
	@echo "TODO: make $@ appear normally"
	@touch $@
	@-$(call coverage,$@,$<,PassFail.o B64.o UTF8.o)
	@$(call timestamp,$@)

test_Page.valgrind:	test_Page

test_Page.out:	test_Page

test_Page:	test_Page.c Page.o PassFail.o
	@--gcc -o $@ $^
	@$(call timestamp,$@)

Page.o:	Page.c Makefile

Page.c:	Page.h Unicode.h
