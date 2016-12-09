/* test_Unicode.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bzlib.h>
#include <assert.h>

#include "PassFail.h"
#include "Unicode.h"

void test_u32string() {
}

void test_codepoint() {
    codepoint_t source, target;
    unsigned long head, tail;
    char utf8[5];

    head = 0;
    tail = 1;
    source = 0x41;
    char32_t_to_UTF8(source, utf8);
    target = UTF8_to_char32_t(utf8, &head, tail);
    passfail(source == target, utf8);
}

int main(int argc, char **argv) {
    puts(argv[0]);
    test_codepoint();
    test_u32string();
    while (--argc) puts(*++argv);
    return 0;
}
