# UnicodeThis library contains modules for efficient lexical operations onUnicode strings to supportsimple generation of DSL (Domain Specific Languages).```Node.cpp is an O(1) dictionary for translating Codepoints to Tablenumbers.Tree.cpp is an O(N) dictionary for translating u32strings to Canonical Lists.```## Requirements:* g++ (gnu c++ compiler) supporting the -std=c++11 flag* cpplint* valgrind* doxygen## Building:* make clean; clear; make## Node: an O(1) state transition mechanismNode acts as an O(1) dictionary to translateUnicode Codepoints or Unicode Codepoint classifications into table numbers.This enables rapid state machine operations.## Tree: an O(N) token lookup mechanismTree acts as an O(N) dictionary to identify and/or canonicalizevalid Unicode tokens.It can be used to recognize keywords.It can also be used to identify tokens that are absent from the dictionary.