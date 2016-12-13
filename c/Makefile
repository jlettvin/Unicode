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
# 	results:	review quality/compiler/test output for issues.
#
# Rules output target names and whether the controlling rule is "generic".
# Rules output timestamps to enable assessment of make progress.
###############################################################################

# Identity of this repository subdirectory
IDENTITY=Unicode
VERSION=0.0.1
MODULES=PassFail Endian B64 UTF8 Classify Page

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
SHARED_OPTS=-std=gnu11 -o3 -Wall -Wextra -fPIC
GOPTS=$(SHARED_OPTS) -O0 -g -coverage -pg
COPTS=$(SHARED_OPTS) -O3

# Usefule sets of names
CSRCS=$(addsuffix .c,$(MODULES))
HSRCS=$(addsuffix .h,$(MODULES))
OBJS=$(addsuffix .o,$(MODULES))
TESTS=$(addprefix test_,$(MODULES))
GCOVS=$(addsuffix .gcov,$(CSRCS))
SRCS=$(CSRCS) $(HSRCS)

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
# $(3) == $(basename $(2))
# $(4) == list of .o files needed for link
define coverage2
	echo "$(1),$(2),$(3),$(4)"
	gcc $(GOPTS) -c -o test_$(3).o test_$(2)
	gcc $(GOPTS) -c -o      $(3).o      $(2)
	gcc $(GOPTS)    -o gcov_test_$(3) test_$(3).o $(3).o $(4)
	./gcov_test_$(3)
	gcov $(2) >$(1).out 2>&1
endef

define coverage
	./gcov_test_$(basename $(2))
	gcov $(2) >$(1).out 2>&1
endef

# Other specific platform funtions
ifeq ($(UNAME), Darwin)
define profile
	echo "No Mac gprof"
endef
endif

ifeq ($(UNAME), Linux)
define profile
	gprof -b $(1) gmon.out > $(2) 2>&1
endef
endif

###############################################################################
# generic rules

# generic translation of c source files to library objects.
%.o	:	%.c
	@echo "\t$@ generic"
	@-gcc $(COPTS) -c -o $@ $<
	@$(call timestamp,$@)

# generic running of test_* executables
%.out : %
	@echo "\t$@ generic"
	@./$< >$@ 2>&1
	@$(call timestamp,$@)

# generic production of valgrind output
%.valgrind : %
	@echo "\t$@ generic"
	@-valgrind --leak-check=full --show-leak-kinds=all ./$< > $@ 2>&1
	@$(call timestamp,$@)

# generic production of coverage files (unused)
%.gcov : %
	@echo "\t$@ generic (needs work)"
	@-gcc $(GOPTS) -c -o test_$(basename $<).o test_$<
	@-gcc $(GOPTS) -c -o      $(basename $<).o      $<
	@-gcc $(GOPTS)    -o gcov_test_$(basename $<) \
		test_$(basename $<).o \
		$(basename $<).o \
		-L . -l $(LIBRARY)
	@-./gcov_test_$(basename $<)
	@-gcov $< >$@.out 2>&1
	@-$(call profile,gcov_test_$(basename $<),$<.prof)
	@$(call timestamp,$@)

###############################################################################
# Doing gcov after library avoids putting coverage objects in the library.
all:	start doxygen lint test valgrind $(LIBRARY) gcov results end
	@echo "\t$@ SUMMARY gcov percentages"
	@grep '%' *.gcov.out
	@$(call timestamp,$@)


.PHONY:
clean:
	@-rm -f \
		$(TESTS) \
		*.o \
		*.out \
		*.valgrind \
		*gcov* \
		*gcda* \
		*gcno* \
		*.prof \
		*.dylib \
		*.so
	@$(call timestamp,$@)

###############################################################################
.PHONY:
start:
	@echo 'vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv'
	@$(call timestamp,$@ $(UNAME))

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
	@echo $(line)
	@echo "\tout out out out out out out out out out out out out out out out "
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
	@echo "\t$@ and gprof(on linux)"
	@-$(call coverage2,$@,$<,$(basename $<),)
	@-$(call profile,gcov_test_$(basename $<),$<.prof)
	@$(call timestamp,$@)

test_PassFail.valgrind:	test_PassFail

test_PassFail.out:	test_PassFail

test_PassFail:	test_PassFail.c PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

PassFail.o:	PassFail.c Makefile


###############################################################################
B64.c.gcov: B64.c
	@echo "\t$@ and gprof(on linux)"
	@-$(call coverage2,$@,$<,$(basename $<),PassFail.o)
	@-$(call profile,gcov_test_$(basename $<),$<.prof)
	@$(call timestamp,$@)

test_B64.valgrind:	test_B64

test_B64.out:	test_B64

test_B64:	test_B64.c B64.o PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

B64.o:	B64.c Makefile



###############################################################################
Endian.c.gcov: Endian.c
	@echo "\t$@ and gprof(on linux)"
	@-$(call coverage2,$@,$<,$(basename $<),PassFail.o)
	@-$(call profile,gcov_test_$(basename $<),$<.prof)
	@$(call timestamp,$@)

test_Endian.valgrind:	test_Endian

test_Endian.out:	test_Endian

test_Endian:	test_Endian.c Endian.o PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

Endian.o:	Endian.c Makefile



###############################################################################
UTF8.c.gcov: UTF8.c
	@echo "\t$@ and gprof(on linux)"
	@-$(call coverage2,$@,$<,$(basename $<),PassFail.o B64.o)
	@-$(call profile,gcov_test_$(basename $<),$<.prof)
	@$(call timestamp,$@)

test_UTF8.valgrind:	test_UTF8

test_UTF8.out:	test_UTF8

test_UTF8:	test_UTF8.c UTF8.o PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

UTF8.o:	UTF8.c Makefile



###############################################################################
Classify.c.gcov: Classify.c
	@echo "\t$@ and gprof(on linux)"
	@-$(call coverage2,$@,$<,$(basename $<),PassFail.o B64.o UTF8.o)
	@-$(call profile,gcov_test_$(basename $<),$<.prof)
	@$(call timestamp,$@)

test_Classify.valgrind:	test_Classify

test_Classify.out:	test_Classify

test_Classify:	test_Classify.c Classify.o PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

Classify.o:	Classify.c Makefile


###############################################################################
Page.c.gcov: Page.c
	@echo "\t$@ and gprof(on linux)"
	@-$(call coverage2,$@,$<,$(basename $<),PassFail.o B64.o UTF8.o)
	@-$(call profile,gcov_test_$(basename $<),$<.prof)
	@$(call timestamp,$@)

test_Page.valgrind:	test_Page

test_Page.out:	test_Page

test_Page:	test_Page.c Page.o PassFail.o
	@-gcc -o $@ $^
	@$(call timestamp,$@)

Page.o:	Page.c Makefile
