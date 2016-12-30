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
TESTS=$(addprefix test_,$(MODULES))
GCOVS=$(addsuffix .gcov,$(CSRCS))
GPROFS=$(addsuffix .gprof,$(CSRCS))
PYSRCS=$(addsuffix .py,$(PYMODS))
PEP8S=$(addsuffix .pep8,$(PYMODS))
PYFLAKES=$(addsuffix .pyflakes,$(PYMODS))
PYLINTS=$(addsuffix .pylint,$(PYMODS))

OBJS=$(addsuffix .o,$(MODULES))
CSRCS=$(addsuffix .c,$(MODULES))
HSRCS=$(addsuffix .h,$(MODULES))
SRCS=$(CSRCS) $(HSRCS)

# Useful runtime parameters
PEP8IGNORE=--ignore=E122,E128,E201,E202,E203,E221,E241,E265,E266,E272

# Shared library generic information
CURDIR=.
MACDYLIB=libjlettvin.dylib
MACDYLIBOPT=

LINUXLIB=libjlettvin.so.$(VERSION)
LINUXLIBOPT=-L. -ljlettvin

# Valgrind support
#  --leak-check=full
#  --show-reachable=yes
#  --error-limit=no
#  --gen-suppressions=all
# https://wiki.wxwidgets.org/Parse_valgrind_suppressions.sh 
VALGRINDOPTS=\
			 --error-limit=no \
			 --leak-check=full \
			 --show-reachable=yes \
			 --show-leak-kinds=all \
			 --gen-suppressions=all \
			 --suppressions=./valgrind.supp

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
	@$(call T0,$(2),coverage for $(1))
	@-gcc $(GOPTS) -c -o test_$(basename $(2)).o test_$(2)
	@-gcc $(GOPTS) -c -o      $(basename $(2)).o      $(2)
	@-gcc $(GOPTS)    -o gcov_test_$(basename $(2)) \
		test_$(basename $(2)).o \
		$(basename $(2)).o \
		$(3)
	@-./gcov_test_$(basename $(2))
	@-gcov $(2) >$(1).out 2>&1
	@$(call T1,$(2))
endef

define T0
	@./MakeTime.py $(1) $(2)
endef

define T1
	@./MakeTime.py $(1)
endef

# Other specific platform funtions
ifeq ($(UNAME), Darwin)
define profile
	@$(call T0,$(2),profile for $(1))
	@-echo "No Mac gprof (ignoring $(2))" > $(2)
	@$(call T1,$(2))
endef
endif

ifeq ($(UNAME), Linux)
define profile
	@$(call T0,$(2),profile for $(1))
	@-gprof -b $(1) gmon.out > $(2) 2>&1
	@$(call T1,$(2))
endef
endif

###############################################################################
# generic rules

%.pylint : %.py
	@$(call T0,$@,generic rule)
	@-pylint $< >$(basename $<).pylint 2>&1
	@$(call T1,$@)

%.pyflakes : %.py
	@$(call T0,$@,generic rule)
	@-pyflakes $< >$(basename $<).pyflakes 2>&1
	@$(call T1,$@)

%.pep8 : %.py
	@$(call T0,$@,generic rule)
	@-pep8 $(PEP8IGNORE) $< >$(basename $<).pep8 2>&1
	@$(call T1,$@)

# generic translation of c source files to library objects.
%.o	:	%.c
	@$(call T0,$@,generic rule)
	@echo "\t$@ generic"
	@-gcc $(COPTS) -c -o $@ $<
	@$(call T1,$@)

# generic running of test_* executables
%.out : %
	@$(call T0,$@,generic rule)
	@echo "\t$@ generic"
	@echo "\tGENERIC $@" >> PassFail.out
	@-./$< >$@ 2>&1
	@$(call T1,$@)

# generic production of valgrind output
%.valgrind : %
	@$(call T0,$@,generic rule)
	@echo "\t$@ generic"
	@echo "\tGENERIC $@" >> PassFail.out
	@--valgrind $(VALGRINDOPTS) ./$< > $@ 2>&1
	@$(call T1,$@)


test_% :	test_%.c %.o
	@--gcc -o $@ $^

###############################################################################
# Doing gcov after library avoids putting coverage objects in the library.
all: START graph doxygen lint $(LIBRARY) test valgrind gcov gprof results END
	@echo "\t$@ SUMMARY gcov percentages"
	@grep '[0-9]%' *.out
	@$(call timestamp,Makefile execution finished timestamp)
	@./DeBrief.py

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
		*.pep8 \
		*.pyc \
		*.pyflakes \
		*.pylint \
		*.so \
		*.valgrind
	@./MakeTime.py --initialize

.PHONY:
clear:
	@clear

.PHONY:
total: clean clear all

###############################################################################
.PHONY:
START:
	@./MakeTime.py --initialize
	@./MakeTime.py $@ Elapsed time for entire make
	@echo 'vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv'

# https://github.com/lindenb/makefile2graph
# Modified with fputs("    rankdir=LR;\n",out);  ///< jlettvin 20161213
graph: Makefile START
	@$(call T0,$@,compound rule)
	@make -Bnd | makefile2graph | dot -Tpng -o Makefile.png
	@$(call T1,$@)

