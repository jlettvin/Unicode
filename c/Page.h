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

typedef void (*call_p)(void);   ///< A function pointer.

typedef call_p* call_pp;        ///< A pointer to an array of function ptrs.

/** Page_t
 * A VM (virtual memory) with demand paging.
 */
typedef struct {
    // https://gcc.gnu.org/onlinedocs/gcc-3.3/gcc/Type-Attributes.html
    void* frame __attribute__ ((aligned));  ///< make access efficient.
    call_pp call;                 ///< A dispatch table of function pointers.
    unsigned char prefetch[32];  ///< Used to buffer instructions from VM.
} Page_t;
typedef Page_t* Page_p;

/** PageSpec_t
 * Tell Page(spec) what to allocate in the Page_t it generates.
 */
typedef struct {
    size_t framebits;  ///< Used to generate the frame count.
    size_t pagebits;   ///< Used to generate the page size.
    size_t callbits;   ///< Used to generate the call table.
} PageSpec_t;
typedef PageSpec_t* PageSpec_p;

/** Page
 * Page(specification) allocates a Page_t to specification and initializes it.
 */
Page_p Page(PageSpec_p specification);
#endif  // C_PAGE_H_
