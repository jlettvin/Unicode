/* Page.h Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#ifndef C_PAGE_H_
#define C_PAGE_H_
/** Page.h and Page.c
 *
 * \class Omni
 *
 * \ingroup jlettvin
 *
 * \brief memory management scheme to support sparse memory use.
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


/** Page
 *
 * Memory management for getting (peek) and setting (poke)
 * unsigned and signed values of sizes of 1, 2, 4, and 8 bytes.
 *
 * Pages are allocated on demand by poke.
 * Values are assumed to be 0 when peek addresses unallocated memory.
 *
 * IMPORTANT!  These are implemented as macros generating inline functions.
 * This is a nod to the DRY principle (Don't repeat yourself)
 * and also avoiding generation of stack frames by function calls.
 *
 * inline functions (used for one approach) has some advantage while
 * C preprocessor macros (another approach) has other advantages.
 *
 * http://stackoverflow.com/questions/22767523/
 *      what-inline-attribute-always-inline-means-in-the-function
 * https://gcc.gnu.org/ml/gcc-help/2007-01/msg00049.html
 *
 * These are the declared interface functions.
 */
#include <stdio.h>   ///< remove after debugging
#include <stdlib.h>

#include "Unicode.h"

/** NEW MACROS
 * These macros enable object-oriented programming in C where
 * a data space of all one data type is declared, constructed
 * and operated upon by peek and poke methods, and finally destructed.
 *
 * Subsequent to construction, the object acts as an autonomous data space
 * containing data all of one type where peek of non-existent data returns 0
 * and poke of non-existent data causes sparse allocation of sufficient
 * data and linkage to allow a peek operation to recover the data.
 */

/** PAGE_TYPEDEF must be a macro.
 * inline was considered but could not vary with types.
 * The struct and constructor must be declared in Page.h.
 */
#define PAGE_TYPEDEF(type, abytes) \
typedef struct _page_t_##type { \
    void** base; \
    size_t bytes; \
    size_t bits; \
    size_t pshift; \
    size_t pcount; \
    size_t psize; \
    struct _page_t_##type* (*ctor)(); \
    void (*dtor)(struct _page_t_##type* self); \
    type (*peek)(struct _page_t_##type* self, size_t index); \
    void (*poke)(struct _page_t_##type* self, size_t index, type value); \
} page_t_##type; \
page_t_##type* page_t_##type##_ctor();

/** PAGE_DEFINE must be a macro.
 * inline was considered but could not vary with types.
 * The implementations of the constructor and other pointed at functions
 * must be defined in Page.c to avoid duplicate symbols.
 * TODO(jlettvin): implement peek/poke/dtor recursion
 * TODO(jlettvin): assert that abytes is 1, 2, 3, or 4.
 * TODO(jlettvin): make peek/poke/dtor recursion use abytes for dereference.
 * TODO(jlettvin): rather than recursion, simply iterate updating a pointer.
 */
#define PAGE_DEFINE(type, abytes, power) \
void page_t_##type##_dtor(page_t_##type *self) { \
    free(self->base);  /* recursively free before this. */ \
} \
type page_t_##type##_peek(page_t_##type *self, size_t index) { \
    size_t address = index << sizeof(type); \
    self = self; \
    return (type)address; \
} \
void page_t_##type##_poke(page_t_##type* self, size_t index, type value) { \
    size_t address = index << sizeof(type); \
    self = self; \
    self->base || (self->base = (void**)calloc(self->psize, 1)); \
    value = (type)address; \
} \
page_t_##type* page_t_##type##_ctor() { \
    size_t bytes_per_element = sizeof(type); \
    size_t elements_per_page = 1L << power; \
    size_t bytes_per_page = bytes_per_element * elements_per_page; \
    page_t_##type* self = \
        (page_t_##type *)calloc(sizeof(page_t_##type), 1); \
    self->bytes = abytes; \
    self->bits = abytes * 8; \
    self->pshift = power; \
    self->pcount = bytes_per_page; \
    self->psize = bytes_per_page; \
    self->base = (void**)0; \
    self->dtor = &page_t_##type##_dtor; \
    self->peek = &page_t_##type##_peek; \
    self->poke = &page_t_##type##_poke; \
    return self; \
}

/** PAGE_TYPEDEF declares the struct types/constructor for paged typed data.
 */
