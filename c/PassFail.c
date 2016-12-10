/* PassFail.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <stdio.h>

#include "PassFail.h"

/** passfail
 *
 * \param T a 0 or 1 value (boolean) to decide reporting.
 * \return nothing.
 *
 * This function reports comparisons where T==PASS and F=FAIL.
 * Use of this function with an empty string causes a tally of results so far.
 */
void passfail(
        const int T,
        const char*file,
        const unsigned line,
        const char* message)
{
    const char* pf[2] = {"[PASS]", "[FAIL]"};
    static int count[2] = {0, 0};
    if (message[0] == 0) {
        printf("%s (%s:%u) %d\t", pf[0], file, line, count[0]);
        if (count[1]) puts("");
        printf("%s (%s:%u) %d\t", pf[1], file, line, count[1]);
        puts("RESULT");
    } else {
        int sense = !T;
        count[sense]++;
        printf("%s (%s:%u) %s\n", pf[sense], file, line, message);
    }
}
