# Unicode
This library contains modules for efficient lexical operations on
Unicode strings to support
simple generation of DSL (Domain Specific Languages).
A dynamic library is generated.
Unit tests exercise most of the functionality.
Mostly, the DRY principle has been followed.
More work is needed in these directions:
* More unit tests (including failure modes)
* TDD Implement Tree functionality.
* More language support (c, c++, py2, py3, js, cuda)

The goal is to enable generation of DSL lexers
from and to graphviz text files
so grammars can be inspected visually
and the code resulting from the grammar
is already optimized into table lookup
and computed goto to support big data ingest.

```
.gitignore          patterns of intermediate files not to be stored on github
LICENSE             (GPLv3)
README.md           this file
c                   C implementation (including python support via CFFI)
c++                 TODO
js                  TODO
py2                 Python 2.7 demonstration of ideas
```

## Requirements:
* g++ (gnu c++ compiler) supporting the -std=c++11 flag
* cpplint
* valgrind
* gcov
* gprof
* doxygen
* pep8
* pyflakes
* pylint
* pytest

## Build:
TODO

## Theory:
Dictionary lookup typically incurs a cost of
calculating the hash followed by disambiguating the hash bucket.
Both of these calculations can be expensive.
The cost is fixed for hash tables with no collisions.

Node and Tree are alternative algorithms for guaranteeing
low fixed cost dictionary operations on
simple Unicode codepoints (Node) and
simple Unicode codepoint strings (Tree).

### Observations supporting the Node implementation:
Codepoints have 21 significant bits. 
Observe letter B converted to indices.
```
B
0042                          hexadecimal
000000000000001000010         binary
000 000 000 000 001 000 010   7 groups of 3
a=0 b=0 c=0 d=0 e=1 f=0 g=2   position labels and indices
```

Lookup is performed by multiple table dereferences to get new indices.
The code for getting an association for a codepoint is:
```
((codepoint>>(n*3))&0xf) where a uses n==0, b uses n==1, etc...
lookup(codepoint) { return T[T[T[T[T[T[T[1][g]][f]][e]][d]][c]][b]][a] }
```
* 7 multiplies      # Can be eliminated by manifest constants
* 7 shifts
* 7 masks
* 8 table dereferences
* 29 total machine code operations in original iAPX86 8088 instructions
* operation count may be less for more recent processors
* memory consumption per layer is 32 bytes

Consider all the values to be hexadecimal.
The initial table index is 1 because index 0 is used to flag errors.
Suppose we want to associate the codepoint for letter 'B' with its codepoint.
In other words, this lookup will be the identity function lookup(42) = 42.
We will restrict the hex representation to 16 bits for visual ease.
Keep in mind that the full width is 21.
We start by building a table T with the following contents:
```
B
0042
000000000000001000011
000 000 000 000 001 000 010
a=0 b=0 c=0 d=0 e=1 f=0 g=2
T = [
 [0000 0000 0000 0000 0000 0000 0000 0000]  # <- error layer (all zeros always)
 [0002 0000 0000 0000 0000 0000 0000 0000]  # <- starting index for lookup
 [0003 0000 0000 0000 0000 0000 0000 0000]
 [0004 0000 0000 0000 0000 0000 0000 0000]
 [0005 0000 0000 0000 0000 0000 0000 0000]
 [0000 0006 0000 0000 0000 0000 0000 0000]
 [0007 0000 0000 0000 0000 0000 0000 0000]
 [0000 0000 0042 0000 0000 0000 0000 0000]  # <- layer containing association
]
```

Illustration of sequential dereferencing.
```
 T[1][a] yields 2 then
 T[2][b] yields 3 then
 T[3][c] yields 4 then
 T[4][d] yields 5 then
 T[5][e] yields 6 then
 T[6][f] yields 7 then
 T[7][g] yields 42  # The association
```

If we want to add an association for 'C' the table will look like this:
```
C
0043
000000000000001000011
000 000 000 000 001 000 011
a=0 b=0 c=0 d=0 e=1 f=0 g=3
T = [
 [0000 0000 0000 0000 0000 0000 0000 0000]
 [0002 0000 0000 0000 0000 0000 0000 0000]
 [0003 0000 0000 0000 0000 0000 0000 0000]
 [0004 0000 0000 0000 0000 0000 0000 0000]
 [0005 0000 0000 0000 0000 0000 0000 0000]
 [0000 0006 0000 0000 0000 0000 0000 0000]
 [0007 0000 0000 0000 0000 0000 0000 0000]
 [0000 0000 0042 0043 0000 0000 0000 0000] # <- table 8 has 0043 at offset 3
]
```
Note how no layers were added to support 'C'.

