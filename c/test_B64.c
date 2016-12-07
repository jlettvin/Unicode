#include <stdio.h>
#include <string.h>

#include "PassFail.h"
#include "B64.h"

static const char* sample[4][3] = {
    {
        "Letter",
        "M",
        "TQ=="
    }, {
        "Paired",
        "Ma",
        "TWE="
    }, {
        "Minimal",
        "Man",
        "TWFu"
    }, {
        "Content",
        "Man is distinguished, not only by his reason, but by this singular "
        "passion from other animals, which is a lust of the mind, that by a "
        "perseverance of delight in the continued and indefatigable "
        "generation of knowledge, exceeds the short vehemence of any "
        "carnal pleasure.",
        "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBie"
        "SB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcy"
        "BhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWd"
        "odCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yg"
        "a25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hb"
        "CBwbGVhc3VyZS4="
    }
};

void test_B64(const int index) {
    const char *titling = sample[index][0];
    const char *decoded = sample[index][1];
    const char *encoded = sample[index][2];

    char dtarget[strlen(decoded)+2];  
    char etarget[strlen(encoded)+2];  
    unsigned lengths, compare;
    unsigned source, target;

    printf("\t%s:\n", titling);

    B64_decode(encoded, dtarget);
    source = strlen(decoded);
    target = strlen(dtarget);
    lengths = (source == target);
    compare = !strcmp(dtarget, decoded);
    passfail(lengths, "decode string lengths");
    passfail(compare, "decode string content");

    B64_encode(dtarget, etarget);
    source = strlen(encoded);
    target = strlen(etarget);
    lengths = (source == target);
    compare = !strcmp(etarget, encoded);
    passfail(lengths, "encode string lengths");
    passfail(compare, "encode string content");
    if (!(lengths && compare)) {
        puts(etarget);
        puts(encoded);
        printf("[FAIL] %u == %u ?\n", source, target);
        B64_decode(etarget, dtarget);
        puts(dtarget);
    }
    passfail(0, "");
}

void test() {
    for (int i = 0; i < 4; ++i) test_B64(i);
}

/* Use this approach to execute code before main. */
__attribute__ ((constructor))
void premain(void) {
    puts("premain");
}

int main(int argc, char **argv) {
    puts("main");
    if (argc == 1) {
        test();
    } else while (--argc) {
        char target[256];
        puts(*++argv);
        B64_decode(*argv, target);
        puts(target);
        puts("");
    }
    return 0;
}
