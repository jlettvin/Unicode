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
const char Classify_Label[31][3] = {
    "__",
    "Cc",
    "Cf",
    "Cn",
    "Co",
    "Cs",
    "Ll",
    "Lm",
    "Lo",
    "Lt",
    "Lu",
    "Mc",
    "Me",
    "Mn",
    "Nd",
    "Nl",
    "No",
    "Pc",
    "Pd",
    "Pe",
    "Pf",
    "Pi",
    "Po",
    "Ps",
    "Sc",
    "Sk",
    "Sm",
    "So",
    "Zl",
    "Zp",
    "Zs"
};

/**
 * Classify_uniq contains all unique (label_index, length) pairs.
 * This is useful because the actual pairs are quire repetitive.
 */
static const unsigned Classify_uniq[459][2] = {
    { 0,     1},    { 0,     2},    { 0,     3},    { 0,     4},
    { 0,     5},    { 0,     6},    { 0,     7},    { 0,     8},
    { 0,     9},    { 0,    10},    { 0,    11},    { 0,    12},
    { 0,    13},    { 0,    14},    { 0,    15},    { 0,    16},
    { 0,    17},    { 0,    19},    { 0,    20},    { 0,    22},
    { 0,    23},    { 0,    24},    { 0,    26},    { 0,    31},
    { 0,    32},    { 0,    33},    { 0,    34},    { 0,    37},
    { 0,    38},    { 0,    41},    { 0,    42},    { 0,    43},
    { 0,    46},    { 0,    47},    { 0,    48},    { 0,    52},
    { 0,    54},    { 0,    55},    { 0,    57},    { 0,    63},
    { 0,    64},    { 0,    65},    { 0,    80},    { 0,    96},
    { 0,    98},    { 0,   102},    { 0,   126},    { 0,   130},
    { 0,   139},    { 0,   142},    { 0,   144},    { 0,   152},
    { 0,   166},    { 0,   174},    { 0,   186},    { 0,   263},
    { 0,   270},    { 0,   352},    { 0,   385},    { 0,   448},
    { 0,   841},    { 0,   880},    { 0,   894},    { 0,  1022},
    { 0,  1184},    { 0,  1360},    { 0,  1504},    { 0,  1597},
    { 0,  2005},    { 0,  2748},    { 0,  3070},    { 0,  4049},
    { 0,  4956},    { 0,  6398},    { 0,  8633},    { 0,  9485},
    { 0, 10590},    { 0, 11170},    { 0, 61438},    { 0, 65532},
    { 0, 65534},    { 1,    32},    { 1,    33},    { 2,     1},
    { 2,     3},    { 2,     4},    { 2,     5},    { 2,     6},
    { 2,     8},    { 2,    10},    { 2,    96},    { 3,     1},
    { 3,     2},    { 3,     3},    { 3,     9},    { 3,    16},
    { 3,    21},    { 3,    25},    { 3,    26},    { 3,    30},
    { 3,    32},    { 3,    59},    { 3,   128},    { 3,  3600},
    { 4,     1},    { 5,     1},    { 5,     2},    { 6,     1},
    { 6,     2},    { 6,     3},    { 6,     4},    { 6,     5},
    { 6,     6},    { 6,     7},    { 6,     8},    { 6,     9},
    { 6,    11},    { 6,    13},    { 6,    14},    { 6,    18},
    { 6,    24},    { 6,    25},    { 6,    26},    { 6,    27},
    { 6,    28},    { 6,    32},    { 6,    34},    { 6,    35},
    { 6,    36},    { 6,    38},    { 6,    39},    { 6,    40},
    { 6,    43},    { 6,    44},    { 6,    47},    { 6,    48},
    { 6,    51},    { 6,    69},    { 6,    80},    { 7,     1},
    { 7,     2},    { 7,     3},    { 7,     4},    { 7,     5},
    { 7,     6},    { 7,     9},    { 7,    12},    { 7,    13},
    { 7,    18},    { 7,    37},    { 7,    63},    { 8,     1},
    { 8,     2},    { 8,     3},    { 8,     4},    { 8,     5},
    { 8,     6},    { 8,     7},    { 8,     8},    { 8,     9},
    { 8,    10},    { 8,    11},    { 8,    12},    { 8,    13},
    { 8,    14},    { 8,    15},    { 8,    16},    { 8,    17},
    { 8,    18},    { 8,    19},    { 8,    21},    { 8,    22},
    { 8,    23},    { 8,    24},    { 8,    25},    { 8,    26},
    { 8,    27},    { 8,    28},    { 8,    29},    { 8,    30},
    { 8,    31},    { 8,    32},    { 8,    33},    { 8,    34},
    { 8,    35},    { 8,    36},    { 8,    37},    { 8,    38},
    { 8,    40},    { 8,    41},    { 8,    43},    { 8,    44},
    { 8,    45},    { 8,    47},    { 8,    48},    { 8,    49},
    { 8,    50},    { 8,    52},    { 8,    53},    { 8,    54},
    { 8,    56},    { 8,    57},    { 8,    64},    { 8,    67},
    { 8,    69},    { 8,    70},    { 8,    73},    { 8,    75},
    { 8,    78},    { 8,    86},    { 8,    89},    { 8,    90},
    { 8,    94},    { 8,    99},    { 8,   106},    { 8,   107},
    { 8,   108},    { 8,   123},    { 8,   135},    { 8,   196},
    { 8,   197},    { 8,   222},    { 8,   268},    { 8,   311},
    { 8,   332},    { 8,   363},    { 8,   366},    { 8,   542},
    { 8,   569},    { 8,   583},    { 8,   620},    { 8,   755},
    { 8,   922},    { 8,  1071},    { 8,  1143},    { 8,  4149},
    { 8,  5762},    { 8,  6125},    { 8,  6582},    { 8, 20950},
    { 8, 42711},    { 9,     1},    { 9,     8},    {10,     1},
    {10,     2},    {10,     3},    {10,     4},    {10,     5},
    {10,     6},    {10,     7},    {10,     8},    {10,     9},
    {10,    17},    {10,    23},    {10,    25},    {10,    26},
    {10,    32},    {10,    34},    {10,    36},    {10,    38},
    {10,    40},    {10,    47},    {10,    51},    {10,    86},
    {11,     1},    {11,     2},    {11,     3},    {11,     4},
    {11,     5},    {11,     6},    {11,     7},    {11,     8},
    {11,    16},    {11,    46},    {12,     1},    {12,     2},
    {12,     3},    {12,     4},    {13,     1},    {13,     2},
    {13,     3},    {13,     4},    {13,     5},    {13,     6},
    {13,     7},    {13,     8},    {13,     9},    {13,    10},
    {13,    11},    {13,    12},    {13,    13},    {13,    14},
    {13,    15},    {13,    16},    {13,    17},    {13,    18},
    {13,    21},    {13,    22},    {13,    27},    {13,    32},
    {13,    36},    {13,    45},    {13,    50},    {13,    54},
    {13,    55},    {13,   112},    {13,   240},    {14,    10},
    {14,    50},    {15,     1},    {15,     3},    {15,     4},
    {15,     5},    {15,     9},    {15,    10},    {15,    35},
    {15,    53},    {15,   111},    {16,     1},    {16,     2},
    {16,     3},    {16,     4},    {16,     5},    {16,     6},
    {16,     7},    {16,     8},    {16,     9},    {16,    10},
    {16,    13},    {16,    15},    {16,    16},    {16,    18},
    {16,    19},    {16,    20},    {16,    22},    {16,    27},
    {16,    30},    {16,    31},    {16,    45},    {16,    46},
    {16,    60},    {17,     1},    {17,     2},    {17,     3},
    {18,     1},    {18,     2},    {18,     6},    {19,     1},
    {19,     2},    {20,     1},    {21,     1},    {21,     2},
    {22,     1},    {22,     2},    {22,     3},    {22,     4},
    {22,     5},    {22,     6},    {22,     7},    {22,     8},
    {22,     9},    {22,    10},    {22,    11},    {22,    13},
    {22,    14},    {22,    15},    {22,    23},    {23,     1},
    {24,     1},    {24,     2},    {24,     4},    {24,    31},
    {25,     1},    {25,     2},    {25,     3},    {25,     4},
    {25,     5},    {25,     7},    {25,    14},    {25,    16},
    {25,    17},    {25,    23},    {26,     1},    {26,     2},
    {26,     3},    {26,     4},    {26,     5},    {26,     6},
    {26,     8},    {26,    16},    {26,    21},    {26,    25},
    {26,    31},    {26,    32},    {26,    63},    {26,   131},
    {26,   258},    {26,   268},    {27,     1},    {27,     2},
    {27,     3},    {27,     4},    {27,     5},    {27,     6},
    {27,     7},    {27,     8},    {27,     9},    {27,    10},
    {27,    11},    {27,    12},    {27,    13},    {27,    14},
    {27,    15},    {27,    17},    {27,    18},    {27,    20},
    {27,    26},    {27,    29},    {27,    30},    {27,    31},
    {27,    32},    {27,    34},    {27,    36},    {27,    37},
    {27,    39},    {27,    40},    {27,    44},    {27,    45},
    {27,    48},    {27,    54},    {27,    55},    {27,    56},
    {27,    59},    {27,    60},    {27,    61},    {27,    63},
    {27,    64},    {27,    66},    {27,    78},    {27,    81},
    {27,    85},    {27,    87},    {27,    89},    {27,   100},
    {27,   111},    {27,   116},    {27,   183},    {27,   214},
    {27,   246},    {27,   248},    {27,   251},    {27,   256},
    {27,   512},    {27,   723},    {28,     1},    {29,     1},
    {30,     1},    {30,    11},    { 0,     0}
};

