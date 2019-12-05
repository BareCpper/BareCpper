#pragma once

#include <cstdint>

#if AVR_PLATFORM
#include <avr/io.h>
#define getDataRegister( bit ) ((bit>=8) ? PORTB : PORTD)
#define getDataDirectionRegister( bit ) ((bit>=8) ? DDRB : DDRD)
#else
#warning "GPIO not implemented on this platform"
#endif

#define LOW 0
#define HIGH 1

#define bitRead(val, bit) (((val) >> (bit)) & 0x01)
#define bitSet(val, bit) ((val) |= (1 << (bit)))
#define bitClear(val, bit) ((val) &= ~(1 << (bit)))
#define bitToggle(val, bit) ((val) ^= (1 << (bit)))
#define bitWrite(val, bit, bitvalue) (bitvalue ? bitSet(val, bit) : bitClear(val, bit))

#define digitalBitRead( bit) bitRead( getDataRegister(bit), bit%8 )
#define digitalBitSet( bit ) bitSet( getDataRegister(bit), bit%8 )
#define digitalBitClear( bit ) bitClear( getDataRegister(bit), bit%8 )
#define digitalBitWrite( bit, bitValue) bitWrite( getDataRegister(bit), bit%8, bitValue )
#define digitalBitToggle( bit ) bitToggle( getDataRegister(bit), bit%8 )

/** Writes the state of a bit and sets another bit to on
    \remarks Both bits must be in the same port 0-7 or 8-13
*/
#define bitWriteAndSet(value, bit, bitvalue, setBit) \
	(bitvalue ? \
		((value) |= (1UL << (bit%8)) | (1UL << ((setBit)%8))) : \
		((value) &= ~(1UL << (bit%8))) |= (1UL << (setBit())%8)) )

#define digitalBitWriteAndSet( bit, bitvalue, setBit) \
	bitWriteAndSet( getDataRegister(bit), bit, bitvalue, setBit)

//http://iamsuhasm.wordpress.com/tutsproj/avr-gcc-tutorial/
#define setPinAsOutput( bit ) getDataDirectionRegister( (bit) ) |= (1<<((bit)%8))
#define setPinAsInput( bit ) getDataDirectionRegister( (bit) ) &= ~(1<<((bit)%8))
