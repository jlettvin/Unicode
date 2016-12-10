/* Unicode.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */

/**
 * Invariant Endian handling
 */
typedef union {
    unsigned long u32;      ///< data as a 32 bit representation
    unsigned char u8[4];    ///< data as 4  8 bit representations
} Endian_invariant32_t;

/** Endian_static
 *
 * This provides the indices used for the second-level dereference.
 * By double dereferencing, the order of bytes is reversed automatically.
 */
static const Endian_invariant32_t Endian_static = { .u32 = 0x03020100 };

/** Endian_static
 * \param p32 a pointer to a 32 bit quantity.
 * \param off an index between 0 and 3 to choose a byte field from *p32.
 * \return address of byte indexed (allows assignment).
 */
unsigned char* Endian_invariant(void* p32, unsigned long off) {
    return &(*((Endian_invariant32_t*)p32)).u8[Endian_static.u8[off]];
}
