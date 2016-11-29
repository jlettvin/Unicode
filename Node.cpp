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
 * license GPLv3
 *
 * \copyright Copyright(C) 2016 Jonathan D. Lettvin, All Rights Reserved"
 *
 */
#include <iostream>
#include <iomanip>
#include <limits>

#include "Node.h"  ///< cpplint complains even though existing file is included

namespace jlettvin {

    using std::endl;
    using std::cout;
    using std::hex;
    using std::setw;
    using std::setfill;
    using std::ostream;

    // private:
    /** \brief DRY initializer */
    void Node::initialize() {
        T.clear();
        T.push_back(subtable_t(8, 0L));
        T.push_back(subtable_t(8, 0L));
        N = 2;
    }

    // public:
    // Node::Node() : T(2, vector<size_t>(8, 0L)), N(2L) { }
    /** \brief Constructor
     * This could use C++ instance initializers but
     * because Node::drop() needs to perform the same function
     * it is refactored into a single call so
     * changes to initialization happen in one place
     * following the DRY principle.
     */
    Node::Node() { initialize(); }

    /** \brief Destructor has no responsibilities */
    Node::~Node() { }

    /** \brief Size is best retrieved from the count of dictionary keys.
     * \return size
     */
    size_t Node::size() const { return M.size(); }

// Convenience macro to expose regular access pattern for peek and poke
#define STEP(codepoint, shift) ((codepoint >> (shift*3)) & 0x7L)

// Convenience macro to expose regular access pattern for peek
#define Q(n) [STEP(codepoint, n)]

    /** \brief peek O(1) lookup around which this class is focused.
     * \param codepoint
     * \return assoc
     *
     * A codepoint is cut into 7 3bit chunks which are used to
     * dereference the table 7 times after which
     * the last dereference produces the associated value.
     * The number 7 and 3 bits is chosen because 7*3 = 21
     * where 21 bits is the number of bits needed for
     * Unicode codepoints up to the value 0x10FFFF (max).
     * It also is parsimonious of space enabling
     * a compromise between speed and size.
     * This is a form of sparse table implementation.
     */
    target_t Node::peek(const codepoint_t codepoint) const {
        return (codepoint > sentinel) ? 0L :
            T[ T[ T[ T[ T[ T[ T[1] Q(6)] Q(5)] Q(4)] Q(3)] Q(2)] Q(1)] Q(0);
    }

// Convenience macro to expose regular access pattern for poke
#define R(n) this->layer(&last, &next, STEP(codepoint, n));

    /** \brief poke ~O(1) table and backup dict insertion.
     * \param codepoint
     * \param assoc is the intended output of a lookup
     *
     * Where a needed layer already exists, it is re-used.
     * Where it does not, it is inserted and then used.
     */
    void Node::poke(const codepoint_t codepoint, const target_t assoc) {
        if (codepoint > sentinel) return;

        M[codepoint] = assoc;

        either_t last = 0L, next = 1L;
        R(6L); R(5L); R(4L); R(3L); R(2L); R(1L);
        T[next][STEP(codepoint, 0L)] = assoc;
    }

    /** \brief drop mechanism for removing a codepoint:assoc from the loopup.
     * \param codepoint
     *
     * It is poorly implemented, rebuilding the Node from scratch and
     * then re-inserting all the associations from the dictionary.
     * TODO there must be a better way than rebuilding from scratch.
     */
    void Node::drop(const codepoint_t codepoint) {
        M_iter iter;
        iter = M.find(codepoint);
        if (codepoint > sentinel || iter == M.end()) return;
        M.erase(iter);
        initialize();
        for (iter=M.begin(); iter != M.end(); ++iter)
            this->poke(iter->first, iter->second);
    }

}  // namespace jlettvin
