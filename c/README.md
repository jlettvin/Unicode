# Unicode dynamic lib for scripting language support of high speed lexing
This C language optimized library offers
a set of functions designed to enable
development of DSL lexing/parsing
at very high performance while being
as high quality as is reasonable without
obfuscating what is done and
why the implementation works.

The principle insight is that
GOTO and table-driven transforms
outperform architectures which
test for values and take alternate code paths.
GOTO and table-driven are considered unusual
for modern development.

The code is designed to be very short and tight.
It makes use of the gnu compiler stack where
computed GOTO has been a standard extension for many years,
where other compilers have no support for this efficiency.

A benefit of the use of GOTO in this library
is that the likely code path has been
prefetched into the processor cache
(so there is no flushing of the cache for
either of the two directions the GOTO may take).

The general approach to table-driven paths
is explained in the main README.md
which is in the parent directory to this library source.

This directory will be the primary area
for development of a shared Python library
because CFFI is less friendly to C++
than it is to C function entrypoints.

## TODO
* Improve unit testing
* Clarify typedefs in Unicode.h
* Find a worthy gprof for Mac OS X

### TODO valgrind
* Document memory leaks due to C standard library calls
* Eliminate memory leaks (if any) due to code in this library

### TODO Unicode.c
* Improve Tests of UTF8 <-> char32 transforms in test_Unicode.c

### TODO gcov
* Discover how to make Makefile generic rule for gcov work
* Improve coverage
