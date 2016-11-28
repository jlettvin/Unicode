#!/usr/bin/env make

# TODO use strip on library executables.

timestamp=echo `date '+%Y/%m/%d %H:%M:%S'` $(1)
COPTS=-std=c++11 -O3 -Wall -Wextra -Werror


# all #########################################################################
.PHONY:
all:	test_Node.out lint valgrind
	@$(call timestamp,$@)


# clean #######################################################################
.PHONY:
clean:
	@echo "\t$@"
	@rm -f *.o *.lint *.valgrind test_Node test_Node.out
	@rm -fr test_Node.dSym
	@$(call timestamp,$@)


# metrics #####################################################################
test_Node.out:	test_Node
	@-./$< > $@ 2>&1
	@tail -1 $@
	@$(call timestamp,$@)

valgrind:	test_Node.cpp.valgrind
	@$(call timestamp,$@)

lint:	Node.h.lint Node.cpp.lint test_Node.cpp.lint
	@$(call timestamp,$@)


# unit tests ##################################################################
test_Node:	test_Node.cpp Node.o
	@g++ $(COPTS) -o $@ $^
	@$(call timestamp,$@)


# metrics break out ###########################################################
test_Node.cpp.valgrind:	test_Node.cpp
	@-valgrind --error-exitcode=0 ./$< 2>&1 | \
		grep "ERROR SUMMARY" | \
		cut -d' ' -f2- > \
		$@
	@$(call timestamp,$@)

Node.cpp.lint:	Node.cpp
	@-cpplint $< 2>&1 | grep -v "should include its header" > $@
	@$(call timestamp,$@)

Node.h.lint:	Node.h
	@-cpplint $< 2>&1 | grep -v "should include its header" > $@
	@$(call timestamp,$@)

test_Node.cpp.lint:	test_Node.cpp
	@-cpplint $< 2>&1 | grep -v "should include its header" > $@
	@$(call timestamp,$@)

# object files ################################################################
Node.o:	Node.cpp
	@g++ $(COPTS) -c -o $@ $<
	@$(call timestamp,$@)
