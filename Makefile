#!/usr/bin/env make

# make clean; clear; make

# TODO use strip on library executables.


# setup #######################################################################
MODULE=Unicode
VERSION=0.0.1

timestamp=echo `date '+%Y/%m/%d %H:%M:%S'` $(1)

COPTS=-std=c++11 -O3 -Wall -Wextra -Werror -fPIC -g
CURDIR=.

MACDYLIB=libjlettvin.dylib
MACDYLIBOPT=

LINUXLIB=libjlettvin.so.$(VERSION)
LINUXLIBOPT=-L. -ljlettvin

LIBRARY=$(MACDYLIB)
LIBOPT=$(MACDYLIBOPT)

# generic rules ###############################################################

%.o	:	%.cpp
	@g++ $(COPTS) -c -o $@ $<
	@$(call timestamp,$@)

%	:	%.cpp $(LIBRARY)
	@g++ $(COPTS) -o $@ $^ $(LIBOPT)
	@$(call timestamp,$@)

%.out	:	%
	@-./$< > $@ 2>&1
	@tail -1 $@
	@$(call timestamp,$@)

%.cpp.lint	:	%.cpp
	@-cpplint $< 2>&1 | grep -v "should include its header" > $@
	@$(call timestamp,$@)

%.h.lint	:	%.h
	@-cpplint $< 2>&1 | grep -v "should include its header" > $@
	@$(call timestamp,$@)

%.cpp.valgrind	:	%
	@-valgrind --error-exitcode=0 ./$< 2>&1 | \
		grep "ERROR SUMMARY" | \
		cut -d' ' -f2- > \
		$@
	@$(call timestamp,$@)

# all #########################################################################
.PHONY:
all:	begin $(LIBRARY) out lint valgrind doxygen end

# clean #######################################################################
.PHONY:
clean:
	@rm -f *.o *.out *.lint *.valgrind
	@rm -f test_Node test_Tree
	@rm -f $(LIBRARY)
	@rm -fr test_*.dSYM
	@$(call timestamp,$@)

# documentation ###############################################################
.PHONY:
begin:
	@echo 'vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv make'
	@$(call timestamp,$@)

.PHONY:
end:
	@$(call timestamp,$@)
	@echo '^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ make'

.PHONY:
doxygen:
	@doxygen > doxygen.out 2>&1
	@$(call timestamp,$@)

# unit tests ##################################################################

out:	test_Node.out test_Tree.out

test_Node.out:	test_Node

test_Tree.out:	test_Tree

test_Node:	test_Node.cpp $(LIBRARY)

test_Tree:	test_Tree.cpp $(LIBRARY)

# valgrind ####################################################################

.PHONY:
valgrind:	\
	test_Node.cpp.valgrind \
	test_Tree.cpp.valgrind
	@$(call timestamp,$@)

test_Node.cpp.valgrind:	test_Node

test_Tree.cpp.valgrind:	test_Tree

# lint ########################################################################

.PHONY:
lint:	\
	Node.h.lint \
	Tree.h.lint \
	Node.cpp.lint \
	Tree.cpp.lint \
	test_Node.cpp.lint \
	test_Tree.cpp.lint
	@$(call timestamp,$@)

Node.cpp.lint:	Node.cpp

Tree.cpp.lint:	Tree.cpp

Node.h.lint:	Node.h

Tree.h.lint:	Tree.h

test_Node.cpp.lint:	test_Node.cpp

test_Tree.cpp.lint:	test_Tree.cpp

# shared library ##############################################################

$(MACDYLIB):	Node.o Tree.o
	@g++ \
		-dynamiclib -undefined suppress -flat_namespace \
		-install_name '$(CURDIR)/$(MACDYLIB)' -current_version $(VERSION) \
		$^ \
		-o $@
	@$(call timestamp,$@)

$(LINUXLIB):	Node.o Tree.o
	@g++ -shared -Wl,-soname,libjlettvin.so.1 -lc $^ -o $@
	@$(call timestamp,$@)

# object file #################################################################

Node.o:	Node.cpp Node.h Unicode.h

Tree.o:	Tree.cpp Tree.h Node.h Unicode.h
