/* test_Endian.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bzlib.h>
#include <assert.h>

#include "PassFail.h"
#include "Endian.h"

void test_endian() {
    struct { unsigned long wide; char*  thin; } d = { 0x64636261,  "abcd" };
    for (int i = 0; i < 4; ++i) {
        char c = *(char*)endian_invariant(&d.wide, i);
        passfail(c == d.thin[i], "Endian invariant byte indexing");
    }
}

int main(int argc, char **argv) {
    puts(argv[0]);
    test_endian();
    while (--argc) puts(*++argv);
    return 0;
}
