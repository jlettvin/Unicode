/* Page.c Copyright(c)2016 Jonathan D. Lettvin, All Rights Reserved */
#include <stdlib.h>

#include "Unicode.h"
#include "Page.h"

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

void nop() {}

Page_p Page(PageSpec_p specification) {

    size_t framebits = 12;
    size_t  pagebits = 12;
    size_t  callbits =  5;

    if (specification) {
        framebits = specification->framebits;
         pagebits = specification-> pagebits;
         callbits = specification-> callbits;
    }

    size_t frames = 1 << (framebits-1);
    size_t  bytes = 1 << ( pagebits-1);
    size_t  calls = 1 << ( callbits-1);

    Page_p space = (Page_p)calloc(sizeof(Page_t), 1);
    space-> call = (call_pp)calloc(sizeof(call_p), calls);
    space->frame = (void*) calloc(bytes, frames);

    for (size_t i = 0; i < calls; ++i) space->call[i] = &nop;

    return space;
}
