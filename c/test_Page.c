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
#define PAGE_TEST(type, addr, val0) \
{ \
    type val1; \
    Page_poke_ue0_t(addr, (type)val0); \
    val1 = Page_peek_##type(addr); \
    PASSFAIL(val0 == val1, "compare poke with peek"); \
}

void test_page() {
    page_t_ue0_t* bytes = page_t_ue0_t_ctor();
    PASSFAIL(bytes->peek(bytes, 0) == (ue0_t)0, "New 1st peek");
    bytes->poke(bytes, 0, 0xff);
    PASSFAIL(bytes->peek(bytes, 0) == (ue0_t)0xff, "New 1st poke");
    
    ue0_t u0;
    address_t off = 0;

    u0 = Page_peek_ue0_t(0);
    PASSFAIL(u0 == (ue0_t)0, "Unused page memory first peek");

    PAGE_TEST(ue0_t, 0, 0);
    PAGE_TEST(ue0_t, 0, 0xff);
    PAGE_TEST(ue0_t, 0x10000, 0xff);
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

