/* PassFail.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <stdio.h>

#include "PassFail.h"

/** passfail
 *
 * \param count is 0 [FAIL] or nonzero [PASS] to decide reporting.
 * \return nothing.
 *
 * This function reports comparisons where T==PASS and F=FAIL.
 * Use of this function with an empty string causes a tally of results so far.
 *
 * count==0 msg==""  reports
 * count==1 msg==""  disables
 * msg!="" acquires results
 */
void passfail(
        const int count,
        const char* source,
        const unsigned line,
        const char* msg) {
    const char* pf[2] = {"[PASS]", "[FAIL]"};
    static int tally[2] = {0, 0};
    static int disable = 0;

    FILE* handle = fopen("PassFail.out", "a");

    if (msg[0] == 0) {
        if (count == 1) disable = 1;
        else if(!disable) {
            fprintf(handle, "%s (%s:%u) %d\t", pf[0], source, line, tally[0]);
            if (tally[1] != count) fprintf(handle, "\n");
            fprintf(handle, "%s (%s:%u) %d\t", pf[1], source, line, tally[1]);
            fprintf(handle, "RESULT\n");
        }
    } else if(!disable) {
        tally[!count]++;
        fprintf(handle, "%s (%s:%u) %s\n", pf[!count], source, line, msg);
    }

    fclose(handle);
}
