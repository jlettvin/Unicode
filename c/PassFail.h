/* PassFail.c Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */
#ifndef C_PASSFAIL_H_
#define C_PASSFAIL_H_
/** passfail
 *
 * \param T a 0 or 1 value (boolean) to decide reporting.
 * \return nothing.
 */
void passfail(const int T, const char* F, const unsigned L, const char* msg);
#define PASSFAIL(T, msg) passfail(T, __FILE__, __LINE__, msg)
#endif  // C_PASSFAIL_H_
