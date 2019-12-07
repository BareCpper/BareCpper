/*
 * iom329p.h
 *
 * Created: 02/07/2016 22:53:56
 *  Author: CraigH
 */  
 /* barecpper/iom328p.h - definitions for ATmega328P. */
 
 #ifndef BARECPPER_GPIO_H_
 #  error "Include <barecpper/gpio.h> instead of this file."
 #endif

#if !_AVR_IOM328P_H_
#  error "AVR library error, please check and update accordingly."
#endif

#ifndef BARECPPER_GPIO_ATMEGA328P_H_
#define BARECPPER_GPIO_ATMEGA328P_H_

namespace BareCpper {

	BARECPPER_PORT_PINS(1, /* PORTB7, PORTB6, */  PORTB5, PORTB4, PORTB3, PORTB2, PORTB1, PORTB0); //<6&7 have clock crystal!
	BARECPPER_PORT_PINS(2, PORTC6, PORTC5, PORTC4, PORTC3, PORTC2, PORTC1, PORTC0 );
	BARECPPER_PORT_PINS(3, PORTD7, PORTD6, PORTD5, PORTD4, PORTD3, PORTD2, PORTD1, PORTD0 );

	BARECPPER_PORT_REGISTERS( 1, DDRB, PORTB, PINB );
	BARECPPER_PORT_REGISTERS( 2, DDRC, PORTC, PINC );
	BARECPPER_PORT_REGISTERS( 3, DDRD, PORTD, PIND );


	/** Mapping from ardunio pin index to Port::Pin
	*/
	//D (digital pins 0 to 7) 
	class Pin0 : public PortD::Pin0 {};
	class Pin1 : public PortD::Pin1 {};
	class Pin2 : public PortD::Pin2 {};
	class Pin3 : public PortD::Pin3 {};
	class Pin4 : public PortD::Pin4 {};
	class Pin5 : public PortD::Pin5 {};
	class Pin6 : public PortD::Pin6 {};
	class Pin7 : public PortD::Pin7 {};
	//B (digital pin 8 to 13) 	
	class Pin8 : public Port::B::Pin0 {};
	class Pin9 : public Port::B::Pin1 {};
	class Pin10 : public Port::B::Pin2 {};
	class Pin11 : public Port::B::Pin3 {};
	class Pin12 : public Port::B::Pin4 {};
	class Pin13 : public Port::B::Pin5 {};
	//C (analog input pins) 
	//TODO:

	/** Hardware Uart 0 **/
	class UartRx0 : public PortD::Pin0 {};
	class UartTx0 : public PortD::Pin1 {};
	
	/** Hardware SPI 0 **/
	class SpiMosi0 : public Port::B::Pin3 {};
	class SpiMiso0 : public Port::B::Pin4 {}; //< SPI Master-In Slave-Out
	class SpiSck0 : public Port::B::Pin5 {};
	
} //END: BareCopper

#endif