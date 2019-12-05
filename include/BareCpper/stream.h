/*
 * stream.h
 *
 * Created: 09/07/2016 23:18:04
 *  Author: CraigH
 */ 


#ifndef BARECOPPER_STREAM_H_
#define BARECOPPER_STREAM_H_

#include "io.h"
#include "fastmath.h"

namespace BareCpper {
namespace Stream
{

	
	template <typename _T>
	const char* toString( _T val, char buff[sizeof(_T)*3u+2] );
	
	template <>
	const char* toString<uint16_t>( uint16_t val, char buff[8u] )
	{
		char* iCursor = &buff[sizeof(buff) - 1];
		*iCursor = '\0';
		do {
			unsigned long m = val;
			val = fastDivide<10>(val);
			uint8_t remainder = m - 10 * val;
			*--iCursor = '0' + remainder;
		} while(val);
		return iCursor;
	}
	

} //END: Stream
} //END: BareCpper

#endif /* STREAM_H_ */