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
 * size_t funny = 0x76543210;
 * cout << hex << *endless(&funny, 0); // outputs 10 on all machines.
 * cout << hex << *endless(&funny, 3); // outputs 76 on all machines.
 * *endless(&funny, 3) = 0x98;         // always replaces 76 with 98.
 */
unsigned char* Endian_invariant(void* p32, unsigned long off);
#endif  // C_ENDIAN_H_
