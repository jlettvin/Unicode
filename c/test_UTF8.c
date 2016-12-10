/* test_UTF8.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <stdio.h>

#include "PassFail.h"
#include "UTF8.h"

void test_codepoint() {
    codepoint_t source, target;
    unsigned long head, tail;
    char utf8[5];

    head = 0;
    tail = 1;
    source = 0x41;
    char32_t_to_UTF8(source, utf8);
    target = UTF8_to_char32_t(utf8, &head, tail);
    PASSFAIL(source == target, utf8);
}

int main(int argc, char **argv) {
    test_codepoint();
    while (--argc) puts(*++argv);
    return 0;
}
