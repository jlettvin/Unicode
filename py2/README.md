# Uni Library Overview

These modules implement useful functions
for Unicode search, math, and support.

The algorithms enable efficient use of memory and execution
to perform a variety of useful functions.

All modules have unit tests and pass PEP8 and PyFlakes quality checks.

## CORE: Two modules offer the core library value:
* UniTree performs word searches
  * Multiple variations like [hello helo hllo] can canonicalize to hello.
  * Tens of thousands of words can be uniquely identified without slowdown.
  * The complete 21 bit unicode range is supported for words.

* UniDigit performs digit canonicalization across alphabets
  * 37 different 0-9 sequences can be freely intermixed.
  * Input and output languages are recognizable and choosable.
  * utoi (similar to atoi) can be constructed using any accepted digit.

## SUPPORT: Other modules offer support to the core library:
* Array is used by UniDigit to contain the digit recognition tables.
* UniDict is used in both core modules to manage options as member names.
* UniDoc is used in both core modules to support verbose operations.

## FILES:
```
local                   Directory with a common resource file from unicode.org
.gitignore              Intermediate files to be ignored when running/testing
CPT                     Code Point Tree (replacement for segment dict)
LICENSE                 Gnu V3.0
Makefile                Run PEP8 and cleanup, recurse into test and make
README.md               This file
Array.py                Class ndarray emulation to avoid requirements
UniClass.py             Class to support fast/compact codepoint classification
UniCut.py               Class to manage sparse array indexing for codepoints
UniDict.py              Class for maintaining dict elements as class members
UniDigit.py             Class for converting digits between languages
UniDoc.py               Function for accessing function __doc__ string
UniGrammar.py           Generator for Unicode codepoint classification grammar
UniTree.py              Class for treeing, pruning, and canonicalizing words
__init__.py             Root of library
classify16.g4           Narrow Unicode classification grammar
classify21.g4           Standard Unicode classification grammar
test_Codepoint.g4       Grammar for unit test
test_Codepoint.py       Unit test top-level for Unicode classification grammar
test/Makefile           Run tests
test/test_CPT.py        Unit test
test/test_Array.py      Unit test
test/test_UniClass.py   Unit test
test/test_UniCut.py     Unit test
test/test_UniDict.py    Unit test
test/test_UniDigit.py   Unit test
test/test_UniDoc.py     Unit test
test/test_UniTree.py    Unit test
```

## MAKE:
```
 $ make clean  # remove intermediate files
 $ make        # run PEP8, pyflakes, pylint and tests on Uni*.py module files
```

## MODULES:

### CPT O(1) lookup of codepoint in sparse tree
* CPT.py:
  * __init__:
    * cpt = CPT()
  * __setitem__:
    * cpt[97] = 5773
  * __getitem__:
    * assert cpt[97] == 5773
  * __delitem__:
    * del cpt[97]
  * __lenitem__:
    * len(cpt)

The reason this code is needed is that dict uses hashes introducing overhead
in the form of hashing and bucket searching.
This code eliminates this by breaking a codepoint into 7 3 bit segments.
It then creates a table lookup mechanism that has exactly one cost.
From a codepoint, 7 dereferences yields either an associated number or 0.

### Array (numpy ndarray emulator)
* Array.py:
  * init(shape, value=0):
    * shape (just like ndarray)
    * value (initialize (default: 0))

Generate and provide get/set accessors for a simple multidimensional array.

See test_Array.py to see permitted operations.

### UniClass (efficient and compact codepoint classification)
* UniClass.py:
  * Contains a namespace, manifest constants, and a static method
  * UniClass.classify(codepoint) returns a valid Unicode classification or '__'

The reason this code works is that Unicode codepoint classification comprises
huge tracts of codepoints of identical classification making it a candidate for
Run-Length-Encoding compression, and the result is less than 5 KiB of data
in b64 format which expands up to a string of length 1114112.
Indexing this string with a codepoint ordinal yields an 8 bit ordinal which
can be used as an offset into the list of classification names.

#### Manifest constants (all classification data for 0-0x10FFFF codepoints)
```python
    label = [
        "__", "Cc", "Cf", "Cn", "Co", "Cs", "Ll", "Lm", "Lo", "Lt", "Lu",
        "Mc", "Me", "Mn", "Nd", "Nl", "No", "Pc", "Pd", "Pe", "Pf", "Pi",
        "Po", "Ps", "Sc", "Sk", "Sm", "So", "Zl", "Zp", "Zs",
    ]
    index = bz2.decompress("""
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
Jt4TdgiKWlR/8XckU4UJCdVo7kA=
""".decode('base64'))
```

#### Code to extract classification from manifest constants
```python
    return UniClass.label[ord(UniClass.index[codepoint])]
```

See test_UniClass.py to see permitted operations.

### UniCut (sparse array manager for codepoint fragments)
* UniCut.py:
  * init(**kw)      initializes bits and masks
  * cut(codepoint)  divides codepoint according to bits and masks
  * TODO            make tables in UniDigit intrinsic to this class

### UniDict (enhanced dictionary)
* UniDict.py:
  * init(**kw)  initializes
  * call(**kw)  updates

Ingest arbitraty dictionaries (i.e. docopt return value) and
strip out non-alpha characters for keys then
make those keys available as object member names.

See test_UniDict.py to see permitted operations.

### UniDigit (digit converter: language-independent and canonicalizing)
* UniDigit.py:
  * call(codepoint)  returns the associated int digit or -1 if not Nd

