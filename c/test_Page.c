/* test_Page.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <stdio.h>

#include "PassFail.h"
#include "Page.h"

/** test_page
 *
 * \return nothing
 *
 * Output both ways and show the final tally by using a 0-length message.
 */
void test_page() {
    ue0_t u0;

    u0 = Page_peek_ue0_t(0);
    PASSFAIL(u0 == (ue0_t)0, "Unused page memory first peek");

    Page_poke_ue0_t(0, 255);
    u0 = Page_peek_ue0_t(0);
    PASSFAIL(u0 == (ue0_t)255, "First poke page memory");

    PASSFAIL(1, "");
}

/** main
 *
 * Entirely ordinary main.
 * Without args, the tests are run.
 * With args, argc and argv are used (avoids lint warnings).
 */
int main(int argc, char **argv) {
    test_page();
    while (--argc) puts(*++argv);
    return 0;
}

