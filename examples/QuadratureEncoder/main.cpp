
/*End of auto generated code by Atmel studio */


#define F_CPU 8000000UL

#include <util/atomic.h>

#include "QuadratureEncoder.h"

#include "BareCpper/assert.h"
#include "BareCpper/gpio.h"
#include "BareCpper/serial.h"


typedef QueueSPSC<uint8_t,32> EncoderStateQueue;
EncoderStateQueue encoderStateQueue;

ISR(PCINT0_vect);
class PortBInterrupt
{
	friend void PCINT0_vect();
public:
    inline void call()
	{
	}
} portBInterrupt;

BareCpper::Pin9::Input encoder1A;
BareCpper::Pin8::Input encoder1B;
BareCpper::Pins<Pin8,Pin9> encoderPins; //< Both mins as zero-shifted mask

//Stream::SPI<Pin2,Pin3> log;
//Stream::I2C<Pin4,Pin5> log;

//Os
//Program Memory Usage 	:	2598 bytes   7.9 % Full
//Data Memory Usage 		:	424 bytes   20.7 % Full
//O3
//Program Memory Usage 	:	2608 bytes   8.0 % Full
//Data Memory Usage 		:	424 bytes   20.7 % Full

ISR(PCINT0_vect)
{
	encoderStateQueue.push( encoderPins.read() );
	assert( !encoderStateQueue.full() );
}

Encoder::Quadrature<> encoder;
BareCpper::Stream::UART0 serial;

int main(void)
{
#if 0 //724b, 16949c
	serial.write("hello world");
	serial.write( 123u );
#else //724b, 16949c
	serial << "hello world" << 123u;
#endif

	for (;;) 
	{
		//Check if port state changes
		if ( !encoderStateQueue.empty() )
		{		
			do //Update encoder with all port state changes
			{
				encoder.update( encoderStateQueue.pop() );
			}
			while( !encoderStateQueue.empty() );

			//Report new state
			serial.write('#');
			serial.write( encoder.getStep() );
			serial.write('\n');
		}
	}
}

