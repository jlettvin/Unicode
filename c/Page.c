#include <stdlib.h>

#include "Unicode.h"
#include "Page.h"

__attribute__((constructor))
void Page_init(void) {
    // size_t Page_Break = sizeof(address_t) / sizeof(unsigned short);
    Page_Fields = (size_t)PAGE_ADDRESS_BITS/sizeof(unsigned);
    Page_Size = 0x10000;
    Page_Base = calloc(Page_Size, 1);
    Page_Allocated = Page_Size;
    void **Page_ptr = (void **)Page_Base;
    for (size_t i=0; i < Page_Fields-1; ++i) {
        Page_ptr[0] = (void **)calloc(Page_Size, 1);
        Page_ptr = Page_ptr[0];
    }
}

/*
void dealloc(void** page, int layer) {
    for (size_t i = 0; i < Page_Size / Page_Fields; ++i) {
        // TODO dealloc(pointers)
    }
}
`
__attribute__((destructor))
void Page_fini(void) {
}
*/