PAGE_TYPEDEF(ue0_t, 32)
PAGE_TYPEDEF(ue1_t, 32)
PAGE_TYPEDEF(ue2_t, 32)
PAGE_TYPEDEF(ue3_t, 32)
PAGE_TYPEDEF(se0_t, 32)
PAGE_TYPEDEF(se1_t, 32)
PAGE_TYPEDEF(se2_t, 32)
PAGE_TYPEDEF(se3_t, 32)


#define PAGE_ADDRESS_BITS 32  // Must be either 32 or 64

#if     PAGE_ADDRESS_BITS == 32

typedef ue2_t address_t;
#define Page_field \
    size_t field[sizeof(address_t)/sizeof(ue1_t)]; \
    field[0] = offset>>16; \
    field[1] = offset & 0xffff; \
    void** next = (void**)Page_Base; \
    void** last = (void**)Page_Base; \
    last = next; \
    next = (void**)(((ue0_t**)next)[field[1]]);
#define Page_allocate \
    if (!next) { \
        last[field[1]] = (void**) calloc(Page_Size, 1); \
        next = last[field[1]]; \
    }

#elif   PAGE_ADDRESS_BITS == 64

typedef ue3_t address_t;
#define Page_field \
    size_t field[sizeof(address_t)/sizeof(ue1_t)]; \
    field[3] = (offset >> 48) & 0xffff; \
    field[2] = (offset >> 32) & 0xffff; \
    field[1] = (offset >> 16) & 0xffff; \
    field[0] = (offset      ) & 0xffff; \
    Page_allocate; \
    void** next = (void**)Page_Base; \
    void** last = (void**)Page_Base; \
    next = (void**)(((ue0_t**)next)[field[3]]); \
    if (next) next = (void**)(((ue0_t**)next)[field[2]]); \
    if (next) next = (void**)(((ue0_t**)next)[field[1]]);
#define PAGE_allocate \
    if (!next) { \
        void** next = (void**)Page_Base; \
        for (int f = 4; f--; ) { \
            if (next) next = (void**)(((ue0_t**)next)[f]); \
            else { \
                last[f] = (void**) calloc(Page_Size, 1); \
                next = last[f]; \
            } \
        } \
    }

#endif

#define PEEK_FOR(type) \
    __attribute__((always_inline)) \
    inline type Page_peek_##type(address_t address) \
    { \
        static const size_t size = sizeof(type); \
        size_t offset = address * size; \
        if (offset >= Page_Allocated) return (type)0; \
        Page_field; \
        type retval = (type)0; \
        if (next) retval = *(type*)(((ue0_t*)next)+field[0]); \
        return retval; \
    }

// TODO(jlettvin): Note that no new page is created when going off last page.
// Cause page fault equivalent and make a new page.
#define POKE_FOR(type) \
    __attribute__((always_inline)) \
    inline void Page_poke_##type(address_t address, type value) \
    { \
        static const size_t size = sizeof(type); \
        size_t offset = address * size; \
        if (offset >= Page_Allocated) { \
            return; \
        } \
        Page_field; \
        Page_allocate; \
        *(type*)(((ue0_t*)next)+field[0]) = value;\
    }

void*  Page_Base;
size_t Page_Allocated;
size_t Page_Size;
size_t Page_Fields;

// Generate the inline functions for accessing data in paged memory.
// #pragma message "Page_peek_ue0_t"
PEEK_FOR(ue0_t)
// #pragma message "Page_peek_se0_t"
PEEK_FOR(se0_t)
// #pragma message "Page_peek_ue1_t"
PEEK_FOR(ue1_t)
// #pragma message "Page_peek_se1_t"
PEEK_FOR(se1_t)
// #pragma message "Page_peek_ue2_t"
PEEK_FOR(ue2_t)
// #pragma message "Page_peek_se2_t"
PEEK_FOR(se2_t)
// #pragma message "Page_peek_ue3_t"
PEEK_FOR(ue3_t)
// #pragma message "Page_peek_se3_t"
PEEK_FOR(se3_t)

// #pragma message "Page_poke_ue0_t"
POKE_FOR(ue0_t)
// #pragma message "Page_poke_se0_t"
POKE_FOR(se0_t)
// #pragma message "Page_poke_ue1_t"
POKE_FOR(ue1_t)
// #pragma message "Page_poke_se1_t"
POKE_FOR(se1_t)
// #pragma message "Page_poke_ue2_t"
POKE_FOR(ue2_t)
// #pragma message "Page_poke_se2_t"
POKE_FOR(se2_t)
// #pragma message "Page_poke_ue3_t"
POKE_FOR(ue3_t)
// #pragma message "Page_poke_se3_t"
POKE_FOR(se3_t)

#endif  // C_PAGE_H_
