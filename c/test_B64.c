#include <stdio.h>
#include <string.h>

#include "PassFail.h"
#include "B64.h"

/** Sample array of decoded/encoded pairs (see en.wikipedia.org/wiki/Base64)
 */
static const char* sample[][3] = {
    {
        "Letter",
        "M",
        "TQ=="
    },
    {
        "Paired",
        "Ma",
        "TWE="
    },
    {
        "Minimal",
        "Man",
        "TWFu"
    },
    {
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
    },
    { "", "", "" },  // Prevent any further testing
    { "", "", "" }   // Unnecessary sentinel
};

/** test_B64
 *
 * \param index is the index of the decoded/encoded pairs
 * \return nothing
 *
 * This test determines that the decoded text encodes back identically.
 */
void test_B64(const int index) {
    static const char* format = "%s: string %s";

    const char *titling = sample[index][0];
    const char *decoded = sample[index][1];
    const char *encoded = sample[index][2];

    const size_t dlen = strlen(decoded);
    const size_t elen = strlen(encoded);
    const size_t dlenb = (dlen + 0x10) & 0xfffffff0;
    const size_t elenb = (elen + 0x10) & 0xfffffff0;

    char dtarget[dlenb];  
    char etarget[elenb];  
    unsigned lengths, compare;
    unsigned source, target;
    char buffer[1024];

    B64_decode(encoded, dtarget);
    source = strlen(decoded);
    target = strlen(dtarget);
    lengths = (source == target);
    compare = !strcmp(dtarget, decoded);
    snprintf(buffer, 1023, format, titling, "decode lengths");
    PASSFAIL(lengths, buffer);

    snprintf(buffer, 1023, format, titling, "decode content");
    PASSFAIL(compare, buffer);

    B64_encode(dtarget, etarget);
    source = strlen(encoded);
    target = strlen(etarget);
    lengths = (source == target);
    compare = !strcmp(etarget, encoded);
    snprintf(buffer, 1023, format, titling, "encode lengths");
    PASSFAIL(lengths, buffer);
    snprintf(buffer, 1023, format, titling, "encode content");
    PASSFAIL(compare, buffer);
    PASSFAIL(0, "");
}

/** test
 *
 * This function runs each test for which there is a data pair.
 */
void test() {
    for (size_t i = 0; sample[i][2][0] ; ++i) if (sample[i][0][0]) test_B64(i);
}

/** main
 *
 * Entirely ordinary main.
 * Without args, the tests are run.
 * With args, argc and argv are used (avoids lint warnings).
 */
int main(int argc, char **argv) {
    test();
    while (--argc) puts(*++argv);
    return 0;
}
