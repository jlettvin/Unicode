/* passfail.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <stdio.h>

#include "passfail.h"

void passfail(const int T, const char* message) {
    const char* pf[2] = {"[PASS]", "[FAIL]"};
    static int count[2] = {0, 0};
    if (message[0] == 0) {
        printf("%s %d\t", pf[0], count[0]);
        printf("%s %d\t", pf[1], count[1]);
        puts("RESULT");
    } else {
        int sense = !T;
        count[sense]++;
        printf("%s %s\n", pf[sense], message);
    }
}
