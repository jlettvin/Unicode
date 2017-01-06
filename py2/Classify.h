/* Classify.h Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */


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


#ifndef C_CLASSIFY_H_
#define C_CLASSIFY_H_
const char Classify_Label[31][3];  ///< All valid toplevel Unicode classifiers

unsigned char Classify[0x110000];  ///< Runtime reconstructed classifier array
#endif  // C_CLASSIFY_H_

