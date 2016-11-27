/* Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
/**
 * \class Node
 *
 * \ingroup jlettvin
 *
 * \brief Create an O(1) association between codepoint and indices.
 *
 * Node supports building word trees for fast lookup and canonicalization.
 *
 * \author Jonathan D. Lettvin
 * jlettvin@gmail.com
 *
 * \ version 0.0.1
 *
 * \date 2016/11/27 08:14
 *
 * \license GPLv3
 *
 * \copyright Copyright(C) 2016 Jonathan D. Lettvin, All Rights Reserved"
 *
 */
#ifndef   NODE_H_
#define   NODE_H_

#include <exception>
#include <vector>
#include <map>

using std::exception;
using std::vector;
using std::map;

namespace       jlettvin {
class Node {
 private:
        typedef vector< vector< size_t > > T_type;  ///< type of table
        typedef map< size_t, size_t > M_type;       ///< type of dict

        typedef T_type::const_iterator T_iter;      ///< table iterator
        typedef M_type::const_iterator M_iter;      ///< dict iterator

        static const size_t count;                  ///< legal codepoints

        T_type T;   ///< O(1) lookup table for codepoint:index associations
        M_type M;   ///< dictionary of codepoints for use in Node::drop()
        size_t N;   ///< Count of subtable layers

        /** DRY start table from effectively empty */
        void initialize();

        /** insert/re-use layers for new codepoint:index associations */
        inline void layer(size_t* last, size_t* next, size_t index) {
            T_type& T = this->T;
            *last = *next;
            *next = T[*last][index];
            if (*next == 0L) {
                T.push_back(vector<size_t>(8, 0L));
                T[*last][index] = N;
                *next = N++;
            }
        }

 public:
        /** Constructor for a Code Point Table for O(1) lookup */
        Node();

        /** Destructor for a Code Point Table for O(1) lookup */
        ~Node();

        /** Number of codepoints in table */
        size_t size() const;

        /** Lookup index associated with table (0 means error) */
        size_t peek(const size_t codepoint) const;

        /** Insert a new codepoint:association pair into table and dict */
        void poke(const size_t codepoint, const size_t association);

        /** Delete a codepoint:association from table and dict */
        void drop(const size_t codepoint);
};
}  // namespace jlettvin
#endif  // NODE_H_
