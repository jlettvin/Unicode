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

/*
static const char* label[] = {"__",
    "Cc", "Cf", "Cn", "Co", "Cs", "Ll", "Lm", "Lo", "Lt", "Lu",
    "Mc", "Me", "Mn", "Nd", "Nl", "No", "Pc", "Pd", "Pe", "Pf",
    "Pi", "Po", "Ps", "Sc", "Sk", "Sm", "So", "Zl", "Zp", "Zs"};

// Unicode character classification
static const char *compressed_classifiers =
    "QlpoOTFBWSZTWZ1WjuQAIZt///////////7vsvY7F9HcwJeIlAQgUBAJAnAoMVQgFDTYYA3S"
    "L492ztgazDVVtTFqw2QUk0oF2zu7k7FtdtMsOICTJqekm2hNMmhTyTR6ageo9QHqGQAaABoA"
    "AAAGgAGg00IAmiT0jTTQRqj2qbU8k2ieUyZAHqBoaHqAaGhoADIZA0D1EGmJgAAAAAAAAAAA"
    "AAAjCMAAAADNIkI0RNMJ6EUeU8j1Ro2oxNMhkAaAaAAAAADQDQaBBpiYAAAAAAAAAAAAAAIw"
    "jAAAAAiUmiaNFT/KMptTRkjQaGgADQ0AAAAANAAAaADTQ4Y3eb28p6CD7V7vB99oC1aLKS5Y"
    "8LZtTiReO95YUeiqjZOGf2uG7ThQalsY6reLZvnXX8GzKkWJhZQwIYiaqg5L8U6mFlRtGae5"
    "wMl3Wq3HDWSIkjkSxBcKRrVX5oMtNavzEhbBh4lZ64721/eozPcsOMBx0Wo32zgIAXiNCcow"
    "Hq93o3/lkXC0K5IN9+NVOHRyQjUQ5GwoIa5uJSh+ZCGMhC6Ub6MakDteC9sY+ptJsGuXN6z/"
    "Sblzid8xgsR6f7lFTIjPEuD3zCoEHqmxQN31aQ4IqOEO61URcIA8aLzYgXxFxggHPioaIiBy"
    "oChwQAXcighxYgB0IAB9sFc0FAUTdgAjcbVMrHOID5xNrMYJHMTjinTI2d/hSoDsnoepe3vR"
    "JeFAhTcAiMBB5m0nk6paCAA8huSNyqCVxiBlpEc2gggCor9UBXbBNmXt47JarbBPJ1cTKkrQ"
    "kLqPqBVAdEzkXWS0iCEJrORDQgS2C+jnF7k/J68XeElmD2qKfFiSrWcQohbRpbheSkJCCphW"
    "aXMsPua1eXrkTotj1GIIkqDzz8v3VHv6qBIoSZoZRN2BZfR3JqgehAFUVugBUgA+oW80EvEk"
    "bp9Kt2FspoYlyCsW3vcfZbc8MSzGtYn2UoCBLYW6S7N0+ZiNAd8hkSPKqW0V1NntFDWX6v25"
    "4DzccRrrMLZIsEqTpINY6ohIUb+bhOl8+ZcxLSW0oV+zmvPne3CzEIXjxKQ+3ly1ZoQSyFwH"
    "VxRGU3U30sXeym20uus6gxYFgeZxCW2o0jVEUNCio4gO3G4WgGJwz6oxSL9EmkhgRmq1MyT0"
    "0UCZUgt3IuATUo1xDa4hXrGd0UQhhZ5HR4M7S2Dz7O1QchjZxUt8NkMTVdzr7TP4tDsdGmxt"
    "rQpDiW0MZ7MXlRFBFbTVbnhNlSgStEyNFTey2hlhmwomMAuvwwVDaCgoZKCCKQUkTnTXQzDv"
    "HPIVsZLlYKZhQjAvdlUGl5ogbyZdzNYaHQp0kIxqb3ydLkB8iAoUh4IIWckkYKRL3HPNnSFd"
    "LnDLJhF2lcGeTToiCLOLhkQCDgLDYW97yhB0OuQ054eDRdPZdAnsbW+A0ll7E5vMjevpxJrn"
    "49/dj5QxLOgqdXC8ljhSBQWWWSgzhGEIkUKABaQhKbDhIKiVtTyyur94oQqW1i8jRpRaNGze"
    "uFaoVixCOhVpdLvceDGIpnIvFM39rx53DolAq9dqVe6dDQlDvIaqkCggRB0bQk8BnU26ZKqd"
    "xUk0lfRG2zrxlJiInEg7zYdYAwxN3hEhR3HXWaCIYkoJCq81enklA+M8WeDwZbpIRIuiKtIR"
    "hCQIQtkZvWCkG2LfIubFSZGVOCEK8KRdFKDdjTTAFb7KKSIMiAhIABIgEiASNpgmySDkIkoq"
    "igshxSKJCFlZV6LhlksZajoe/Ceddym1Gu2KIFL04YimuzOoFaEIh+K+cLKmrKJmebsALlEy"
    "a/wOTb0wTw5eLisifGuqceNFbeU18B4VwIGfrqH8K7o8CkDKi5GV3yZyDr0tAyXe79FmsOJq"
    "pauEJazCoFC0kf/RjCDEiZRiiQIQIRRGwWRBIEJAxAADGqoUWRzQG+IjIqWelROnzUSsmmc6"
    "NiBEgB/VIIECQCYxraJJteiKhg0CSOUaQiSAKRDTA1Q/XRbL7pbrlOXKdnFia48yIdPGUidf"
    "Sz7vJlFB7JCcAUSnOqkJC7t1FMU1ZNGZpOrECJTZtfs2wQUImYkFas+C1PywGn5JXdBEVwyH"
    "svZ/gkMfoGBhtXlWn85i7+k2TbsR1ddsrh47/1ZRiIGxnqzJ2uZcjdcf2wo2g4jIMLz2xn7u"
    "kp5r6DV95V8uT+fLzc94PNi84wcuILmFBc3sSC5B+uOtJ0Dfqi/ydHVROPF++PRmiKgedFXG"
    "J1ESQAXfiInJ6VS2DlJw2VkqtIgG/FTbEBF4YJ3ZFRmCEkVEvSGm6ElkNttNiHDfMD3XGNLy"
    "5Fs8ExpG/u5aU+rdE1YlrplTaoSe4djWeyTO6yEmDdDXl6ONtJN5Yhlpdv0Xl5SW27T7r1VZ"
    "Pxi0KNNO1KEsDcQQiazE0Y9HkmY8h6ZxiGgD30cTdDUEvXNBJUMEkRKQpAUMoAhjAA6SIVgq"
    "G8KCSevZGkvME21Y0CyILiFOSJQRxkBhx7KrfiMoQ2uRCTVAIaEBwErpJ3BCwcYVYBspSacK"
    "BIUxhSt+oLKdrn/uwD9ORmzae22fXpuyAGkE3yXl8KAGuNhdWhYJldwXRmKp9Q1KImVOORUJ"
    "2xpx6EF3ZQui+WoWzuUlCooNvonob7pGCRRt60rFY9RVd19b1HnASI/g/kPzD1RVf5HdQUJi"
    "QpiR2nfCq6tyKC3JhYHgPWkGDPWyoVKPPoi4DtXNfSkKJwiwhV2o1QgQY06Kd7SIEvCOgrrp"
    "VXv+yVe4QDIy3BAbDr2KX6qyftrFcjU2qFFYrKawDDJYFKhQ3ZqILGcfTSdoZcbgnvM0JpJb"
    "hLz8TI6ayu6FFncpknGTqjiyHrXeLQn0gNxAjPTZpuMTKISA7pB8tSlsIvT69KXqGFmQQ71j"
    "o9wWq6CvKtVSQQyTHxjToabZd9HHjr54si5G5eapeI56iyXfkQBR+uVQ5gjBeESd7rpJIEk1"
    "sHleMW9R1RcgU5Epx+dvj2fKkmZHUfVsMMKHzKQY1bvu5D9uzuT+aCkjrmqDqMVSaaKsz23+"
    "ipzKF00yDicY5510toYBiib+SGOXAxWo8xAdO1AreBAyOYyE1XAxdt9sXjG6GK/bVvAUyiGH"
    "fUYQIEe7BggQJbPJUXuZzfWK1uJtAizbul65ZkMNMV851tJpg/VorApsGhzgNCPUCcUgoQa0"
    "0yn4a9f8Zh0RwDPEWTgmEASQSkFDXDDhtLbPSmAlEymaZsE16Najt0Gjzdp1znlgYhd+xg3i"
    "guw84haJdJDti2DeY9woNb3fC9pwnOLVbdFY1pl3K0J8SuInlwya7oT1N9KjyAo+bVLoUCf5"
    "IQIGEussgpFRMqp8CAYVJpyZqkJCEUFUHHaQSELK2w68MMrkoxzyDSs7gk8YjSW4GkJf3188"
    "pbBA3s056VC/wYFkD2ZRunvNd/ijp98ZryGLpHKhdO2jyYGfHVoaiNYXwTj40Ssxu3tdf63L"
    "XPUMiFXpoHJ6zsmoGpA2FB1FAib8LVZMg/wafE4+eOej6Z60aOhzgqVbPMVW+26z5Y/I6dKa"
    "fDvx6XvsOlOAnSE6cztyIN+xQQRS8GTnG6KNYpQ4MgUciZfc94JHNaTyEHDPUsrCUM1gFsNG"
    "5RP9mXQols5cX2Ib040ORDHVcTWw7U6FzXyqokXUq4C4YmILyWoPkXR8DxX1ohLTGYZIlByx"
    "7GwU1KaSdgPO8sbd2u7HRqDukAo2F134xrgNY5LM87uoKadimtKNGXDyLaS742sBU4q2bo2u"
    "grg8nM30UCCQEaiAUovoIoBNMwL/fw6VwX/MYiG87gvGtn63p9OUZdM0DklAiOUkQloPo+XL"
    "1sOUgVRvinTT0FauLnFtqLujy8c5dk1HBKYi3jhEDrh2GhCJMoEsczMgDYdDkQmdMmTdfYwm"
    "ysuTJcjsKZIQBoQiJAok7KBggDDXr5sGqqz0VXV1gTy4+E5ygx2UZnGGJ5qbEkv9MeFR9PTT"
    "mOsvnCRyBBSSWiYjTKaRkOkQQgKMscspdVYR0bDWLzotWGZYXbM4gIgANLtLLcefV4tfSkPQ"
    "M2nOvos1YpJ5TfomFjHxKKljWqhclPJyyLvFjgWVUdoEOpKRRPDFM+q8s880qUjWYqFn7vGV"
    "glge3KKjNRVSodE5BbBa45MorGSC2jUFVVEbbpD9naUqRPWq1Jbmo9lRIAdiiERRQeJTpNpp"
    "Jt4TdgiKWlR/8XckU4UJCdVo7kA=";
    */

char classifiers[0x110000];  // sentinel+1

/*
void init_labels() {
    unsigned int count = 0;
    unsigned int length = strlen(compressed_classifiers);
    char *temp = malloc(length);  // Actual contents shrink to 3/4ths.
    B64_decode(compressed_classifiers, temp);
    BZ2_bzBuffToBuffDecompress(
            classifiers,
            &count,
            temp,
            length,
            0,
            0);
    snprintf(temp, length, "%u == %zu ?  decompressed correctly if true",
            count, sentinel+1);
    passfail(count == sentinel+1, temp);
    free(temp);
}
*/
