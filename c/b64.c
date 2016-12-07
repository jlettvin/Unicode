/* B64.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#include <stdio.h>

#include "B64.h"

/*
 * Man
 *    M        a        n
 * 01001101 01100001 01101110
 * 010011 010110 000101 101110
 *    19     22      5     46
 * T=[19] W=[22] F=[ 5] u=[46]
 * TWFu
 */

typedef unsigned int uint_t;

void B64_decode(const char* src, char* tgt) {
#define __3 __, __, __
#define __6 __3, __3
#define __43 __6, __6, __6, __6, __6, __6, __6, __
#define __133 __43, __43, __43, __3, __
#define __NEXT(N) U = (uint_t)(d=*src++); goto *Z[N][(!d) || (d == '=')]
    static const char __ = 0;
    static const unsigned char xlat[256] = {
        __43, 62,
        __3 , 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
        __3 , 64,
        __3 , 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,
        12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
        __6 , 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
        38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
        __133
    };
    static const void *Z[4][2] = {
        {&&B, &&F}, {&&C, &&F}, {&&D, &&F}, {&&E, &&F}
    };
    char d, m;
    uint_t U;
A:  __NEXT(0);
B:  m = xlat[U]; *tgt  = m << 2;                  __NEXT(1);
C:  m = xlat[U]; *tgt |= m >> 4; *++tgt = m << 4; __NEXT(2);
D:  m = xlat[U]; *tgt |= m >> 2; *++tgt = m << 6; __NEXT(3);
E:  m = xlat[U]; *tgt |= m     ; *++tgt = m     ;   goto A;
F:  *tgt = 0;
#undef __3
#undef __6
#undef __43
#undef __133
#undef __NEXT
    return;
}

void B64_encode(const char* src, char *tgt) {
    static const char* c64 = ""
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "+/=";
    static const void *Z[4][2] = {
        {&&B, &&E0}, {&&C, &&E2}, {&&D, &&E1}, {&&A, &&E0}
    };
    char s, t;
A:  s = *src++; goto *Z[0][!s];
B:  *tgt++ = c64[    (s >> 2)]; t = (s & 0x3) << 4;
    *tgt = c64[t & 0xff]; s = *src++; goto *Z[1][!s];
C:  *tgt++ = c64[t | (s >> 4)]; t = (s & 0xf) << 2; *tgt = c64[t & 0xff];
    s = *src++; goto *Z[2][!s];
D:  *tgt++ = c64[t | (s >> 6)];
    *tgt++ = c64[(s & 0x3f)];
    goto *Z[3][!s];
E2: *++tgt = '=';
E1: *++tgt = '='; *++tgt = 0;
E0: *tgt = 0;
}
