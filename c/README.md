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

valgrind does not identify any memory leaks
resulting from the code in this library
There are memory leaks in the test modules
due to implementation errors in the standard library.

## Points of Interest

### UTF8 <-> char32_t
Python libraries for handling UTF8 and full-width Unicode codepoints
are fraught with pitfalls for the everyday developer.
This library offers a simple pair of symmetric operations which convert
between UTF8 encoded strings (the default most popular Unicode format)
and string32 (the necessary ordinal size for internal Unicode).

Were this implementation not available, here are issues to consider:
* Recompile Python to handle 21 bit codepoints rather than only 16 bit
* Import libraries for which the encode/decode interface is richer
* Run additional tests to guarantee that the imports perform fully
* Additional concerns not listed here...

```
    # Here is all that will be needed to use these library functions:
    
    with open("UTF8encoded_source.txt", "rb") as source:
        asUTF8 = source.read()
        UTF8_to_u32string(asUTF8, asU32String)

    # asU32String is an array of 32 bit codepoints.
    
    with open("UTF8encoded_target.txt", "wb") as target:
        u32string_to_UTF8(asU32String, asUTF8)
        target.write(asUTF8)
```

## TODO
* Improve unit testing
* Clarify typedefs in Unicode.h
* Find a worthy gprof for Mac OS X

### TODO Unicode.c
* Improve Tests of UTF8 <-> char32 transforms in test_Unicode.c

### TODO gcov
* Discover how to make Makefile generic rule for gcov work
* Improve coverage
