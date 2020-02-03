#ifndef BARECPPER_CHARACTERSET_8x5_1Bit_H
#define BARECPPER_CHARACTERSET_8x5_1Bit_H

#include "Types.h"


/** Data array to store a single 8x5 character
* @todo PORTING: Only tested on Atmega platform
*/
typedef UInt8 CharGlyph[5];

/** Get the specified characters data
* @remarks Characters shall be stored in Flash memory where relevant to save RAM
* @todo PORTING: Only tested on Atmega platform
*/
bool getCharGlyph( const Char chr, CharGlyph& outCharGlyph );

#endif