/**
 * Classify_RLE contains indices to pairs in the Classify_uniq table.
 * The sequence of pairs is used to fill the Classify array.
 */
static const unsigned Classify_RLE[3697] = {
 81, 456, 354, 368, 354, 367, 347, 352, 382, 352, 344, 353, 307, 353, 384, 353,
255, 367, 352, 347, 372, 341, 372, 122, 367, 382, 347, 382,  82, 456, 352, 370,
398, 352, 372, 398, 151, 350, 382,  83, 398, 372, 398, 382, 319, 372, 107, 353,
372, 318, 151, 349, 320, 352, 253, 382, 249, 120, 382, 114, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 108, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 108, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 244, 107, 243, 107, 243, 109, 244, 107, 243, 107, 244, 107, 245, 108,
246, 107, 244, 107, 245, 109, 244, 107, 244, 107, 243, 107, 243, 107, 244, 107,
243, 108, 243, 107, 244, 107, 245, 107, 243, 107, 244, 108, 151, 243, 109, 154,
243, 241, 107, 243, 241, 107, 243, 241, 107, 243, 107, 243, 107, 243, 107, 243,
107, 243, 107, 243, 107, 243, 107, 243, 108, 243, 107, 243, 107, 243, 107, 243,
107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 108, 243, 241, 107, 243, 107,
245, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 113, 244, 107, 244, 108,
243, 107, 246, 107, 243, 107, 243, 107, 243, 107, 243, 137, 151, 123, 148, 375,
146, 378, 143, 377, 139, 372, 139, 380, 305, 243, 107, 243, 107, 139, 372, 243,
107,   1, 139, 109, 352, 243,   3, 373, 243, 352, 245,   0, 243,   0, 244, 107,
252,   0, 251, 127, 243, 108, 245, 109, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 111,
243, 107, 382, 243, 107, 244, 108, 262, 135, 243, 107, 243, 107, 243, 107, 243,
107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243,
107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 398, 282, 275, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 244, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 108, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107,   0, 259,   1, 139, 357,   0, 130,   0, 352, 344,   1, 399, 368,   0,
301, 344, 278, 352, 279, 352, 279, 352, 278,   7, 176,   4, 153, 353,  10,  87,
384, 353, 368, 353, 399, 288, 352,  83,   0, 353, 181, 139, 160, 296, 307, 355,
152, 278, 213, 352, 151, 284,  83, 398, 283, 140, 279, 398, 281, 152, 307, 153,
399, 151, 364,   0,  83, 151, 278, 179, 298,   1, 210, 288, 151,  13, 307, 182,
286, 140, 398, 354, 139,   4, 171, 281, 139, 286, 139, 280, 139, 282,   1, 365,
  0, 174, 280,   1, 352,  41, 170,   0, 158,  19, 291,  83, 299, 264, 199, 278,
264, 278, 151, 266, 285, 267, 278, 265, 151, 284, 160, 279, 353, 307, 352, 139,
165, 278, 265,   0, 158,   1, 152,   1, 171,   0, 157,   0, 151,   2, 154,   1,
278, 151, 266, 281,   1, 265,   1, 265, 278, 151,   7, 264,   3, 152,   0, 153,
279,   1, 307, 152, 369, 323, 398, 368,   4, 279, 264,   0, 156,   3, 152,   1,
171,   0, 157,   0, 152,   0, 152,   0, 152,   1, 278,   0, 266, 279,   3, 279,
  1, 280,   2, 278,   6, 154,   0, 151,   6, 307, 279, 153, 278,  10, 279, 264,
  0, 159,   0, 153,   0, 171,   0, 157,   0, 152,   0, 155,   1, 278, 151, 266,
282,   0, 279, 264,   0, 265, 278,   1, 151,  14, 152, 279,   1, 307, 352, 368,
  6, 151,   6, 278, 265,   0, 158,   1, 152,   1, 171,   0, 157,   0, 152,   0,
155,   1, 278, 151, 264, 278, 264, 281,   1, 265,   1, 265, 278,   7, 278, 264,
  3, 152,   0, 153, 279,   1, 307, 398, 151, 323,   9, 278, 151,   0, 156,   2,
153,   0, 154,   2, 152,   0, 151,   0, 152,   2, 152,   2, 153,   2, 162,   3,
265, 278, 265,   2, 266,   0, 266, 278,   1, 151,   5, 264,  13, 307, 320, 403,
368, 398,   4, 278, 266,   0, 158,   0, 153,   0, 172,   0, 166,   2, 151, 280,
267,   0, 280,   0, 281,   6, 279,   0, 153,   4, 152, 279,   1, 307,   7, 324,
398, 151, 278, 265,   0, 158,   0, 153,   0, 172,   0, 160,   0, 155,   1, 278,
151, 264, 278, 268,   0, 278, 265,   0, 265, 279,   6, 265,   6, 151,   0, 152,
279,   1, 307,   0, 152,  13, 278, 265,   0, 158,   0, 153,   0, 189,   1, 151,
266, 281,   0, 266,   0, 266, 278, 151, 398,   3, 153, 264, 324, 153, 279,   1,
307, 326, 398, 156,   1, 265,   0, 168,   2, 173,   0, 159,   0, 151,   1, 157,
  2, 278,   3, 266, 280,   0, 278,   0, 271,   5, 307,   1, 265, 352,  11, 194,
278, 152, 284,   3, 368, 156, 139, 285, 352, 307, 353,  27, 152,   0, 151,   1,
152,   0, 151,   1, 151,   5, 154,   0, 157,   0, 153,   0, 151,   0, 151,   1,
152,   0, 154, 278, 152, 283,   0, 279, 151,   1, 155,   0, 139,   0, 283,   1,
307,   1, 154,  24, 151, 400, 365, 398, 352, 400, 279, 403, 307, 327, 398, 278,
398, 278, 398, 278, 367, 347, 367, 347, 265, 158,   0, 185,   3, 291, 264, 282,
352, 279, 155, 288,   0, 300,   0, 405, 278, 403,   0, 399, 356, 401, 353,  27,
190, 265, 281, 264, 283, 264, 279, 265, 279, 151, 307, 357, 156, 265, 279, 154,
280, 151, 266, 152, 270, 153, 281, 163, 278, 265, 279, 269, 278, 151, 264, 307,
266, 278, 399, 259,   0, 243,   4, 243,   1, 190, 352, 139, 224,   0, 154,   1,
157,   0, 151,   0, 154,   1, 189,   0, 154,   1, 182,   0, 154,   1, 157,   0,
151,   0, 154,   1, 165,   0, 201,   0, 154,   1, 203,   1, 280, 360, 333,   2,
166, 407,   5, 263,   1, 112,   1, 344, 230, 353, 167, 456, 175, 367, 347,   2,
207, 354, 310, 158,   6, 163,   0, 154, 280,  10, 168, 280, 353,   8, 168, 279,
 11, 163,   0, 153,   0, 279,  11, 197, 279, 264, 284, 271, 278, 265, 288, 354,
139, 354, 368, 151, 278,   1, 307,   5, 327,   5, 357, 344, 355, 280,  83,   0,
307,   5, 184, 139, 197,   7, 155, 279, 183, 278, 151,   4, 205,   9, 180,   0,
280, 267, 279, 266,   3, 265, 278, 269, 280,   3, 398,   2, 353, 307, 179,   1,
155,  10, 191,   3, 175,   5, 307, 318,   2, 421, 172, 279, 265, 278,   1, 353,
198, 264, 278, 264, 284,   0, 278, 264, 278, 265, 285, 269, 287,   1, 278, 307,
  5, 307,   5, 358, 139, 357,   1, 291, 274,  41, 281, 264, 193, 278, 264, 282,
264, 278, 268, 278, 265, 157,   3, 307, 358, 407, 286, 406,   2, 279, 264, 179,
264, 281, 265, 279, 264, 280, 152, 307, 191, 278, 264, 279, 266, 278, 264, 280,
265,   7, 355, 185, 271, 285, 265, 279,   2, 356, 307,   2, 153, 307, 179, 144,
353, 115,  37, 359,   7, 280, 352, 290, 264, 284, 154, 278, 154, 265, 278, 152,
  0, 279,   5, 133, 150, 117, 139, 126, 149, 303,   4, 282, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 115, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 115, 250, 112,   1, 248,   1, 114, 250, 114, 250, 112,   1, 248,
  1, 114,   0, 243,   0, 243,   0, 243,   0, 243, 114, 250, 118,   1, 114, 242,
114, 242, 114, 242, 111,   0, 108, 246, 241, 372, 107, 374, 109,   0, 108, 246,
241, 374, 110,   1, 108, 246,   0, 374, 114, 247, 374,   1, 109,   0, 108, 246,
241, 373,   0, 457,  86, 346, 353, 350, 349, 367, 351, 349, 367, 350, 359, 454,
455,  86, 456, 360, 350, 349, 355, 342, 354, 382, 367, 347, 362, 382, 352, 341,
361, 456,  86,  91,  89, 318, 139,   1, 323, 384, 367, 347, 139, 327, 384, 367,
347,   0, 147,   2, 371,  16, 290, 277, 278, 276, 289,  14, 399, 243, 401, 243,
399, 107, 245, 108, 245, 107, 398, 243, 399, 382, 247, 403, 243, 398, 243, 398,
243, 398, 246, 398, 107, 246, 107, 154, 107, 399, 108, 244, 386, 243, 110, 398,
382, 399, 107, 398, 330, 315, 243, 107, 311, 318, 399,   3, 386, 402, 383, 401,
382, 399, 382, 399, 382, 404, 382, 419, 383, 399, 382, 398, 382, 419, 397, 405,
367, 347, 367, 347, 415, 383, 404, 367, 347, 439, 382, 418, 391, 425, 387, 417,
 91, 424,  97, 408,  96, 340, 438, 334, 446, 382, 406, 382, 429, 388, 444, 382,
449, 367, 347, 367, 347, 367, 347, 367, 347, 367, 347, 367, 347, 367, 347, 336,
426, 386, 367, 347, 392, 367, 347, 367, 347, 367, 347, 367, 347, 367, 347, 389,
451, 395, 367, 347, 367, 347, 367, 347, 367, 347, 367, 347, 367, 347, 367, 347,
367, 347, 367, 347, 367, 347, 367, 347, 394, 367, 347, 367, 347, 393, 367, 347,
396, 428, 390, 399, 387, 424,  92, 420,  92, 421,  93, 409,  91, 405,  98, 401,
 95, 261,   0, 134,   0, 243, 107, 245, 108, 243, 107, 243, 107, 243, 107, 246,
107, 243, 108, 243, 112, 140, 245, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 108, 403, 243, 107, 243, 107, 280,
243, 107,   4, 355, 318, 353, 129,   0, 107,   4, 107,   1, 200,   6, 139, 352,
 13, 278, 172,   8, 157,   0, 157,   0, 157,   0, 157,   0, 157,   0, 157,   0,
157,   0, 157,   0, 299, 353, 350, 349, 350, 349, 354, 350, 349, 352, 350, 349,
360, 344, 353, 344, 352, 350, 349, 353, 350, 349, 367, 347, 367, 347, 367, 347,
367, 347, 356, 139, 361, 345, 355, 344, 352, 367, 353, 101, 416,   0, 442,  11,
447,  22, 409,   3, 456, 354, 398, 139, 151, 309, 367, 347, 367, 347, 367, 347,
367, 347, 367, 347, 399, 367, 347, 367, 347, 367, 347, 367, 347, 344, 367, 348,
398, 313, 281, 265, 344, 143, 399, 310, 139, 151, 352, 399,   0, 209,   1, 279,
373, 140, 151, 344, 211, 352, 141, 151,   4, 189,   2, 212,   0, 399, 321, 407,
176,   4, 422,  11, 166, 419,   0, 327, 418, 325, 398, 329, 420, 327, 424, 329,
435,   0, 451, 238,   9, 436, 239,  30, 170, 139, 234,   2, 430,   8, 188, 144,
353, 222, 139, 354, 166, 307, 152,  18, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 151, 278, 276, 352, 287, 352, 139, 243, 107, 243,
107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243,
107, 243, 107, 243, 107, 243, 107, 243, 107, 140, 279, 205, 314, 279, 357,   7,
381, 145, 373, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243,
109, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243,
107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243,
107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243,
107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 139,
114, 243, 107, 243, 107, 244, 107, 243, 107, 243, 107, 243, 107, 243, 107, 139,
373, 243, 107, 243, 107, 151, 243, 107, 243, 109, 243, 107, 243, 107, 243, 107,
243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 243, 107, 247,   0,
247, 107, 243, 107,  39, 151, 140, 107, 157, 278, 153, 278, 154, 278, 172, 265,
279, 264, 401,   3, 323, 399, 368, 398,   5, 197, 355,   7, 265, 196, 272, 279,
  7, 353, 307,   5, 295, 156, 354, 151, 352, 151,   1, 307, 177, 285, 353, 172,
288, 265,  10, 352, 178,   2, 280, 264, 193, 278, 265, 281, 265, 278, 267, 363,
  0, 139, 307,   3, 353, 155, 278, 139, 159, 307, 155,   0, 189, 283, 265, 279,
265, 279,   8, 153, 278, 158, 278, 264,   1, 307,   1, 355, 166, 139, 156, 400,
151, 264, 278, 264, 196, 278, 151, 280, 152, 279, 155, 279, 151, 278, 151,  21,
152, 139, 353, 161, 264, 279, 265, 353, 151, 140, 264, 278,   9, 156,   1, 156,
  1, 156,   8, 157,   0, 157,   0, 132, 372, 142, 112,   9, 138, 184, 265, 278,
265, 278, 265, 352, 264, 278,   1, 307,   5, 151,  77, 151,  11, 172,   3, 195,
  3, 105,  62, 106,  46, 106,  63, 105, 104,  73, 104, 226,   1, 214,  28, 113,
 11, 111,   4, 151, 278, 160, 382, 163,   0, 155,   0, 151,   0, 152,   0, 152,
  0, 216, 379,  16, 225, 347, 367,  15, 202,   1, 199,   7, 100, 162, 368, 398,
  1, 293, 358, 367, 347, 352,   5, 293, 352, 345, 342, 367, 347, 367, 347, 367,
347, 367, 347, 367, 347, 367, 347, 367, 347, 367, 347, 353, 367, 347, 355, 343,
354,   0, 355, 344, 367, 347, 367, 347, 367, 347, 354, 382, 344, 384,   0, 352,
368, 353,   3, 155,   0, 218,   1,  83,   0, 354, 368, 354, 367, 347, 352, 382,
352, 344, 353, 307, 353, 384, 353, 255, 367, 352, 347, 372, 341, 372, 122, 367,
382, 347, 382, 367, 347, 352, 367, 347, 353, 160, 139, 192, 140, 180,   2, 156,
  1, 156,   1, 156,   1, 153,   2, 369, 382, 372, 398, 369,   0, 398, 385, 399,
  0,  94,  84, 399,  92, 162,   0, 175,   0, 169,   0, 152,   0, 165,   1, 164,
 26, 217,   4, 354,   3, 338,   2, 406, 316, 321, 413, 319, 400,   0, 409,   3,
398,  33, 427, 278,  47, 178,   2, 195,  14, 278, 335,   3, 181, 321,  11, 167,
309, 158, 309,   4, 187, 282,   4, 179,   0, 352, 185,   3, 158, 352, 312,  30,
260, 131, 208,   1, 307,   5, 258,   3, 128,   3, 188,   7, 197,  10, 352,  50,
223,   8, 171,   9, 158,  51, 156,   1, 151,   0, 191,   0, 152,   2, 151,   1,
172,   0, 352, 325, 172, 399, 324, 180,   7, 326,  34, 169,   0, 152,   4, 322,
171, 323,   2, 352, 175,   4, 352,  40, 200,   3, 319, 152, 330,   1, 339, 151,
280,   0, 279,   4, 281, 154,   0, 153,   0, 176,   3, 280,   3, 278, 325,   7,
360,   6, 178, 319, 352, 178, 320,  24, 158, 398, 177, 279,   3, 322, 358,   8,
199,   2, 358, 171,   1, 325, 169,   4, 325, 168,   6, 355,  11, 324,  42, 206,
 37, 262,  12, 136,   6, 323,  57, 337,  58, 264, 278, 264, 198, 292, 358,   3,
333, 307,  14, 280, 264, 192, 266, 281, 265, 279, 353,  83, 355,  13, 174,   6,
307,   5, 280, 185, 282, 264, 285,   0, 307, 355,  11, 184, 278, 353, 151,   8,
279, 264, 194, 266, 286, 265, 154, 356, 280, 352,   1, 307, 151, 352, 151, 354,
  0, 333,  10, 168,   0, 174, 266, 280, 265, 278, 264, 279, 357, 278,  41, 157,
  0, 151,   0, 154,   0, 165,   0, 160, 352,   5, 193, 278, 266, 285,   4, 307,
  5, 279, 265,   0, 158,   1, 152,   1, 171,   0, 157,   0, 152,   0, 155,   1,
278, 151, 265, 278, 267,   1, 265,   1, 266,   1, 151,   5, 264,   4, 155, 265,
  1, 284,   2, 282,  48, 198, 266, 285, 265, 280, 264, 278, 154, 356, 307,   0,
352,   0, 352,  26, 194, 266, 283, 264, 278, 267, 279, 264, 279, 152, 352, 151,
  7, 307,  52, 193, 266, 281,   1, 267, 279, 264, 279, 366, 154, 279,  26, 194,
266, 285, 265, 278, 264, 279, 354, 151,  10, 307,   5, 363,  17, 190, 278, 264,
278, 265, 283, 264, 278,   7, 307,  36, 175,   2, 280, 265, 281, 264, 282,   3,
307, 319, 354, 398,  57, 256, 125, 307, 326,  11, 151,  59, 201,  55, 159,   0,
186, 264, 284,   0, 283, 264, 278, 151, 356,   9, 307, 332,   2, 353, 179,   1,
297,   0, 264, 284, 264, 279, 264, 279,  60, 232,  45, 317,   0, 356,  10, 219,
 69, 233,  71, 229,  74, 228,   6, 180,   0, 307,   3, 353,  43, 179,   1, 282,
352,   9, 194, 284, 356, 401, 142, 352, 398,   9, 307,   0, 324,   0, 170,   4,
169,  61, 204,  10, 151, 273,  15, 281, 147,  40, 139,  23, 237,  17, 231,  75,
152,  70, 215,   4, 163,   2, 159,   6, 160,   1, 398, 279, 352,  85,  72, 448,
  9, 424,   1, 433, 265, 280, 400, 269,  88, 285, 399, 284, 418, 281, 432,  20,
437, 280, 398,  54, 441,   8, 331,  49, 255, 122, 255, 113,   0, 119, 255, 122,
243,   0, 244,   1, 243,   1, 244,   1, 246,   0, 250, 110,   0, 107,   0, 113,
  0, 116, 255, 122, 244,   0, 246,   1, 250,   0, 249,   0, 122, 244,   0, 246,
  0, 247,   0, 243,   2, 249,   0, 122, 255, 122, 255, 122, 255, 122, 255, 122,
255, 122, 255, 124,   1, 254, 382, 121, 382, 112, 254, 382, 121, 382, 112, 254,
382, 121, 382, 112, 254, 382, 121, 382, 112, 254, 382, 121, 382, 112, 243, 107,
  1, 308, 452, 304, 401, 302, 405, 278, 411, 278, 399, 356,  14, 282,   0, 292,
 65, 284,   0, 294,   1, 284,   0, 279,   0, 282,  68, 220,   1, 326, 284,  29,
257, 126, 284,   4, 307,   3, 353,  64, 154,   0, 176,   0, 152,   0, 151,   1,
151,   0, 160,   0, 154,   0, 151,   0, 151,   5, 151,   3, 151,   0, 151,   0,
151,   0, 153,   0, 152,   0, 151,   1, 151,   0, 151,   0, 151,   0, 151,   0,
151,   0, 152,   0, 151,   1, 154,   0, 157,   0, 154,   0, 154,   0, 151,   0,
160,   0, 167,   4, 153,   0, 155,   0, 167,  35, 383,  56, 426,   3, 443,  11,
412,   1, 412,   0, 412,   0, 423,   9, 328,   2, 419,   0, 433,   3, 434,  38,
417,  12, 426,   3, 406,   6, 399,  53, 450, 376, 453,  12, 410,   2, 404,   8,
445,  11, 440,  31, 409,   3, 431,   7, 407,   5, 425,   7, 418,  44, 412,   0,
405,   7, 398,   1, 409,   0, 409,   3, 412,  25, 414,  32, 398,  67,  92, 240,
 29, 235,  10, 221,   1, 236,  76, 227,  66,  92,  80,  92,  80,  92,  80,  92,
 80,  92,  80,  92,  80,  92,  80,  92,  80,  92,  80,  92,  80,  92,  80,  93,
 83,  99,  90, 102, 306, 103,  78,  92, 104,  79, 104,  92, 104,  79, 104,  91,
  0
};

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

    for (unsigned i=0; i < 3697 ; ++i) {
        const unsigned *pair = Classify_uniq[Classify_RLE[i]];
        unsigned index = pair[0];
        unsigned width = pair[1];

        for (unsigned n=0; n < width; ++n)
            Classify[codepoint++] = (unsigned char)index;
    }
}
