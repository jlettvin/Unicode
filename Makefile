#!/usr/bin/env make

# make clean; clear; make

# TODO use strip on library executables.

timestamp=echo `date '+%Y/%m/%d %H:%M:%S'` $(1)
COPTS=-std=c++11 -O3 -Wall -Wextra -Werror


# all #########################################################################
.PHONY:
all:	test_Node.out test_Tree.out lint valgrind doxygen
	@$(call timestamp,$@)


# clean #######################################################################
.PHONY:
clean:
	@rm -f *.o *.out *.lint *.valgrind
	@rm -f test_Node test_Tree
	@rm -fr test_Node.dSym
	@$(call timestamp,$@)


# documentation ###############################################################
.PHONY:
doxygen:
	@doxygen > doxygen.out 2>&1
	@$(call timestamp,$@)


# metrics #####################################################################
test_Node.out:	test_Node
	@-./$< > $@ 2>&1
	@tail -1 $@
	@$(call timestamp,$@)

test_Tree.out:	test_Tree
	@-./$< > $@ 2>&1
	@tail -1 $@
	@$(call timestamp,$@)

valgrind:	test_Node.cpp.valgrind test_Tree.cpp.valgrind
	@$(call timestamp,$@)

lint:	\
	Node.h.lint Node.cpp.lint test_Node.cpp.lint \
	Tree.h.lint Tree.cpp.lint test_Tree.cpp.lint
	@$(call timestamp,$@)


# unit tests ##################################################################
test_Node:	test_Node.cpp Node.o
	@g++ $(COPTS) -o $@ $^
	@$(call timestamp,$@)

test_Tree:	test_Tree.cpp Tree.o Node.o
	@g++ $(COPTS) -o $@ $^
	@$(call timestamp,$@)


# metrics break out ###########################################################
test_Node.cpp.valgrind:	test_Node.cpp
	@-valgrind --error-exitcode=0 ./$< 2>&1 | \
		grep "ERROR SUMMARY" | \
		cut -d' ' -f2- > \
		$@
	@$(call timestamp,$@)

test_Tree.cpp.valgrind:	test_Tree.cpp
	@-valgrind --error-exitcode=0 ./$< 2>&1 | \
		grep "ERROR SUMMARY" | \
		cut -d' ' -f2- > \
		$@
	@$(call timestamp,$@)

Node.cpp.lint:	Node.cpp
	@-cpplint $< 2>&1 | grep -v "should include its header" > $@
	@$(call timestamp,$@)

Tree.cpp.lint:	Tree.cpp
	@-cpplint $< 2>&1 | grep -v "should include its header" > $@
	@$(call timestamp,$@)


Node.h.lint:	Node.h
	@-cpplint $< 2>&1 | grep -v "should include its header" > $@
	@$(call timestamp,$@)

Tree.h.lint:	Tree.h
	@-cpplint $< 2>&1 | grep -v "should include its header" > $@
	@$(call timestamp,$@)


test_Node.cpp.lint:	test_Node.cpp
	@-cpplint $< 2>&1 | grep -v "should include its header" > $@
	@$(call timestamp,$@)

test_Tree.cpp.lint:	test_Tree.cpp
	@-cpplint $< 2>&1 | grep -v "should include its header" > $@
	@$(call timestamp,$@)

# object files ################################################################
Node.o:	Node.cpp
	@g++ $(COPTS) -c -o $@ $<
	@$(call timestamp,$@)

Tree.o:	Tree.cpp
	@g++ $(COPTS) -c -o $@ $<
	@$(call timestamp,$@)
