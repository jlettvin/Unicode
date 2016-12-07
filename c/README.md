# Unicode (C) for dynamic lib Python support of high speed lexing
This library offers a set of functions
designed to be very high performance
while being as high quality as reasonable.

The principle approach uses two methods
considered unusual for modern development.
* Multiple dereferencing table-driven paths
* Computed GOTO to eliminate if/for/while

The code is designed to be very short and tight.
It makes use of the gnu compiler stack where
computed GOTO has been a standard extension
for many years, where other compilers have not.

The general approach to table-driven paths
is explained in the main README.md
which is in the parent directory to this.

This directory will be the primary area
for development of a shared Python library
because CFFI is less friendly to C++
than it is to C function entrypoints.
