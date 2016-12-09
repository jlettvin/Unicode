#include <stdio.h>
#include <string.h>

#include "PassFail.h"
#include "Classify.h"

void test_codepoint(unsigned codepoint, char* expect) {
    passfail(!strcmp(Classify_Label[Classify[codepoint]], expect), "classify");
}

void test_codepoints() {
    puts("test_codepoints");
    for (char c='0'; c <= '9'; ++c) test_codepoint((unsigned)c, "Nd");
    for (char c='A'; c <= 'Z'; ++c) test_codepoint((unsigned)c, "Lu");
    for (char c='a'; c <= 'z'; ++c) test_codepoint((unsigned)c, "Ll");
    passfail(0, "");
}

void test() {
    test_codepoints();
}

int main(int argc, char **argv) {
    if (argc == 1) {
        test();
    } else while (--argc) {
        puts(*++argv);
    }
    return 0;
}
