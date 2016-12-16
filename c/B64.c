/* B64.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */

#include <stdio.h>

#include "B64.h"

static struct {
    u1_p encode;       ///< the B64 encoding alphabet
    u1_t decode[256];  ///< that alphabet turned backwards by B64_init.
} B64_static = {
    (u1_p)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=",
    {}
};

/** B64_init
 *
 * Run before main to initialize data structures.
 */
__attribute__((constructor))
void B64_init(void) {
    u1_p encode = B64_static.encode;
    u1_p decode = B64_static.decode;
    for (size_t i=0; i < 256; ++i) decode[i] = 64;
    for (size_t i=0; encode[i]; ++i) decode[encode[i]&0xff] = i;
}

/** B64_decode
 *
 * \param src is a sequence of B64 encoded characters
 * \param tgt is a buffer into which src is decoded
 * \return nothing
 *
 * Note that computed GOTO is used liberally to eliminate conditionals.
 */
void B64_decode(const void* vsrc, void* vtgt) {
#define __NEXT(N) U = (size_t)(d=src[N]); goto *Z[N][(!d) || (d == '=')]
//#define INSTR(ARG) (printf(#ARG), printf(" %c", tgt[1]), printf(" %c\n", src[1]));
#define INSTR(ARG)
    static const void *Z[4][2] =
        { {&&B, &&F}, {&&C, &&F}, {&&D, &&F}, {&&E, &&F} };
    const u1_p src = (const u1_p)vsrc;
    u1_p tgt = (u1_p)vtgt;
    u1_p decode = B64_static.decode;
    u1_t d, m;
    size_t U;
A:  INSTR(A) __NEXT(0);
B:  INSTR(B) m = decode[U]; *tgt  = m << 2;                  __NEXT(1);
C:  INSTR(C) m = decode[U]; *tgt |= m >> 4; *++tgt = m << 4; __NEXT(2);
D:  INSTR(D) m = decode[U]; *tgt |= m >> 2; *++tgt = m << 6; __NEXT(3);
E:  INSTR(E) m = decode[U]; *tgt |= m     ; *++tgt = m     ;   goto A;
F:  INSTR(F) *tgt = 0;
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
void B64_encode(const void* vsrc, void* vtgt) {
    static const void *Z[4][2] =
        { {&&B, &&E0}, {&&C, &&E2}, {&&D, &&E1}, {&&A, &&E0} };
    const u1_p src = (const u1_p)vsrc;
    u1_p tgt = (u1_p)vtgt;
    u1_p encode = B64_static.encode;
    u1_t s, t = 0, i = 0;
A:  s = src[i++]; goto *Z[0][!s];
B:  *tgt++ = encode[    (s >> 2)]; t = (s & 0x3) << 4;
    *tgt = encode[t & 0xff]; s = src[i++]; goto *Z[1][!s];
C:  *tgt++ = encode[t | (s >> 4)]; t = (s & 0xf) << 2; *tgt = encode[t & 0xff];
    s = src[i++]; goto *Z[2][!s];
D:  *tgt++ = encode[t | (s >> 6)];
    *tgt++ = encode[(s & 0x3f)];
    goto *Z[3][!s];
E2: *++tgt = '=';
E1: *++tgt = '='; *++tgt = 0;
E0: *tgt = 0;
}