If we want to add an association for 'L' the table will look like this.
```
L
004C
000000000000001001100
000 000 000 000 001 001 100
a=0 b=0 c=0 d=0 e=1 f=1 g=4
T = [
 [0000 0000 0000 0000 0000 0000 0000 0000]
 [0002 0000 0000 0000 0000 0000 0000 0000]
 [0003 0000 0000 0000 0000 0000 0000 0000]
 [0004 0000 0000 0000 0000 0000 0000 0000]
 [0005 0000 0000 0000 0000 0000 0000 0000]
 [0000 0006 0000 0000 0000 0000 0000 0000]
 [0007 0008 0000 0000 0000 0000 0000 0000] # <- see the addition of table 8
 [0000 0000 0042 0043 0000 0000 0000 0000]
 [0000 0000 0000 0000 004C 0000 0000 0000] # <- table 8 has 004C at offset 4
]
```

Now, let's add the Chinese character for mountain U+5C71
```
山
5C71
000000101110001110001 
000 000 101 110 001 110 001
a=0 b=0 c=5 d=6 e=1 f=6 g=1
T = [
 [0000 0000 0000 0000 0000 0000 0000 0000]
 [0002 0000 0000 0000 0000 0000 0000 0000]
 [0003 0000 0000 0000 0000 0000 0000 0000]
 [0004 0000 0000 0000 0000 0009 0000 0000] # <- see the addition of table 9
 [0005 0000 0000 0000 0000 0000 0000 0000]
 [0000 0006 0000 0000 0000 0000 0000 0000]
 [0007 0008 0000 0000 0000 0000 0000 0000]
 [0000 0000 0042 0043 0000 0000 0000 0000]
 [0000 0000 0000 0000 004C 0000 0000 0000]
 [0000 0000 0000 0000 0000 0000 000A 0000] # <- see the addition of table A
 [0000 000B 0000 0000 0000 0000 0000 0000] # <- see the addition of table B
 [0000 0000 0000 0000 0000 0000 000C 0000] # <- see the addition of table C
 [0000 000D 0000 0000 0000 0000 0000 0000] # <- see the addition of table D
 [0000 5C71 0000 0000 0000 0000 0000 0000] # <- table D has 5C71 at offset 1
]
```

Illustration of sequential dereferencing for that Chinese character.
```
 T[1][a] yields 2 then
 T[2][b] yields 3 then
 T[3][c] yields 9 then
 T[4][d] yields A then
 T[5][e] yields B then
 T[6][f] yields C then
 T[7][g] yields 5C71  # The association
```

Using 3 groups of 7, the operation count drops to 17 and
the memory consumption per layer is 512 bytes.

7 groups of 3 may look a bit inefficient with space,
but 3 groups of 7 would be worse.
The total consumed here by 7 groups of 3 is 14 * 32 = 448 butes.
For 3 groups of 7 this would be 6 * 512 = 3017 bytes.
7 groups of 3 increases the calculation time by 7/3 or 2.3333...
We choose to trade speed for space.
These representations can be stored and recovered from disk.

Summary:
* Codepoints are intrinsically 21 bits.
* 21 is the product 3 * 7.
* 3 groups of 7 bits or 7 groups of 3 bits will fit a codepoint perfectly.
* a 32 bit internal codepoint or index occupies 4 bytes.
* 3 bits supports 8 indices while 7 bits supports 128 indices.
* 7 groups of 3 is used because the table layer size is small (32 bytes).
* The initial association costs 8 * 32 = 256 bytes.
* Subsequent associations in-layer cost 0 additional bytes.
* Subsequent associations out-of-layer cost at least one final layer each.
* Worst case cost (i.e. 0x10FFF) costs 6 additional layers.

### Special Notes:
0x10FFFF is used as a sentinel in the code as the highest legal codepoint.
This is permitted for two reasons, it enables detection of some errors,
and it is documented to never be transmitted and is declared a nonchar.

These implementations reduce/eliminate function calls and branch points.
Data flows through a table or computed gotos for translations both ways.

## Node: an O(1) state transition mechanism

Node acts as an O(1) dictionary to translate
Unicode Codepoints or Unicode Codepoint classifications into table numbers.
This enables rapid state machine operations.

## Tree: an O(N) token lookup mechanism

Tree acts as an O(N) dictionary to identify and/or canonicalize
valid Unicode tokens.
It can be used to recognize keywords.
It can also be used to identify tokens that are absent from the dictionary.
