COPTS=-std=c++11 -O3 -Wall -Wextra -Werror

all:	test_Node lint valgrind

test_Node:	test_Node.cpp Node.o
	@echo "\t$@"
	@g++ $(COPTS) -o $@ $^
	@-./$@ > $@.out 2>&1
	@tail -1 $@.out

.PHONY:
clean:
	@echo "\t$@"
	@rm -f *.o test_Node test_Node.out
	@rm -fr test_Node.dSym

.PHONY:
valgrind:
	@echo "\t$@"
	@-valgrind --error-exitcode=0 ./$< 2>&1 | \
		grep "ERROR SUMMARY" | \
		cut -d' ' -f2-

#"(false 'should include its header file warning')"
.PHONY:
lint:
	@echo "\tcpplint Node.h"
	@-cpplint Node.h
	@echo "\tcpplint Node.cpp (produces one spurious error)"
	@-cpplint Node.cpp 2>&1 | grep -v "Node.cpp should include its header file"
	@echo "\tcpplint test_Node.cpp"
	@-cpplint test_Node.cpp

Node.o: Node.cpp
	@g++ $(COPTS) -c -o $@ $<
