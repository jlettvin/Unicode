/* test_PassFail.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <stdio.h>

#include "PassFail.h"

/** test_passfail
 *
 * \return nothing
 *
 * Output both ways and show the final tally by using a 0-length message.
 */
void test_passfail() {
    PASSFAIL(1, "Expect exactly 1 [FAIL]");
    PASSFAIL(1, "Expect [PASS]");
    PASSFAIL(0, "Expect [FAIL]");
    PASSFAIL(1, "");
}

/** main
 *
 * Entirely ordinary main.
 * Without args, the tests are run.
 * With args, argc and argv are used (avoids lint warnings).
 */
int main(int argc, char **argv) {
    test_passfail();
    while (--argc) puts(*++argv);
    return 0;
}
