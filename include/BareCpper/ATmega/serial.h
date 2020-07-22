/*
 * serial.h
 *
 * Created: 25/07/2016 21:23:10
 *  Author: CraigH
 */ 


#ifndef BARECOPPER_SERIAL_H_
#define BARECOPPER_SERIAL_H_

#include "stream.h"

namespace BareCpper {
namespace Stream
{

	template< typename RxPin, typename TxPin >
	class SerialUART;
	
	template<>
	class SerialUART<UartRx0,UartTx0>
	{
	public:

		SerialUART()
		{

			//Enable transmit and receive
			UCSR0B = (1<<RXEN0)|(1<<TXEN0);
			//
			UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));

			uint16_t baud = 57600;
			setBaud( baud );//The baudrate that we want to use
		}

		static void setBaud( uint32_t baud )//The baudrate that we want to use
		{
			uint16_t baud_setting = F_CPU / 4 / baud;  //The formula that does all the required maths
			
			//TODO: baud_setting > 8192 then UCSR0A = 0; baud_setting /= 2;
			UCSR0A = 1<<U2X0;
			
			baud_setting = (baud_setting-1) / 2;
			
			// assign the baud_setting, a.k.a. ubbr (USART Baud Rate Register)
			UBRR0H = (uint8_t)(baud_setting>>8);
			UBRR0L = (uint8_t)(baud_setting);
		}
		
		template <typename _T>
		static void writeUnsigned( _T val )
		{
			char buff[sizeof(_T)*3u+1];
			write( (const char*)toString(val, buff) );
		}

		//TODO: decimal/hex/oct mode etc
		template <typename _T>
		static void writeSigned( _T val )
		{
			if ( val < 0 ) //< output sign
			{
				write('-');
				val = -val;
			}
			writeUnsigned( val );
		}
		//TODO: missing enable_if and is_signed etc

		static void write( char byte )
		{
			//TODO; interup driven writes instead of waiting
			while(!(UCSR0A & (1<<UDRE0)));
			UDR0 = byte;
		}

		//String write
		static void write( char* cstr ) { write( static_cast<const char*>(cstr) ); }
		static void write( const char* cstr )
		{
			while (*cstr) { write(*cstr++); }
		}
		
		/* TODO: String/progmem literals have known size at compile time
		template<size_t cSize>
		static void write( const char cstr[cSize] )
		{
			for ( size_t i =0; i < cSize; ++i) { write(cstr[i]); }
		}
		*/
		
		//Unsigned types
		static void write( uint8_t val ) { writeUnsigned( val ); }
		static void write( uint16_t val ) { writeUnsigned( val ); }
		
		//Signed types
		static void write( int8_t val ) { writeSigned( val ); }
		static void write( int16_t val ) { writeSigned( val ); }
					
		template<typename _T>
		SerialUART<UartRx0,UartTx0>& operator << ( const _T& rhs )
		{
			write(rhs);
			return *this;
		}
	};


	typedef SerialUART<UartRx0,UartTx0> UART0;
	
} //END: Stream
} //END: BareCpper

#endif /* SERIAL_H_ */