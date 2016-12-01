/* Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
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
 */

#include <iostream>
#include <bitset>
#include <vector>
#include <map>

namespace jlettvin {
    using std::vector;
    using std::map;
    using std::cout;
    using std::endl;
    using std::hex;
    using std::setw;
    using std::setfill;
    using std::ostream;
    using std::bitset;

    typedef unsigned char ubyte_t;                   ///< buffer element type

    typedef size_t codepoint_t;                   ///< clear type of codepoint
    typedef size_t target_t;                      ///< clear type of result
    typedef size_t either_t;                      ///< ambiguous type

    typedef vector<either_t> subtable_t;          ///< type of table entry
    typedef vector< subtable_t > T_type;          ///< type of table
    typedef map< codepoint_t, target_t > M_type;  ///< type of dict

    typedef T_type::const_iterator T_iter;        ///< table iterator
    typedef M_type::const_iterator M_iter;        ///< dict iterator

    typedef union { char32_t wide; ubyte_t thin; } u_32_and_8_t;

    static const codepoint_t sentinel = 0x10FFFF; ///< highest legal Unicode

    static const size_t eABCD = 0x03020100;       ///< endian index setter
    /**
     * eA, eB, eC, eD are useful for indexing bytes from 32 bit words.
     * They are guarateed to index the byte containing the same value
     * regardless of machine endianness because the endian index setter
     * has the index values jammed together such that they split out
     * into the correct indices.
     */
    static const size_t eA = (eABCD>>0x18) & 0xff;
    static const size_t eB = (eABCD>>0x10) & 0xff;
    static const size_t eC = (eABCD>>0x08) & 0xff;
    static const size_t eD = (eABCD>>0x00) & 0xff;
    /**
     * ABCD[0] indexes a byte with the same value invariant to endianness.
     */
    static const size_t ABCD[4] = { eA, eB, eC, eD };
    //template<typename L, typename B>
    //B& byteAt(L& wide, const size_t index) {
        //u_32_and_8_t *u = &wide;
        //return u.thin[ABCD[index]];
    //}

    /** \brief enable use of cout << T << std::endl;
     */
    template<typename T1>
    ostream& operator <<(ostream& o, const vector<T1>& v) {
        o << setw(1) << setfill(' ') << "[";
        if (!v.empty()) {
            typename vector<T1>::const_iterator iter;
            for (iter = v.begin(); iter != --v.end(); ++iter)
                o << hex << setw(6) << setfill(' ') << *iter << ' ';
            o << hex << setw(6) << setfill(' ') << *--v.end();
        }
        o << setw(1) << setfill(' ') << "]" << endl;
        return o;
    }


    /** char32_t_to_UTF8 converts internal 32 bit codepoints to UTF8.
     *
     * \param source is a 32 bit codepoint
     * \param target is a 5 unsigned byte buffer from the caller.
     *
     * The codepoint is converted to UTF8 with trailing NUL bytes
     * such that it is immediately ready for output via std::cout.
     *
     * Branch points are avoided.
     * Maximum cost is 23 Intel opcodes.
     */
    inline void char32_t_to_UTF8(const char32_t source, ubyte_t target[5]) {
        target[4] = target[3] = target[2] = target[1] = target[0] = 0;
        static const void *assemble[] = {&&u7, &&u11, &&u16, &&u21};
        goto *assemble[
            static_cast<size_t>(source > 0x7f) +
            static_cast<size_t>(source > 0x7ff) +
            static_cast<size_t>(source > 0xffff)];
u7:     target[0] = static_cast<ubyte_t>(source);
        return;
u11:    target[0] = 0xC0 + static_cast<ubyte_t>((source >> 0x06) & 0x1f);
        target[1] = 0x80 + static_cast<ubyte_t>((source        ) & 0x3f);
        return;
u16:    target[0] = 0xE0 + static_cast<ubyte_t>((source >> 0x0c) & 0x0f);
        target[1] = 0x80 + static_cast<ubyte_t>((source >> 0x06) & 0x3f);
        target[2] = 0x80 + static_cast<ubyte_t>((source        ) & 0x3f);
        return;
u21:    target[0] = 0xF0 + static_cast<ubyte_t>((source >> 0x12) & 0x07);
        target[1] = 0x80 + static_cast<ubyte_t>((source >> 0x0c) & 0x3f);
        target[2] = 0x80 + static_cast<ubyte_t>((source >> 0x06) & 0x3f);
        target[3] = 0x80 + static_cast<ubyte_t>((source        ) & 0x3f);
        return;

    }

