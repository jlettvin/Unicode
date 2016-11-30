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
 * TODO Implement the functions.
 */
#include <iostream>
#include <iomanip>
#include <limits>

#include "Tree.h"  ///< cpplint complains even though existing file is included

namespace jlettvin {

    using std::endl;
    using std::cout;
    using std::hex;
    using std::setw;
    using std::setfill;
    using std::ostream;

    const Tree::vprustr* Tree::nonep = new vprustr();
    const Tree::vprustr& Tree::none = *nonep;
    const Tree::ustr Tree::empty = U"";

    // private:

    // public:
    /** \brief Constructor
     */
    Tree::Tree() { }

    /** \brief Destructor has no responsibilities */
    Tree::~Tree() { }

    /** \brief Size is best retrieved from the count of dictionary keys.
     * \return size
     */
    size_t Tree::size() const { return 0; }

    /** \brief peek O(N) lookup around which this class is focused.
     * \param token
     * \return assoc
     */
    const Tree::vprustr& Tree::peek(const Tree::ustr& token) const {
        cout << sizeof(token) << endl;
        return none;
    }

    /** \brief poke O(N) table and backup dict insertion.
     * \param token
     * \param canonical is a possible canonical version of token
     * \param score is a relative probability of token to canonical match
     */
    void Tree::poke(
            const Tree::ustr& token,
            const Tree::ustr& canonical,
            const float score)
    {
        cout <<
            sizeof(token) <<
            '(' << sizeof(canonical) << ')' <<
            score << endl;
    }

    /** \brief drop mechanism for removing a codepoint:assoc from the loopup.
     * \param token
     */
    void Tree::drop(const Tree::ustr& token) {
        cout << sizeof(token) << endl;
    }

}  // namespace jlettvin
