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
 * \version 0.0.1
 *
 * \date 2016/11/27 08:14
 *
 * license GPLv3
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

namespace       jlettvin {

using std::exception;
using std::vector;
using std::u32string;
using std::map;
using std::pair;

using jlettvin::Node;

/* Prefix string literal with U to make it 32bit wide: U"hello world" */

/** \class Tree
 *
 * \brief dictionary equivalent interface: high performance u32string lookup.
 *
 */
class Tree {
 private:
     typedef u32string ustr;
     typedef ustr::const_iterator ustr_iter;
     typedef vector<ustr> vustr;
     typedef pair<u32string, float> prustr;
     typedef vector<prustr *> vprustr;
     typedef vector< vector<size_t> > vvi;
     typedef map<u32string, float> mu32;
     typedef vector<mu32> vmu32;

     static const vprustr* nonep;
     static const vprustr& none;
     static const ustr empty;

     vector<Node *> table;
     vmu32 candidate;
     mu32 blank;

 public:
        /** Constructor for a Code Point Table for O(1) lookup */
        Tree();

        /** Destructor for a Code Point Table for O(1) lookup */
        ~Tree();

        /** Number of codepoints in table */
        size_t size() const;

        /** Lookup index associated with table (0 means error) */
        const mu32& peek(const ustr& token) const;

        /** Insert a new codepoint:association pair into table and dict */
        void poke(
                const ustr& token,
                const ustr& canonical = empty,
                const float score=1.0);

        /** Delete a codepoint:association from table and dict */
        void drop(const ustr& token);
};
}  // namespace jlettvin
#endif  // TREE_H_