    // UTF8 Bytes being translated have this pattern with bit indices
    // shown as uppercase letters from the high bit to the low bit:
    // 1  7	U+00000	U+00007F    0ABCDEFG
    // 2 11	U+00080	U+0007FF    110ABCDE  10FGHIJK
    // 3 16	U+00800	U+00FFFF    1110ABCD  10EFGHIJ  10KLMNOP
    // 4 21	U+10000	U+10FFFF    11110ABC  10DEFGHI  10JKLMNO  10PQRSTU

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
     *
     * TODO fix so it works
     */
    inline char32_t
    UTF8_to_char32_t(const ubyte_t *buf, size_t& head, const size_t tail) {
        // hello="hello"; hello[head] == 'h'; hello[tail] == '\0';
        static const size_t need[] = {
            // The number of bytes needed for an ingest act is specified here.
            // This table is indexed using the top 5 bits of the first byte
            0, 4, 3, 3, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
        };
        static const void *alabel[] = {
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
        static const void *dblabel[] = { &&err, &&err, &&db, &&err };
        static const void *dclabel[] = { &&err, &&err, &&dc, &&err };
        static const void *ddlabel[] = { &&err, &&err, &&dd, &&err };
        static const void *cblabel[] = { &&err, &&err, &&cb, &&err };
        static const void *cclabel[] = { &&err, &&err, &&cc, &&err };
        static const void *bblabel[] = { &&err, &&err, &&bb, &&err };
        // These label pairs change the code path depending on whether
        // the incoming buffer has bytes left to ingest and
        // the head/tail args are sufficient to support ingest needs.
        static const void *headed[] = { &&err, &&ingest };
        static const void *tailed[] = { &&err, &&enough };

        size_t off = head, high;
        char32_t _ = 0;
        ubyte_t a, b, c, d;

        goto *headed[static_cast<size_t>(head < tail)];
ingest: a = buf[off++];
        high = static_cast<size_t>((a >> 3) & 0x1f);
        goto *tailed[static_cast<size_t>(need[high] <= (tail - head))];
enough: goto *alabel[high];
a1e:    b = buf[off++]; goto *dblabel[static_cast<char32_t>((b >> 6) & 0x3)];
db:     c = buf[off++]; goto *dclabel[static_cast<char32_t>((c >> 6) & 0x3)];
dc:     d = buf[off++]; goto *ddlabel[static_cast<char32_t>((d >> 6) & 0x3)];
dd:     _ = (static_cast<char32_t>(a & 0x07) << 0x12) +
            (static_cast<char32_t>(b & 0x3f) << 0x0c) +
            (static_cast<char32_t>(c & 0x3f) << 0x06) +
            (static_cast<char32_t>(d & 0x3f));
        head = off; return _;
a1d:    b = buf[off++]; goto *cblabel[static_cast<char32_t>((b >> 6) & 0x3)];
cb:     c = buf[off++]; goto *cclabel[static_cast<char32_t>((c >> 6) & 0x3)];
cc:     _ = (static_cast<char32_t>(a & 0x0f) << 0x0c) +
            (static_cast<char32_t>(b & 0x3f) << 0x06) +
            (static_cast<char32_t>(c & 0x3f));
        head = off; return _;
a1c:    b = buf[off++]; goto *bblabel[static_cast<char32_t>((b >> 6) & 0x3)];
bb:     _ = (static_cast<char32_t>(a & 0x1f) << 0x06) +
            (static_cast<char32_t>(b & 0x3f));
        head = off; return _;
a00:    _ = static_cast<char32_t>(a);
        head = off; return _;
err:    return static_cast<char32_t>(_);
    }
}
