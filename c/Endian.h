/* Endian.h Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#ifndef C_ENDIAN_H_
#define C_ENDIAN_H_
/** Endian.h and Endian.c
 *
 * \class Omni
 *
 * \ingroup jlettvin
 *
 * \brief codepoint classifier (lexer generics)
 *
 * \author Jonathan D. Lettvin
 * jlettvin@gmail.com
 *
 * \version 0.0.1
 *
 * \date 2016/12/10 09:47
 *
 * license GPLv3
 *
 * \copyright Copyright(C) 2016 Jonathan D. Lettvin, All Rights Reserved"
 */

/** Endian_invariant
 *
 * \param p32 a pointer to a 32 bit wide datum from which to extract a byte.
 * \param off an index (between 0 and 3) byte position within *p32.
 * \return pointer to the specified byte.
 *
 * endless(a_pointer_to_32_bits, an_index_between_0_and_3)
 * returns a reference to the same-valued byte invariant of endianness.
 *
 * These indices address the byte containing the same value in 32 bits
 * regardless of machine endianness because the endian index setter
 * has the index values jammed together such that they split out
 * into the correct indices.
 * The extra cost is one instruction for one extra indirection.
 *
 * size_t sample = 0x76543210;
 * cout << hex << *Endian_invariant(&sample, 0); // outputs 10 on all machines.
 * cout << hex << *Endian_invariant(&sample, 3); // outputs 76 on all machines.
 * *endless(&funny, 3) = 0x98;         // always replaces 76 with 98.
 */

#include "Unicode.h"

#define ENDIAN_INDICES 0x03020100

typedef union {
    ue2_t u32;      ///< data as a 32 bit representation
    ue0_t u8[4];    ///< data as 4  8 bit representations
} Endian_invariant32_t, *Endian_invariant32_p;

unsigned char* Endian_invariant(void* p32, size_t off);

#endif  // C_ENDIAN_H_
