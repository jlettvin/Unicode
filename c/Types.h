/* Unicode.h Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#ifndef C_TYPES_H_
#define C_TYPES_H_

/**
 * \class Omni
 *
 * \ingroup jlettvin
 *
 * \brief Common header
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

// The non-e types have sizeof(type) in their typedef.
// u1_t => 1 BYTES
// u2_t => 2 BYTES
// u4_t => 4 BYTES
// u8_t => 8 BYTES
// The e types depend on the shift value of the sizeof(type)
// PRE    SHFT    BYTES
// ue0 => 1<<0 => 1
// ue1 => 1<<1 => 2
// ue2 => 1<<2 => 4
// ue3 => 1<<3 => 8
typedef unsigned char       ue0_t, *ue0_p, u1_t, *u1_p;
typedef          char       se0_t, *se0_p, s1_t, *s1_p;
typedef unsigned short      ue1_t, *ue1_p, u2_t, *u2_p;
typedef          short      se1_t, *se1_p, s2_t, *s2_p;
typedef unsigned long       ue2_t, *ue2_p, u4_t, *u4_p;
typedef          long       se2_t, *se2_p, s4_t, *s4_p;
typedef unsigned long long  ue3_t, *ue3_p, u8_t, *u8_p;
typedef          long long  se3_t, *se3_p, s8_t, *s8_p;

typedef ue0_t ubyte_t, *ubyte_p;               ///< imitate c++ type
typedef ue2_t size_t, *size_p;                 ///< imitate c++ type
typedef size_t char32_t;                       ///< imitate c++ type
typedef char32_t* u32string;                   ///< imitate c++ type

typedef size_t codepoint_t;                    ///< clear type of codepoint
typedef size_t address_t;                      ///< clear type of address
typedef size_t target_t;                       ///< clear type of result
typedef size_t either_t;                       ///< ambiguous type

#endif  // C_TYPES_H_
