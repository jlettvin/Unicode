/* Unicode.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bzlib.h>
#include <assert.h>

#include "PassFail.h"
#include "B64.h"
#include "Unicode.h"

typedef unsigned char ubyte_t;                 ///< imitate c++ type
typedef unsigned long size_t;                  ///< imitate c++ type
typedef size_t char32_t;                       ///< imitate c++ type

typedef size_t codepoint_t;                    ///< clear type of codepoint
typedef size_t target_t;                       ///< clear type of result
typedef size_t either_t;                       ///< ambiguous type

// Endian invariant handling
typedef union { size_t u32; ubyte_t u8[4]; } invariant32_t;
typedef invariant32_t in32_t;
static const in32_t in32 = { .u32 = 0x03020100 };
ubyte_t endless(void* p, size_t o) { return (*((in32_t *)p)).u8[in32.u8[o]]; }

void bits(char x) {
    static const char* digit = "10";
    for (int i=8; i--; ) putchar(digit[!(x & (1 << i))]);
}

void char32_t_to_UTF8(char32_t source, char target[5]) {
        static const void *assemble[] = {&&u7, &&u11, &&u16, &&u21, &&err};
        goto *assemble[
            (size_t)(source > 0x7f) +
            (size_t)(source > 0x7ff) +
            (size_t)(source > 0xffff) +
            (size_t)(source > sentinel)];
u7:     target[0] =         (char)(source);
        target[1] =         0;
        return;
u11:    target[0] = 0xC0 + (char)((source >> 0x06) & 0x1f);
        target[1] = 0x80 + (char)((source        ) & 0x3f);
        target[2] = 0;
        return;
u16:    target[0] = 0xE0 + (char)((source >> 0x0c) & 0x0f);
        target[1] = 0x80 + (char)((source >> 0x06) & 0x3f);
        target[2] = 0x80 + (char)((source        ) & 0x3f);
        target[3] = 0;
        return;
u21:    target[0] = 0xF0 + (char)((source >> 0x12) & 0x07);
        target[1] = 0x80 + (char)((source >> 0x0c) & 0x3f);
        target[2] = 0x80 + (char)((source >> 0x06) & 0x3f);
        target[3] = 0x80 + (char)((source        ) & 0x3f);
        target[4] = 0;
        return;
err:    target[0] = 0;
        return;
}

codepoint_t UTF8_to_char32_t(char* buf, size_t* head, size_t tail) {
        // hello="hello"; hello[head] == 'h'; hello[tail] == '\0';
        static size_t need[] = {
            // The number of bytes needed for an ingest act is specified here.
            // This table is indexed using the top 5 bits of the first byte
            0, 4, 3, 3, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
        };
        static void *alabel[] = {
            // The same top 5 bits indexes this table.
            // This table is filled with the labels for continuation code.
            // A number of top bit patterns are illegal in UTF8 (&&err).
            // For instance, the first byte cannot have the top two bits be 10.
            &&a00, &&a00, &&a00, &&a00, &&a00, &&a00, &&a00, &&a00,
            &&a00, &&a00, &&a00, &&a00, &&a00, &&a00, &&a00, &&a00,
            &&err, &&err, &&err, &&err, &&err, &&err, &&err, &&err,
            &&a1c, &&a1c, &&a1c, &&a1c, &&a1d, &&a1d, &&a1e, &&err
        };
        // Once ingesting, subsequent bytes MUST have the top two bits be 10.
        // These tables guarantee a legal code path or error.
        static void *dblabel[] = { &&err, &&err, &&db, &&err };
        static void *dclabel[] = { &&err, &&err, &&dc, &&err };
        static void *ddlabel[] = { &&err, &&err, &&dd, &&err };
        static void *cblabel[] = { &&err, &&err, &&cb, &&err };
        static void *cclabel[] = { &&err, &&err, &&cc, &&err };
        static void *bblabel[] = { &&err, &&err, &&bb, &&err };
        // These label pairs change the code path depending on whether
        // the incoming buffer has bytes left to ingest and
        // the head/tail args are sufficient to support ingest needs.
        static void *headed[] = { &&err, &&ingest };
        static void *tailed[] = { &&err, &&enough };

        size_t off = *head, high;
        char32_t _ = 0;
        char a, b, c, d;

        goto *headed[(size_t)(*head < tail)];
ingest: a = buf[off++];
        high = (size_t)((a >> 3) & 0x1f);
        goto *tailed[(size_t)(need[high] <= (tail - *head))];
enough: goto *alabel[high];
a1e:    b = buf[off++]; goto *dblabel[(char32_t)((b >> 6) & 0x3)];
db:     c = buf[off++]; goto *dclabel[(char32_t)((c >> 6) & 0x3)];
dc:     d = buf[off++]; goto *ddlabel[(char32_t)((d >> 6) & 0x3)];
dd:     _ = ((char32_t)(a & 0x07) << 0x12) +
            ((char32_t)(b & 0x3f) << 0x0c) +
            ((char32_t)(c & 0x3f) << 0x06) +
            ((char32_t)(d & 0x3f));
        *head = off; return _;
a1d:    b = buf[off++]; goto *cblabel[(char32_t)((b >> 6) & 0x3)];
cb:     c = buf[off++]; goto *cclabel[(char32_t)((c >> 6) & 0x3)];
cc:     _ = ((char32_t)(a & 0x0f) << 0x0c) +
            ((char32_t)(b & 0x3f) << 0x06) +
            ((char32_t)(c & 0x3f));
        *head = off; return _;
a1c:    b = buf[off++]; goto *bblabel[(char32_t)((b >> 6) & 0x3)];
bb:     _ = ((char32_t)(a & 0x1f) << 0x06) +
            ((char32_t)(b & 0x3f));
        *head = off; return _;
a00:    _ = (char32_t)(a);
        *head = off; return _;
err:    return (char32_t)(_);
}

char classifiers[0x110000];  // sentinel+1
