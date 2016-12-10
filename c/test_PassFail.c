/* test_PassFail.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bzlib.h>
#include <assert.h>

#include "PassFail.h"

/** test_passfail
 *
 * \return nothing
 *
 * Output both ways and show the final tally by using a 0-length message.
 */
void test_passfail() {
    puts("\tExpect exactly 1 [FAIL]");
    passfail(1, "This shall pass");
    passfail(0, "This shall fail");
    passfail(1, "");
}

/** main
 *
 * Entirely ordinary main.
 * Without args, the tests are run.
 * With args, argc and argv are used (avoids lint warnings).
 */
int main(int argc, char **argv) {
    puts(argv[0]);
    test_passfail();
    while (--argc) puts(*++argv);
    return 0;
}

