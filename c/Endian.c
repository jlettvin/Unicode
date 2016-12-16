/* Endian.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */

/**
 * Invariant Endian handling
 */

#include "Endian.h"

/** Endian_static
 *
 * This provides the indices used for the second-level dereference.
 * By double dereferencing, the order of bytes is reversed automatically.
 */
static const Endian_invariant32_t Endian_static = { .u32 = ENDIAN_INDICES };

/** Endian_invariant
 * \param p32 a pointer to a 32 bit quantity.
 * \param off an index between 0 and 3 to choose a byte field from *p32.
 * \return address of byte indexed (allows assignment).
 */
unsigned char* Endian_invariant(void* p32, size_t off) {
    return &(*((Endian_invariant32_t*)p32)).u8[Endian_static.u8[off]];
}
