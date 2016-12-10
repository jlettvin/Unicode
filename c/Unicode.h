/* Unicode.h Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#ifndef C_UNICODE_H_
#define C_UNICODE_H_

/**
 * \class Omni
 *
 * \ingroup jlettvin
 *
 * \brief Shared resources.
 *
 * (i.e. jlettvin::sentinel = 0x10FFFF; )
 *
 * \author Jonathan D. Lettvin
 * jlettvin@gmail.com
 *
 * \version 0.0.1
 *
 * \date 2016/11/29 09:47
 *
 * license GPLv3
 *
 * \copyright Copyright(C) 2016 Jonathan D. Lettvin, All Rights Reserved"
 *
 * BACKGROUND:
 *  http://lemire.me/blog/2012/06/20/do-not-waste-time-with-stl-vectors/
 */

typedef unsigned char ubyte_t;                 ///< imitate c++ type
typedef unsigned long size_t;                  ///< imitate c++ type
typedef size_t char32_t;                       ///< imitate c++ type
typedef char32_t* u32string;                   ///< imitate c++ type

typedef size_t codepoint_t;                    ///< clear type of codepoint
typedef size_t target_t;                       ///< clear type of result
typedef size_t either_t;                       ///< ambiguous type

// typedef size_t[8] subtable_t;                  ///< type of table entry
// typedef subtable_t* T_type;                    ///< type of table

/**
 * A type to support linkage.
 */
typedef struct {
    codepoint_t codepoint;  ///< the value
    target_t table;         ///< the linkage
} entry_t;
typedef entry_t* M_type;                       ///< type of dict

static const codepoint_t sentinel = 0x10FFFF;  ///< highest legal Unicode

/** char32_t_to_UTF8 converts internal 32 bit codepoints to UTF8.
 *
 * \param source is a 32 bit codepoint
 * \param target is a 5 unsigned byte buffer from the caller.
 * return nothing
 *
 * The codepoint is converted to UTF8 with trailing NUL bytes
 * such that it is immediately ready for output via std::cout.
 *
 * Branch points are avoided.
 * Maximum cost is 23 Intel opcodes.
 */
void char32_t_to_UTF8(char32_t source, char* target);

// UTF8 Bytes being translated have this pattern with bit indices
// shown as uppercase letters from the high bit to the low bit:
// 1  7 U+00000 U+00007F    0ABCDEFG
// 2 11 U+00080 U+0007FF    110ABCDE  10FGHIJK
// 3 16 U+00800 U+00FFFF    1110ABCD  10EFGHIJ  10KLMNOP
// 4 21 U+10000 U+10FFFF    11110ABC  10DEFGHI  10JKLMNO  10PQRSTU

/** UTF8_to_32 converts UTF8 to internal 32 bit codepoint value.
 *
 * \param buf  is a buffer full of UTF8 unsigned bytes.
 * \param head is the index where ingest begins or continues (variable).
 * \param tail is the index past the last byte in the buffer.
 *
 * This function is designed to make conversion efficient.
 * It uses the gnu c++ compiler support for computed goto
 * to eliminate the need for branch points.
 *
 * Review of a least and most costly execution in opcode counts.
 * No stack frame is generated or discarded (inline).
 * The caller provides room in its stack frame for this function.
 * Least:
 * 2 opcodes: compare head to tail, goto err;
 * 2 opcodes: cast byte 0 to 32 bit return (essentially a goto)
 * Most:
 * 2 opcodes: compare head to tail goto ingest
 * 6 opcodes: dereference byte, increment, shift, mask, cast, goto enough
 * 6 opcodes: dereference byte, increment, shift, mask, cast, goto a1e
 * 6 opcodes: dereference byte, increment, shift, mask, cast, goto db
 * 6 opcodes: dereference byte, increment, shift, mask, cast, goto dc
 * 6 opcodes: dereference byte, increment, shift, mask, cast, goto dd
 * 2 opcodes: update head, return
 * 34 total opcodes 
 */
codepoint_t UTF8_to_char32_t(char* buf, size_t* head, size_t tail);

/** UTF8_to_u32string
 *
 * \param source a reference to a UTF8 encoded string of 8 bit elements.
 * \param target a reference to a char32_t encoded elements.
 */

// Must develop appending of char32_t to target.
/*
void UTF8_to_u32string(const char* source, u32string target) {
    size_t head = 0, tail = strlen(source);
    while (head < tail) {
        char32_t c = UTF8_to_char32_t(source, head, tail);
        if (!c) break;
        target += c;
    }
    target += char32_t(0);
}
*/

/** u32string_to_UTF8
 *
 * \param source a reference to a char32_t encoded elements.
 * \param target a reference to a UTF8 encoded string of 8 bit elements.
 *
 * Enhancement potential if pre-allocated and translated in-place.
 */
/*
inline void
u32string_to_UTF8(const u32string& source, string& target) {
    char buf[5];
    u32string::const_iterator iter;
    for (iter = source.begin(); iter != source.end(); ++iter) {
        char32_t_to_UTF8(*iter, buf);
        target += buf;
    }
}
*/
#endif  // C_UNICODE_H_
