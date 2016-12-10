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
    codepoint_t codepoint;  ///< the value
    target_t table;         ///< the linkage
} entry_t;
typedef entry_t* M_type;                       ///< type of dict


#endif  // C_UNICODE_H_
