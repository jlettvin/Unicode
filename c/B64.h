/* B64.h Copyright(c) 2016 Jonathan D. Lettvin, All Rights Reserved. */

/** B64 is en encoding/decoding algorith (see en.wikipedia.org/wiki/Base64).
 *
 * Using these examples from wikipedia, a two-way transform is shown.
 * "M"      <=>     "TQ=="
 * "Ma"     <=>     "TWE="
 * "Man"    <=>     "TWFu"
 *
 * 8 bits taken 3 at a time is equal to 6 bits taken 4 at a time.
 * In other words, encoding expands by 4/3 and decoding shrinkgs by 3/4.
 *
 * The original 8 bit bytes are sliced up 6 bits at a time and
 * the 6 bits are encoded as common ASCII to avoid encoding problems.
 * The absence of high bits and control characters is historically valuable.
 * The example below shows how 3 bytes is converted to 6 bit fields
 * and how those fields are related to encoding letters.
 *
 * Man
 *    M        a        n
 * 01001101 01100001 01101110
 * 010011 010110 000101 101110
 *    19     22      5     46
 * T=[19] W=[22] F=[ 5] u=[46]
 * TWFu
 *
 * These are the declared interface functions.
 */

#ifndef C_B64_H_
#define C_B64_H_
void B64_decode(const char* source, char* target);  ///< as wikipedia describes
void B64_encode(const char* source, char* target);  ///< as wikipedia describes
#endif  // C_B64_H_
