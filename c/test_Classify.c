#include <stdio.h>
#include <string.h>

#include "PassFail.h"
#include "Classify.h"

/** test_codepoint
 *
 * \param codepoint is a legal value for Unicode 9.0
 * \param expect is a two character Unicode classification string.
 * \return nothing
 *
 * This test determines that the codepoint classification matches
 * the hand-determined classification string.
 *
 * TODO Identify why there are so many [FAIL]s when count is so low.
 */
void test_codepoint(unsigned codepoint, char* expect) {
    PASSFAIL(!strcmp(Classify_Label[Classify[codepoint]], expect), "classify");
}

/** test_codepoints
 *
 * \return nothing
 *
 * This test runs the singular test function above on several codepoints.
 */
void test_codepoints() {
    for (char c='0'; c <= '9'; ++c) test_codepoint((unsigned)c, "Nd");
    for (char c='A'; c <= 'Z'; ++c) test_codepoint((unsigned)c, "Lu");
    for (char c='a'; c <= 'z'; ++c) test_codepoint((unsigned)c, "Ll");
    PASSFAIL(0, "");
}

/** main
 *
 * Entirely ordinary main.
 * Without args, the tests are run.
 * With args, argc and argv are used (avoids lint warnings).
 */
int main(int argc, char **argv) {
    test_codepoints();
    while (--argc) puts(*++argv);
    return 0;
}
