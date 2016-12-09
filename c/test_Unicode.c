/* Unicode.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bzlib.h>
#include <assert.h>

#include "PassFail.h"
#include "B64.h"
#include "Unicode.h"

void test_endian() {
    struct { size_t wide; char*  thin; } d = { 0x64636261,  "abcd" };
    for (int i = 0; i < 4; ++i)
        assert((char)endless(&d.wide, i) == d.thin[i]);
}

void test_labels() {
    /*
    int classified = 0;
    for (size_t i = 0; i <= sentinel; ++i) {
        if (classifiers[i]) {
            printf("%06zx: %s\n", i, label[(size_t)classifiers[i]]);
            classified++;
        }
    }
    printf("%d classified\n", classified);
    */
}

int main(int argc, char **argv) {
    test_endian();
    //init_labels();
    test_labels();
    while (--argc) puts(*++argv);
    return 0;
}
