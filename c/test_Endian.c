/* test_Endian.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bzlib.h>
#include <assert.h>

#include "PassFail.h"
#include "Endian.h"

/** test_Endian
 *
 * \return nothing
 *
 * This test determines that the byte order is invariant to Endianness
 * when it is indexed using the implemented double dereferencing.
 */
void test_Endian() {
    puts("test_Endian");
    struct { unsigned long wide; char*  thin; } d = { 0x64636261,  "abcd" };
    for (int i = 0; i < 4; ++i) {
        char c = *(char*)Endian_invariant(&d.wide, i);
        passfail(c == d.thin[i], "Endian invariant byte indexing");
    }
}

/** main
 *
 * Entirely ordinary main.
 * Without args, the tests are run.
 * With args, argc and argv are used (avoids lint warnings).
 */
int main(int argc, char **argv) {
    puts(argv[0]);
    test_Endian();
    while (--argc) puts(*++argv);
    return 0;
}
