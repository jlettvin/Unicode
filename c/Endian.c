/* Unicode.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */

// Endian invariant handling
typedef union { unsigned long u32; unsigned char u8[4]; } endian_invariant32_t;
static const endian_invariant32_t Endian_static = { .u32 = 0x03020100 };
unsigned char* endian_invariant(void* p32, unsigned long off) {
    return &(*((endian_invariant32_t*)p32)).u8[Endian_static.u8[off]];
}
