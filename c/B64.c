/* B64.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
/* en.wikipedia.org/wiki/Base64 */

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

static struct {
    char* talx;
    char xlat[256];
} B64_static = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=",
    {}
};

__attribute__((constructor))
void B64_init(void) {
    // puts("B64_init");
    char* talx = B64_static.talx;
    char* xlat = B64_static.xlat;
    for (int i=0; i < 256; ++i) xlat[i] = 64;
    for (int i=0; talx[i]; ++i) xlat[talx[i]&0xff] = i;
}

void B64_decode(const char* src, char* tgt) {
#define __NEXT(N) U = (uint_t)(d=*src++); goto *Z[N][(!d) || (d == '=')]
    static const void *Z[4][2] =
        { {&&B, &&F}, {&&C, &&F}, {&&D, &&F}, {&&E, &&F} };
    char* xlat = B64_static.xlat;
    char d, m;
    uint_t U;
A:  __NEXT(0);
B:  m = xlat[U]; *tgt  = m << 2;                  __NEXT(1);
C:  m = xlat[U]; *tgt |= m >> 4; *++tgt = m << 4; __NEXT(2);
D:  m = xlat[U]; *tgt |= m >> 2; *++tgt = m << 6; __NEXT(3);
E:  m = xlat[U]; *tgt |= m     ; *++tgt = m     ;   goto A;
F:  *tgt = 0;
#undef __NEXT
    return;
}

void B64_encode(const char* src, char *tgt) {
    static const void *Z[4][2] =
        { {&&B, &&E0}, {&&C, &&E2}, {&&D, &&E1}, {&&A, &&E0} };
    char* talx = B64_static.talx;
    char s, t;
A:  s = *src++; goto *Z[0][!s];
B:  *tgt++ = talx[    (s >> 2)]; t = (s & 0x3) << 4;
    *tgt = talx[t & 0xff]; s = *src++; goto *Z[1][!s];
C:  *tgt++ = talx[t | (s >> 4)]; t = (s & 0xf) << 2; *tgt = talx[t & 0xff];
    s = *src++; goto *Z[2][!s];
D:  *tgt++ = talx[t | (s >> 6)];
    *tgt++ = talx[(s & 0x3f)];
    goto *Z[3][!s];
E2: *++tgt = '=';
E1: *++tgt = '='; *++tgt = 0;
E0: *tgt = 0;
}
