/* Unicode.h Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#ifndef C_UNICODE_H_
#define C_UNICODE_H_

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

#define sentinel 0x10FFFF

/*
typedef unsigned char       u1_t;
typedef          char       s1_t;
typedef unsigned short      u2_t;
typedef          short      s2_t;
typedef unsigned long       u4_t;
typedef          long       s4_t;
typedef unsigned long long  u8_t;
typedef          long long  s8_t;
*/

// These types depend on the shift value of the sizeof(type)
// 0 => 1<<0 => 1
// 1 => 1<<1 => 2
// 2 => 1<<2 => 4
// 3 => 1<<3 => 8
typedef unsigned char       ue0_t;
typedef          char       se0_t;
typedef unsigned short      ue1_t;
typedef          short      se1_t;
typedef unsigned long       ue2_t;
typedef          long       se2_t;
typedef unsigned long long  ue3_t;
typedef          long long  se3_t;

typedef unsigned char ubyte_t;                 ///< imitate c++ type
typedef unsigned long size_t;                  ///< imitate c++ type
typedef size_t char32_t;                       ///< imitate c++ type
typedef char32_t* u32string;                   ///< imitate c++ type

typedef size_t codepoint_t;                    ///< clear type of codepoint
typedef size_t target_t;                       ///< clear type of result
typedef size_t either_t;                       ///< ambiguous type

/**
 * A type to support linkage.
 */
typedef struct {
    codepoint_t     codepoint;  ///< the value
    target_t        table;      ///< the linkage
} entry_t;
typedef entry_t* M_type;                       ///< type of dict


#endif  // C_UNICODE_H_
