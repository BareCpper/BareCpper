/*
 * maths.h
 *
 * Created: 20/07/2016 17:17:43
 *  Author: CraigH
 */ 


#ifndef MATHS_H_
#define MATHS_H_

#include <stddef.h>

/** Fast bit-shift integer division operators 
*/
template< size_t Divisor, typename Type >
Type fastDivide( const Type val );

//1, 2, 3,  4,  5,  6,   7,   8,   9,  10,    11,   12,   13,    14,    15,    16,     17,     18,     19
//2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288
 
 /** Equivalent to: * 0.1f (Full range uses 6bits 0-25) 
 @note Uses 16-bit logic 
 */
template<> uint8_t fastDivide<10u,uint8_t>( const uint8_t val )
{ return (uint16_t(val) * 205) >> 11/*2048*/; }

/** Equivalent to: * 0.1f (Full range uses 13bits 0-6553) 
 @note Uses 32-bit logic 
 */
template<> uint16_t fastDivide<10u,uint16_t>( const uint16_t val )
{ return (uint32_t(val) * 52429) >> 19/*524288*/;  }

#endif /* MATHS_H_ */