/* B64.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */

// #include <stdio.h>

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
    for (size_t i=0; i < 256; ++i) decode[i] = 0;
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
#define __NEXT(N) U = (size_t)(d=src[O+N]); goto *Z[N][(!d) || (d == '=')]
/* #define INSTR(ARG) (\
     printf(#ARG \
     " tgt=%p src=%p U=%lu d=%u c=%c\n", tgt, src, U, d, (char)d) \
     );
 */
#define INSTR(ARG)
    static const void *Z[4][2] =
        { {&&B, &&F}, {&&C, &&F}, {&&D, &&F}, {&&E, &&F} };
    const u1_p src = (const u1_p)vsrc;
    u1_p tgt = (u1_p)vtgt;
    u1_p decode = B64_static.decode;
    u1_t d = 0, m;
    size_t U, O = 0;
    goto A;
I:  INSTR(I) O += 4;
A:  INSTR(A) __NEXT(0);
B:  INSTR(B) m = decode[U]; *tgt  = m << 2;                  __NEXT(1);
C:  INSTR(C) m = decode[U]; *tgt |= m >> 4; *++tgt = m << 4; __NEXT(2);
D:  INSTR(D) m = decode[U]; *tgt |= m >> 2; *++tgt = m << 6; __NEXT(3);
E:  INSTR(E) m = decode[U]; *tgt |= m     ; *++tgt = m     ;   goto I;
F:  INSTR(F) *tgt = 0;
#undef INSTR
#undef __NEXT
    // printf("\t\t!! %s\n", (char *)vtgt);
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
/* #define INSTR(ARG) (\
        printf(#ARG " i=%lu\n", i) \
        );
 */
#define INSTR(ARG)
    static const void *Z[4][2] =
        { {&&B, &&E0}, {&&C, &&E2}, {&&D, &&E1}, {&&A, &&E0} };
    const u1_p src = (const u1_p)vsrc;
    size_t i = 0;
    u1_p tgt = (u1_p)vtgt;
    u1_p encode = B64_static.encode;
    u1_t s, t = 0;
A:  INSTR(A) s = src[i++]; goto *Z[0][!s];
B:  INSTR(B) *tgt++ = encode[    (s >> 2)]; t = (s & 0x3) << 4;
    *tgt = encode[t & 0xff]; s = src[i++]; goto *Z[1][!s];
C:  INSTR(C) *tgt++ = encode[t | (s >> 4)]; t = (s & 0xf) << 2;
    *tgt = encode[t & 0xff]; s = src[i++]; goto *Z[2][!s];
D:  INSTR(D) *tgt++ = encode[t | (s >> 6)];
    *tgt++ = encode[(s & 0x3f)];
    goto *Z[3][!s];
E2: INSTR(E2) *++tgt = '=';
E1: INSTR(E1) *++tgt = '='; *++tgt = 0;
E0: INSTR(E0) *tgt = 0;
#undef INSTR
}
