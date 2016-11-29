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
 * \ version 0.0.1
 *
 * \date 2016/11/29 09:47
 *
 * \license GPLv3
 *
 * \copyright Copyright(C) 2016 Jonathan D. Lettvin, All Rights Reserved"
 *
 */

#include <vector>
#include <map>

namespace jlettvin {
    using std::vector;
    using std::map;

    typedef size_t codepoint_t;
    typedef size_t target_t;
    typedef size_t either_t;

    typedef vector<either_t> entry_t;              ///< type of table entry
    typedef vector< entry_t > T_type;              ///< type of table
    typedef map< codepoint_t, target_t > M_type;   ///< type of dict

    typedef T_type::const_iterator T_iter;         ///< table iterator
    typedef M_type::const_iterator M_iter;         ///< dict iterator

    static const codepoint_t sentinel = 0x10FFFF;
}
