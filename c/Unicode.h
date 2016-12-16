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

#include "Types.h"

#define sentinel 0x10FFFF
/**
 * A type to support linkage.
 */
typedef struct {
    codepoint_t     codepoint;  ///< the value
    target_t        table;      ///< the linkage
} entry_t;
typedef entry_t* M_type;                       ///< type of dict


#endif  // C_UNICODE_H_