doxygen:	graph
	@$(call T0,$@,compound rule)
	@doxygen > doxygen.out 2>&1
	@$(call T1,$@)

lint:	$(SRCS) $(PEP8S) $(PYFLAKES) $(PYLINTS) doxygen
	@$(call T0,$@,compound rule)
	@-cpplint $(SRCS) > lint.out 2>&1
	@echo "See file 'lint.out' to review ignored cpplint errors"
	@$(call T1,$@)

test:	$(addsuffix .out,$(TESTS))

valgrind:	$(addsuffix .valgrind,$(TESTS)) test

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

END:	results
	@./MakeTime.py START
	@echo "Elapsed time in order of t0 initialization time:"
	@./MakeTime.py --report
	@echo '^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^'


# shared library ##############################################################

$(MACDYLIB):	$(OBJS)
	@$(call T0,$@,special rule)
	@g++ \
		-dynamiclib -undefined suppress -flat_namespace \
		-install_name '$(CURDIR)/$(MACDYLIB)' -current_version $(VERSION) \
		$^ \
		-o $@
	@$(call T1,$@)

$(LINUXLIB):	$(OBJS)
	@$(call T0,$@,special rule)
	@g++ -shared -Wl,-soname,libjlettvin.so.1 -lc $^ -o $@
	@$(call T1,$@)


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

PassFail.c.gcov: PassFail.c
	@-$(call coverage,$@,$<,)

test_PassFail.valgrind:	test_PassFail

test_PassFail.out:	test_PassFail

#test_PassFail:	test_PassFail.c PassFail.o
#	@$(call T0,$@,$@)
#	@--gcc -o $@ $^
#	@$(call T1,$@)

PassFail.o:	PassFail.c Makefile

PassFail.c:	PassFail.h Makefile


###############################################################################
Endian.c.gprof: Endian.c
	@-$(call profile,gcov_test_$(basename $<),$@)

Endian.c.gcov: Endian.c
	@-$(call coverage,$@,$<,PassFail.o)

test_Endian.valgrind:	test_Endian

test_Endian.out:	test_Endian

test_Endian:	test_Endian.c Endian.o PassFail.o
	@$(call T0,$@,$@)
	@--gcc -o $@ $^
	@$(call T1,$@)

Endian.o:	Endian.c Makefile

Endian.c:	Endian.h Makefile

Endian.h:	Unicode.h Makefile


###############################################################################
B64.c.gprof: B64.c
	@-$(call profile,gcov_test_$(basename $<),$@)

B64.c.gcov: B64.c
	@-$(call coverage,$@,$<,PassFail.o)

test_B64.valgrind:	test_B64

test_B64.out:	test_B64

test_B64:	test_B64.c B64.o PassFail.o
	@$(call T0,$@,$@)
	@--gcc -o $@ $^
	@$(call T1,$@)

B64.o:	B64.c Makefile

B64.c:	B64.h Makefile


###############################################################################
UTF8.c.gprof: UTF8.c
	@-$(call profile,gcov_test_$(basename $<),$@)

UTF8.c.gcov: UTF8.c
	@-$(call coverage,$@,$<,PassFail.o B64.o)

test_UTF8.valgrind:	test_UTF8

test_UTF8.out:	test_UTF8

test_UTF8:	test_UTF8.c UTF8.o PassFail.o
	@$(call T0,$@,$@)
	@--gcc -o $@ $^
	@$(call T1,$@)

UTF8.o:	UTF8.c Makefile

UTF8.c:	UTF8.h Unicode.h Makefile


###############################################################################
# Classify

# Self.py implements access to the callers function/method name and __doc__
Classify.py:	Self.py

Classify.c.gprof: Classify.c
	@-$(call profile,gcov_test_$(basename $<),$@)

# Classify coverage and profiling use thusly named generic functions
Classify.c.gcov: Classify.c
	@-$(call coverage,$@,$<,PassFail.o B64.o UTF8.o)

# Memory leaks are detected by a generic rule
test_Classify.valgrind:	test_Classify

# Here we run the unit tests
test_Classify.out:	test_Classify

# This is how the the unit test is compiled
test_Classify:	test_Classify.c Classify.o PassFail.o
	@$(call T0,$@,$@)
	@--gcc -o $@ $^
	@$(call T1,$@)

# Finally, this is how the shared library object is created
Classify.o:	Classify.c Classify.h Makefile

# Classify.py generates Classify.[ch] from DerivedGeneralCategory.txt
Classify.h Classify.c:	Classify.py Makefile
	@$(call T0,$@,Generate Classify.[ch])
	@-./$<
	@$(call T1,$@)


###############################################################################
Page.c.gprof: Page.c
	@-$(call profile,gcov_test_$(basename $<),$@)

Page.c.gcov: Page.c
	@echo "TODO: make $@ appear normally"
	@touch $@
	@-$(call coverage,$@,$<,PassFail.o B64.o UTF8.o)

test_Page.valgrind:	test_Page

test_Page.out:	test_Page

test_Page:	test_Page.c Page.o PassFail.o
	@$(call T0,$@,$@)
	@--gcc -o $@ $^
	@$(call T1,$@)

Page.o:	Page.c Makefile

Page.c:	Page.h Unicode.h
