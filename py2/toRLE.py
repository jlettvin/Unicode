#!/usr/bin/env python
# -*- coding: utf8 -*-

"""
toRLE.py produces Classify.h and Classify.c source files for
github.com/jlettvin/Unicode/c/ directory.
"""

C_comment = """
/** Classify.h and Classify.c
These files contain sufficient data to reconstruct the array "Classify"
containing the index to the string in "Classify_Label" for each codepoint.
The reason for the odd construction is that classifications are
highly concentrated into RunLengths.
The provate array Classify_uniq contains ALL the uniq pairs of
(index, length) found when constructing the Classify array.
The odd-looking "Classigy_RLE" is the index into Classify_uniq
to a given pair.
The sequence of pairs fills the entire valid codepoint range with
index-to-label.
The classification of a codepoint devolves into the simple:
    Classify[codepoint];
which returns a NUL terminated label char*.

Immediately after the declaration of Classify is an __attribut__((constructor))
which is executed before entry into main for gnu c compilers.
This guarantees that Classify is filled and ready at runtime
after being reconstructed from RunLength pairs.
 */
"""

from pprint import pprint

classed = []
index = {}
classify = {}
backward = {}
current = ""
with open("classify21.g4") as source:
    i = 0
    for line in source:
        if line[6:14] == "CLASSIFY":
            label = line[15:17]
            classed.append(label)
            classify[label] = []
            index[label] = i
            i += 1
            print label, index[label]
        elif line.startswith("CLASSIFY_"):
            if line[10] == ' ':
                break
            current = line[9:11]
        elif line[7:9] == r"\u":
            A = int(line[9:15], 0x10)
            twixt = line[19:21]
            if twixt == r"\u":
                B = int(line[21:27], 0x10)
                classify[current].append((A, B))
                backward[A] = (current, B-A)
            else:
                classify[current].append((A, A))
                backward[A] = (current, 1)

with open("Classify.h", "w+") as header:
    boilerplate = """\
/* Classify.h Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */

%s

#ifndef C_CLASSIFY_H_
#define C_CLASSIFY_H_
const char Classify_Label[%d][3];  ///< All valid toplevel Unicode classifiers

unsigned char Classify[0x110000];  ///< Runtime reconstructed classifier array
#endif  // C_CLASSIFY_H_
"""
    print >> header, boilerplate % (C_comment, len(index))

uniq = set()
for codepoint in sorted(backward.keys()):
    N, I = backward[codepoint]
    uniq.add("    {%2d, %5d}," % (index[N], I))
uniq = sorted(list(uniq))
find = {s:i for i, s in enumerate(uniq)}
pair = ""
for n, elem in enumerate(uniq):
    pair += elem
    if (n%4) == 3:
        pair += '\n'

mono = [
    "%3d" % (find["    {%2d, %5d}," % (index[N], I)])
    for (N,I) in [
            backward[codepoint]
            for codepoint in
            sorted(backward.keys())]
    ]
build = ""
sep = [' ', '\n']
for n, element in enumerate(mono):
    build += element + "," + sep[int(n%16)==15]


with open("Classify.c", "w+") as target:
    boilerplate = """\
/* Classify.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
%s

#include "Classify.h"

/**
 * Classify_Label contains all legal top-level Unicode codepoint classifiers
 */
const char Classify_Label[%d][3] = {
    %s
};

/**
 * Classify_uniq contains all unique (label_index, length) pairs.
 * This is useful because the actual pairs are quire repetitive.
 */
static const unsigned Classify_uniq[%d][2] = {
%s    { 0,     0}
};

/**
 * Classify_RLE contains indices to pairs in the Classify_uniq table.
 * The sequence of pairs is used to fill the Classify array.
 */
static const unsigned Classify_RLE[%d] = {\n%s  0\n};

/**
 * Classify is the array of indices to classification labels for ALL codepoints
 */
unsigned char Classify[0x110000];

/**
 * Classify_init is run before main is called.
 * It initializes the Classify array from the RLE data.
 */
__attribute__((constructor))
void Classify_init(void) {
    unsigned codepoint = 0;

    for (unsigned i=0; i < %d ; ++i) {
        const unsigned *pair = Classify_uniq[Classify_RLE[i]];
        unsigned index = pair[0];
        unsigned width = pair[1];

        for (unsigned n=0; n < width; ++n)
            Classify[codepoint++] = (unsigned char)index;
    }
}
"""
    print >> target, boilerplate % (
            C_comment,

            len(index),
            ',\n    '.join(['"%s"' % name for name in classed]),

            len(uniq)+1,
            pair,
            #''.join([
            #   "%s" % (line) for line in uniq]),

            len(backward.keys())+1,
            build,

            len(backward.keys())+1
            )
