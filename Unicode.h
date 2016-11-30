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

#include <vector>
#include <map>

namespace jlettvin {
    using std::vector;
    using std::map;
    using std::endl;
    using std::hex;
    using std::setw;
    using std::setfill;
    using std::ostream;

    typedef size_t codepoint_t;                    ///< clear type of codepoint
    typedef size_t target_t;                       ///< clear type of result
    typedef size_t either_t;                       ///< ambiguous type

    typedef vector<either_t> subtable_t;          ///< type of table entry
    typedef vector< subtable_t > T_type;          ///< type of table
    typedef map< codepoint_t, target_t > M_type;  ///< type of dict

    typedef T_type::const_iterator T_iter;        ///< table iterator
    typedef M_type::const_iterator M_iter;        ///< dict iterator

    static const codepoint_t sentinel = 0x10FFFF; ///< highest legal Unicode

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

}
