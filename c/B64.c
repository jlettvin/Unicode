/* B64.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */

#include "B64.h"

typedef unsigned int uint_t;

static struct {
    char* encode;       ///< the B64 encoding alphabet
    char  decode[256];  ///< that alphabet turned backwards by B64_init.
} B64_static = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=",
    {}
};

/** B64_init
 *
 * Run before main to initialize data structures.
 */
__attribute__((constructor))
void B64_init(void) {
    char* encode = B64_static.encode;
    char* decode = B64_static.decode;
    for (int i=0; i < 256; ++i) decode[i] = 64;
    for (int i=0; encode[i]; ++i) decode[encode[i]&0xff] = i;
}

/** B64_decode
 *
 * \param src is a sequence of B64 encoded characters
 * \param tgt is a buffer into which src is decoded
 * \return nothing
 *
 * Note that computed GOTO is used liberally to eliminate conditionals.
 */
void B64_decode(const char* src, char* tgt) {
#define __NEXT(N) U = (uint_t)(d=*src++); goto *Z[N][(!d) || (d == '=')]
    static const void *Z[4][2] =
        { {&&B, &&F}, {&&C, &&F}, {&&D, &&F}, {&&E, &&F} };
    char* decode = B64_static.decode;
    char d, m;
    uint_t U;
A:  __NEXT(0);
B:  m = decode[U]; *tgt  = m << 2;                  __NEXT(1);
C:  m = decode[U]; *tgt |= m >> 4; *++tgt = m << 4; __NEXT(2);
D:  m = decode[U]; *tgt |= m >> 2; *++tgt = m << 6; __NEXT(3);
E:  m = decode[U]; *tgt |= m     ; *++tgt = m     ;   goto A;
F:  *tgt = 0;
#undef __NEXT
    return;
}

/** B64_encode
 *
 * \param src is a sequence of 8 bit characters
 * \param tgt is a buffer into which src is encoded
 * \return nothing
 *
 * Note that computed GOTO is used liberally to eliminate conditionals.
 */
void B64_encode(const char* src, char *tgt) {
    static const void *Z[4][2] =
        { {&&B, &&E0}, {&&C, &&E2}, {&&D, &&E1}, {&&A, &&E0} };
    char* encode = B64_static.encode;
    char s, t;
A:  s = *src++; goto *Z[0][!s];
B:  *tgt++ = encode[    (s >> 2)]; t = (s & 0x3) << 4;
    *tgt = encode[t & 0xff]; s = *src++; goto *Z[1][!s];
C:  *tgt++ = encode[t | (s >> 4)]; t = (s & 0xf) << 2; *tgt = encode[t & 0xff];
    s = *src++; goto *Z[2][!s];
D:  *tgt++ = encode[t | (s >> 6)];
    *tgt++ = encode[(s & 0x3f)];
    goto *Z[3][!s];
E2: *++tgt = '=';
E1: *++tgt = '='; *++tgt = 0;
E0: *tgt = 0;
}