An efficient multi-language number conversion algorithm
UniDigit.py is a library class for building and using a fast lookup tree.
It is a demonstration of canonicalizing digits from all supported languages.

See test_UniDigit.py to see permitted operations.

### UniDoc (functions and methods can verbosely report their __doc__ string)
* UniDoc.py:
  * UniDoc()  returns __doc__ of caller

### UniGrammar (antlr4 grammar generator for Unicode codepoint classification)
* UniGrammar.py
  * classify16.g4  a grammar for 16 bit unicode is produced
  * classify21.g4  a grammar for 21 bit unicode is produced

This code is a means for production, therefore its __main__ is to be executed.

### UniTree (tree builder: fast canonicalizing word tree splice/prune/search)
* UniTree.py:
  * init(wordlist)              Insert canonical words into tree
  * tree[word]                  Returns True if word is in tree
  * tree.word(word, variant)    Inserts variant and canonicalize to word
  * tree(word, 'delete')        Deletes word from tree
  * tree.delete(word)           Deletes word from tree

An efficient multi-word canonicalizing search algorithm
UniTree.py is a library class for building and using a fast lookup tree.
It is a demonstration of a technique for rapid tests for presence of
a candidate word in a precompiled set.

UniTree converts a list of words into a fast lookup tree on Unicode codepoints
and provides a function for using that tree to lookup words.

Words are added by an functor which allows dynamic updates,
so that words can be added to the tree while it is in use.
Words can be deleted and variants on words may lead to a canonical word.
In threaded operation (other languages) this operation will need mutexes.

See test_UniTree.py to see permitted operations.

# Details

## UniTree: Consider the words:
```python
[hello, hi, hola, hold, hole, work, world]
```

### UniTree.py builds this dictionary tree structure (except with codepoints):
```python
tree = {
    u'h': {u'e': {u'l': {u'l': {u'o': {'$': set([u'hello'])}}}},
           u'i': {'$': set([u'hi'])},
           u'o': {u'l': {u'a': {'$': set([u'hola'])},
                         u'd': {'$': set([u'hold'])},
                         u'e': {'$': set([u'hole'])}}}},
    u'w': {u'o': {u'r': {u'k': {'$': set([u'work'])},
                         u'l': {u'd': {'$': set([u'world'])}}}}}
}
```

### This simplified Python 2.7 code identifies treed words quickly:
```python
def find(word, tree):
    temp = tree
    for c in list(word):
        temp = temp.get(c, None)
        if not temp:
            break
    return (temp and {} == temp.get('$', None)) or False
```

### Performance
It should be obvious that this is not highly efficient written in Python.
This code, rewritten in C or C++ should be wicked fast.
Maybe java could compete.
Javascript JIT compiler may also do a good job.
But the structure can be turned into indexing rather than hashing
for performance close to one or only a few Intel instructions per letter.
Also, using unicode codepoints rather than letters is likely preferred.

## UniDigit: encapsulate latest Unicode Nd digit translations

### UniDigit.py builds this table for ASCII only
```python
table = [[0, 0, 0, 0, 0, 0, 0, 0],
 [1, 1, 1, 1, 1, 1, 1, 1],
 [2, 2, 2, 2, 2, 2, 2, 2],
 [3, 3, 3, 3, 3, 3, 3, 3],
 [4, 4, 4, 4, 4, 4, 4, 4],
 [5, 5, 5, 5, 5, 5, 5, 5],
 [6, 6, 6, 6, 6, 6, 6, 6],
 [7, 7, 7, 7, 7, 7, 7, 7],
 [8, 8, 8, 8, 8, 8, 8, 8],
 [9, 9, 9, 9, 9, 9, 9, 9],
 [11, -1, -1, -1, -1, -1, -1, -1],
 [12, -1, -1, -1, -1, -1, -1, -1],
 [13, -1, -1, -1, -1, -1, -1, -1],
 [14, -1, -1, -1, -1, -1, -1, -1],
 [15, -1, -1, -1, -1, -1, -1, -1],
 [-1, -1, -1, -1, -1, -1, 16, 17],
 [0, 1, 2, 3, 4, 5, 6, 7],
 [8, 9, -1, -1, -1, -1, -1, -1]]
```

### UniDigit.py uses this code to access the table
Given a codepoint, this code traverses the table and produces
a valid digit between 0:9 if the codepoint is valid
or -1 if the codepoint is invalid.
The tree is much larger when populated with digits from all languages.
For the particular value of bits of 3, this produces a consistent
7 dereferences with no branches to convert a codepoint to a digit
regardless of how many language digits are employed.
```python
def toDigit(c):  # c is a codepoint ordinal
    codepointbits = 21
    base = 10
    bits = 3
    enum = 1 << bits
    mask = enum - 1
    need = codepointbits / bits
    need += int(codepointbits % bits)
    cuts = [(c>>(shft*self.bits)) & self.mask for shft in range(self.need)]
    cuts.reverse()
    for segment in cuts:
        this = self[this][segment]
        if this < base:
            return this
    return digit
```

### UniGrammar.py uses UniClass as the majority source for data
It also uses PropertyValueAliases.txt and Blocks.txt from unicode.org.

# Future

#### Parsing
The functor is targeted for modification such that
each keyword is associated with a trigger function
which is installed in the tree structure at word terminals.
For walking the parse tree after it is built,
execution of those functions is an alternative to trigger execution.
