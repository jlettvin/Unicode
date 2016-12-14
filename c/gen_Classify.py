#!/usr/bin/env python

"""gen_Classify.py
Generates Classify.c and Classify.h from the file: DerivedGeneralCategory.txt
which is downloaded as "normative" from unicode.org for Unicode 9.0.

Currently, the code is very "hackish" as a first cut.
It will be improved with quality control (pep8, pyflakes, pylint, pychecker)
with unit tests, and with documentation at a later date.

TODO Apply quality controls
"""

if __name__ == "__main__":
    CLASSIFY_H = """\
/* Classify.h Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#ifndef C_CLASSIFY_H_
#define C_CLASSIFY_H_
/** Classify.h and Classify.c
 *
 * \class Omni
 *
 * \ingroup jlettvin
 *
 * \brief codepoint classifier (lexer generics)
 *
 * \author Jonathan D. Lettvin
 * jlettvin@gmail.com
 *
 * \version 0.0.1
 *
 * \date 2016/12/10 09:47
 *
 * license GPLv3
 *
 * \copyright Copyright(C) 2016 Jonathan D. Lettvin, All Rights Reserved"
 */

/**
 * Classify[codepoint] is an index into the Classify_Label array.
 * Classify_Label[Classify[codepoint]] is a label as a C string.
 *
 * These files contain sufficient data to reconstruct the array "Classify"
 * containing the index to the string in "Classify_Label" for each codepoint.
 * The reason for the odd construction is that classifications are
 * highly concentrated into RunLengths.
 * The provate array Classify_uniq contains ALL the uniq pairs of
 * (index, length) found when constructing the Classify array.
 * The odd-looking "Classigy_RLE" is the index into Classify_uniq
 * to a given pair.
 * The sequence of pairs fills the entire valid codepoint range with
 * index-to-label.
 * The classification of a codepoint devolves into the simple:
 *    Classify[codepoint];
 * which returns a NUL terminated label char*.
 * Immediately after the declaration of Classify is an
 * __attribute__((constructor))
 * which is executed before entry into main for gnu c compilers.
 * This guarantees that Classify is filled and ready at runtime
 * after being reconstructed from RunLength pairs.
 */

const char Classify_Label[%d][3];  ///< All valid toplevel Unicode classifiers
unsigned char Classify[0x110000];  ///< Runtime reconstructed classifier array
#endif  // C_CLASSIFY_H_\
    """

    CLASSIFY_C_1 = """\
/* Classify.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */

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


#include "Classify.h"

/**
 * Classify_Label contains all legal top-level Unicode codepoint classifiers
 */
const char Classify_Label[%d][3] = {\
"""

    CLASSIFY_C_2 = """\
};

/**
 * Classify_uniq contains all unique (label_index, length) pairs.
 * This is useful because the actual pairs are quire repetitive.
 */
static const unsigned Classify_uniq[%d][2] = {\
"""

    CLASSIFY_C_3 = """\
};

/**
 * Classify_RLE contains indices to pairs in the Classify_uniq table.
 * The sequence of pairs is used to fill the Classify array.
 */
static const unsigned Classify_RLE[%d] = {\
"""

    CLASSIFY_C_4 = """\
};

/**
 * Classify is the array of indices to classification labels for ALL codepoints
 */
unsigned char Classify[0x110000];

/**
 * Classify_init is run before main is called.
 * It initializes the Classify array from the RLE data.
 * https://www.hackerearth.com/practice/notes/
 *      functions-which-get-executed-before-and-after-main-in-c/
 * http://www.geeksforgeeks.org/
 *      functions-that-are-executed-before-and-after-main-in-c/
 */
__attribute__((constructor))
void Classify_init(void) {
    unsigned codepoint = 0;

    for (int i=0; i < %d ; ++i) {
        const unsigned *pair = Classify_uniq[Classify_RLE[i]];
        unsigned index = pair[0];
        unsigned width = pair[1];

        for (unsigned n=0; n < width; ++n)
            Classify[codepoint++] = (unsigned char)index;
    }
}\
"""

    def main():
        total = 0
        ranges = []
        labels = set()
        start = {}
        count = {}
        digit = [False] * 256
        for c in "0123456789ABCDEF":
            digit[int(c, 0x10)] = True
        with open("Normative/DerivedGeneralCategory.txt") as source:
            for line in source:
                semi = [field.strip() for field in line.split(';')]
                if len(semi) >= 2 and digit[int(semi[0][0], 0x10)]:
                    label = semi[1][0:2]
                    labels.add(label)
                    dots = [int(field,0x10) for field in semi[0].split('..')]
                    if len(dots) == 2:
                        total = total + 1 + dots[1] - dots[0]
                        ranges.append([label, 1 + dots[1] - dots[0], dots[0]])
                        start[dots[0]] = {
                                'start': dots[0],
                                'end': dots[1],
                                'label': label,
                                'len': 1 + dots[1] - dots[0]
                                }
                    else:
                        total = total + 1
                        ranges.append([label, 1 + dots[0] - dots[0], dots[0]])
                        start[dots[0]] = {
                                'start': dots[0],
                                'end': dots[0],
                                'label': label,
                                'len': 1
                                }
        starts = sorted(start.keys())
        labels = ['__'] + sorted(labels)
        index = {k:n for n, k in enumerate(labels)}

        ranges = [[index[pair[0]], pair[1], pair[2]] for pair in ranges]

        # Generate the uniq range lengths for each label.
        for triple in ranges:
            count[triple[0]] = count.get(triple[0], set())
            count[triple[0]].add(triple[1])
        uniqs = sum([len(count[n]) for n in count.keys()])

        with open("Classify.c", "w") as target:
            twixt = ["    ", ",\n    "]
    
            print>>target, CLASSIFY_C_1 % (len(labels))
    
            for i in range(len(labels)):
                print>>target, twixt[1 == (i % 6)] + '"' + labels[i] + '"',
                twixt[0] = ", "
            print>>target
    
            print>>target, CLASSIFY_C_2 % (uniqs + 1),
            find = {}
            fmt = " {%2d, %6d},"
            on0 = 0
            j = 0
            for i, k in enumerate(sorted(count.keys())):
                for n in sorted(count[k]):
                    if on0 == 0:
                        print>>target
                        print>>target, "   ",
                    on0 = (on0 + 1) & 3
                    newkey = fmt % (k, n)
                    find[newkey] = j
                    j += 1
                    print>>target, newkey,
            print>>target
            print>>target, "     { 0,     0}"
    
    
            print>>target, CLASSIFY_C_3 % (len(starts) + 1),
    
            on0 = 0
            for item in starts:  # list of sorted start codepoints
                # want the label index and the range length
                this = start[item]
                newkey = fmt % (index[this['label']], this['len'])
                rng = find[newkey]
                if on0 == 0:
                    print>>target
                on0 = (on0 + 1) & 0xf
                print>>target, "%3d," % rng,
            print>>target
            print>>target, "   0"
    
            print>>target, CLASSIFY_C_4 % (len(starts) + 1)
    
            if total != 0x110000:
                print>>target, "MISSING CLASSIFIERS"

        with open("Classify.h", "w") as target:
            print>>target, CLASSIFY_H % (len(labels))


    main()
