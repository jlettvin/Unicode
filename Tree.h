/* Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
/**
 * \class Tree
 *
 * \ingroup jlettvin
 *
 * \brief Create an O(1) association between codepoint and indices.
 *
 * Tree supports building word trees for fast lookup and canonicalization.
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
 * TODO make results of peek be a list of canonical:probability pairs.
 * If a token equals a canonical token, this has a relative probability P = 1.
 * If a Levenshtein1 token equals a canonical token of length 10, P = 0.9.
 * Fat finger statistics should apply for single-hand or both-hand offsets.
 */
#ifndef   TREE_H_
#define   TREE_H_

#include <exception>
#include <vector>
#include <map>

#include "Node.h"

using std::exception;
using std::vector;
using std::u32string;
using std::map;

/* Prefix string literal with U to make it 32bit wide: U"hello world" */

namespace       jlettvin {
class Tree {
 private:
     typedef u32string ustr;
     typedef vector<ustr> vustr;

     static const size_t count;
     static const vustr* nonep;
     static const vustr& none;
     static const ustr empty;

 public:
        /** Constructor for a Code Point Table for O(1) lookup */
        Tree();

        /** Destructor for a Code Point Table for O(1) lookup */
        ~Tree();

        /** Number of codepoints in table */
        size_t size() const;

        /** Lookup index associated with table (0 means error) */
        const vustr& peek(const ustr& token) const;

        /** Insert a new codepoint:association pair into table and dict */
        void poke(const ustr& token, const ustr& canonical = empty);

        /** Delete a codepoint:association from table and dict */
        void drop(const ustr& token);
};
}  // namespace jlettvin
#endif  // TREE_H_
