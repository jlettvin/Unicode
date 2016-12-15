#!/usr/bin/env python
# pragma pylint: disable=bad-whitespace

"""gen_Classify.py
Generates Classify.c and Classify.h from the file: DerivedGeneralCategory.txt
which is downloaded as "normative" from unicode.org for Unicode 9.0.

It will be improved with quality control (pep8, pyflakes, pylint, pychecker)
with unit tests, and with documentation at a later date.
"""

__module__     = "gen_Classify.py"
__author__     = "Jonathan D. Lettvin"
__copyright__  = "\
Copyright(C) 2016 Jonathan D. Lettvin, All Rights Reserved"
__credits__    = ["Jonathan D. Lettvin"]
__license__    = "GPLv3"
__version__    = "0.0.1"
__maintainer__ = "Jonathan D. Lettvin"
__email__      = "jlettvin@gmail.com"
__contact__    = "jlettvin@gmail.com"
__status__     = "Demonstration"
__date__       = "20161215"

from Self import (Self)


class Classify(object):
    """Generate Classify.c and Classify.h"""

    def __init__(self):
        """
        This process may be slightly inefficient, but that is for clarity.
        """

        # Setup for rapid recognition of hex digits
        digit = [chr(d) in "0123456789ABCDEF" for d in range(255)]

        # Read the entire file
        with open("Normative/DerivedGeneralCategory.txt") as source:
            lines = source.readlines()

        # This sequence refines/updates local variable fields several times.

        # Keep only lines divided by semicolons
        fields = [line.split(';') for line in lines if ';' in line]
        # Strip extra whitespace
        fields = [[f.strip() for f in field] for field in fields]
        # Keep only lines beginning with a hex digit
        fields = [field for field in fields if digit[ord(field[0][0])]]
        # Split first into range and cut extra text from second
        fields = [
            [field1.split('..'), field2[0:2]] for field1, field2 in fields]
        # Fill in ranges for solitary codepoints
        for number in range(len(fields)):
            if len(fields[number][0]) == 1:
                fields[number][0] = fields[number][0] * 2

        # Extract, sort, and enhance uniq general category labels.
        self.label = ['__'] + sorted(
            set([field2 for field1, field2 in fields]))
        # Create a reverse lookup for label index from label
        self.index = {label: n for n, label in enumerate(self.label)}

        # Create dictionaries of starts:parameters and labels:lengths.
        self.fields = {}
        self.ranges = {}
        for field1, field2 in fields:
            number1, number2 = int(field1[0], 0x10), int(field1[1], 0x10)
            index, length = self.index[field2], 1 + number2 - number1
            self.fields[number1] = {
                'start': number1,
                'length': length,
                'index': index
            }
            self.ranges[index] = self.ranges.get(index, set())
            self.ranges[index].add(length)

        # Sort codepoint range starts into correct order.
        self.starts = sorted(self.fields.keys())
        self.text = ""
        self.assoc = {}
        self.pairs = 0

    def header(self):
        """\
/* Classify.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */

/** Classify.h and Classify.c
These files contain sufficient data to reconstruct the array "Classify"
containing the index to the string in "Classify_Label" for each codepoint.
The reason for the odd construction is that classifications are
highly concentrated into RunLengths.
Classify_uniq is an array of ALL uniq pairs of (index, length)
found when constructing the Classify array.
The odd-looking "Classigy_RLE" is the index into Classify_uniq
to a given pair.
The sequence of pairs fills the entire valid codepoint range with
index-to-label.
The classification of a codepoint devolves into the simple:
    Classify[codepoint];
which returns a NUL terminated label char*.

Immediately after the declaration of Classify is an
__attribute__((constructor)) which is executed before entry into main
for gnu c compilers.
This guarantees that Classify is filled and ready at runtime
after being reconstructed from RunLength pairs.
 */


#include "Classify.h"
"""
        self.text += Self.doc()
        return self

    def labels(self):
        """
/**
 * Classify_Label contains all legal top-level Unicode codepoint classifiers
 */
const char Classify_Label[%d][3] = {
%s
};
"""
        cols, count, text, twixt = 10, len(self.label), "", ["    ", ",\n    "]
        # Formatting is carefully customized to have a pleasing arrangement
        for i in range(count):
            text += twixt[(i % cols) == 1] + '"' + self.label[i] + '"'
            twixt[0] = ", "
        self.text += Self.doc() % (count, text)
        return self

    def uniques(self):
        """
/**
 * Classify_uniq contains all unique (label_index, length) pairs.
 * This is useful because the actual pairs are quire repetitive.
 */
static const unsigned Classify_uniq[%d][2] = {
%s
};
"""
        rle = sum([len(lengths) for lengths in self.ranges.values()])
        text = "    "
        columns = 5
        for index in sorted(self.ranges.keys()):
            for length in sorted(self.ranges[index]):
                text += " {%2d, %6d}," % (index, length)
                self.assoc[index] = self.assoc.get(index, {})
                self.assoc[index][length] = self.pairs
                self.pairs = self.pairs + 1
                if (self.pairs % columns) == 0:
                    text += '\n    '
        text += " { 0,      0}"
        self.text += Self.doc() % (rle + 1, text)
        return self

    def indices(self):
        """
/**
 * Classify_RLE contains indices to pairs in the Classify_uniq table.
 * The sequence of pairs is used to fill the Classify array.
 */
static const unsigned Classify_RLE[%d] = {
%s
};
"""
        # self.starts is the ordered codepoint range starts
        # self.assoc enables label:length indexing
        text = ""
        columns = 16
        count = 0
        for start in self.starts:
            field = self.fields[start]
            index = field['index']
            length = field['length']
            text += " %3d," % (self.assoc[index][length])
            count = count + 1
            if (count % columns) == 0:
                text += '\n'
        text += "   0"
        self.text += Self.doc() % (len(self.starts) + 1, text)
        return self

    def classify(self):
        """
/**
 * Classify is the array of indices to classification labels for ALL codepoints
 */
unsigned char Classify[0x110000];
"""
        self.text += Self.doc()
        return self

    def constructor(self):
        """
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

    for (int i=0; i < 3655 ; ++i) {
        const unsigned *pair = Classify_uniq[Classify_RLE[i]];
        unsigned index = pair[0];
        unsigned width = pair[1];

        for (unsigned n=0; n < width; ++n)
            Classify[codepoint++] = (unsigned char)index;
    }
}
"""
        self.text += Self.doc()
        return self

    def __call__(self):

        self.header().labels().uniques().indices().classify().constructor()

        return self

    def __str__(self):

        return self.text


if __name__ == "__main__":
    CLASSIFY_H = """\
/* Classify.h Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#ifndef C_CLASSIFY_H_
#define C_CLASSIFY_H_
/** Classify.h and Classify.c
 *
 * \\class Omni
 *
 * \\ingroup jlettvin
 *
 * \\brief codepoint classifier (lexer generics)
 *
 * \\author Jonathan D. Lettvin
 * jlettvin@gmail.com
 *
 * \\version 0.0.1
 *
 * \\date 2016/12/10 09:47
 *
 * license GPLv3
 *
 * \\copyright Copyright(C) 2016 Jonathan D. Lettvin, All Rights Reserved"
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

    def main():
        """
This is the main entrypoint.
It instances the class and fills the two target files with contents.
        """
        classify = Classify()
        with open("Classify.c", "w") as target:
            print>>target, classify()
        with open("Classify.h", "w") as target:
            print>>target, CLASSIFY_H % (len(classify.label))

    main()
