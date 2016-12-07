/* B64.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#ifndef C_B64_H_
#define C_B64_H_
void B64_decode(const char* source, char* target);
void B64_encode(const char* source, char* target);
#endif  // C_B64_H_
