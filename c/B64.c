/* B64.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */

#include "B64.h"

static const struct {
    const u1_p encode;       ///< the B64 encoding alphabet
    const u1_t decode[256];  ///< that alphabet turned backwards by B64_init.
} B64_static = {
    (u1_p)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=",
    {
       64, 64, 64, 64, 64, 64, 64, 64,  64, 64, 64, 64, 64, 64, 64, 64,
       64, 64, 64, 64, 64, 64, 64, 64,  64, 64, 64, 62, 64, 64, 64, 63,
       64, 64, 64, 64, 64, 64, 64, 64,  64, 64, 64, 64, 64, 64, 64, 64,
       52, 53, 54, 55, 56, 57, 58, 59,  60, 61, 64, 64, 64, 64, 64, 64,
       64,  0,  1,  2,  3,  4,  5,  6,   7,  8,  9, 10, 11, 12, 13, 14,
       15, 16, 17, 18, 19, 20, 21, 22,  23, 24, 25, 64, 64, 64, 64, 64,
       64, 26, 27, 28, 29, 30, 31, 32,  33, 34, 35, 36, 37, 38, 39, 40,
       41, 42, 43, 44, 45, 46, 47, 48,  49, 50, 51, 64, 64, 64, 64, 64,

       64, 64, 64, 64, 64, 64, 64, 64,  64, 64, 64, 64, 64, 64, 64, 64,
       64, 64, 64, 64, 64, 64, 64, 64,  64, 64, 64, 64, 64, 64, 64, 64,
       64, 64, 64, 64, 64, 64, 64, 64,  64, 64, 64, 64, 64, 64, 64, 64,
       64, 64, 64, 64, 64, 64, 64, 64,  64, 64, 64, 64, 64, 64, 64, 64,
       64, 64, 64, 64, 64, 64, 64, 64,  64, 64, 64, 64, 64, 64, 64, 64,
       64, 64, 64, 64, 64, 64, 64, 64,  64, 64, 64, 64, 64, 64, 64, 64,
       64, 64, 64, 64, 64, 64, 64, 64,  64, 64, 64, 64, 64, 64, 64, 64,
       64, 64, 64, 64, 64, 64, 64, 64,  64, 64, 64, 64, 64, 64, 64, 64
    }
};

/** B64_decode
 *
 * \param src is a sequence of B64 encoded characters
 * \param tgt is a buffer into which src is decoded
 * \return nothing
 *
 * Note that computed GOTO is used liberally to eliminate conditionals.
 */
void B64_decode(const void* vsrc, void* vtgt) {
// __NEXT macro performs the common code used by all steps.
#define __NEXT(N) U = (size_t)(d=src[O+N]); goto *Z[N][(!d) || (d == '=')]
// INSTR is instrumentation to aid in step-by-step review of operations
/* #define INSTR(L) (\
     printf(#L " tgt=%p src=%p U=%lu d=%u c=%c\n", tgt, src, U, d, (char)d));
 */
#define INSTR(ARG)
    static const void *Z[4][2] =
        { {&&B, &&F}, {&&C, &&F}, {&&D, &&F}, {&&E, &&F} };
    static const u1_t*const decode = B64_static.decode;
    const u1_p src = (const u1_p)vsrc;
    u1_p tgt = (u1_p)vtgt;
    u1_t d = 0, m;
    size_t U, O = 0;
    goto A;  ///< skip update on first step.
I:  INSTR(I) O += 4;
A:  INSTR(A)                                                 __NEXT(0);
B:  INSTR(B) m = decode[U];                   *tgt = m << 2; __NEXT(1);
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
// INSTR is instrumentation to aid in step-by-step review of operations
// #define INSTR(ARG) ( printf(#ARG " i=%lu\n", i) );
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
