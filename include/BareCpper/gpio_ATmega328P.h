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

#ifndef BARECPPER_IOM329P_H_
#define BARECPPER_IOM329P_H_

#define BARECPPER_PORT_REGISTERS( PORTID_, DDR_, WRITE_, READ_ ) \
	template<> struct Registers<Id::PORTID_> { \
		class DDR : public RegisterInterface<DDR> { static volatile uint8_t& ref() { return DDR_; } }; \
		class PortWrite : public RegisterInterface<PortWrite> { static volatile uint8_t& ref() { return WRITE_; } }; \
		static Register direction() { return DDR_; } \
		static Register out() { return WRITE_; } \
		static Register in() { return READ_; } \
	}; \
	typedef Interface<Id::PORTID_> PORTID_;

#define BARECPPER_PORT_PINS( PORTID_, .../*PIN0/1/2 etc*/ ) \
	template<> struct PortPinList<Port::Id::PORTID_> : public PortPins<Port::Id::PORTID_, __VA_ARGS__> {};

namespace BareCpper {
namespace Port
{

	BARECPPER_PORT_REGISTERS( B, DDRB, PORTB, PINB );
	BARECPPER_PORT_REGISTERS( C, DDRC, PORTC, PINC );
	BARECPPER_PORT_REGISTERS( D, DDRD, PORTD, PIND );

	BARECPPER_PORT_PINS(B, /* PORTB7, PORTB6, */  PORTB5, PORTB4, PORTB3, PORTB2, PORTB1, PORTB0); //<6&7 have clock crystal!
	BARECPPER_PORT_PINS(C, PORTC6, PORTC5, PORTC4, PORTC3, PORTC2, PORTC1, PORTC0 );
	BARECPPER_PORT_PINS(D, PORTD7, PORTD6, PORTD5, PORTD4, PORTD3, PORTD2, PORTD1, PORTD0 );

} //END: Port

	/** Mapping from ardunio pin index to Port::Pin
	*/
	//D (digital pins 0 to 7) 
	class Pin0 : public Port::D::Pin0 {};
	class Pin1 : public Port::D::Pin1 {};
	class Pin2 : public Port::D::Pin2 {};
	class Pin3 : public Port::D::Pin3 {};
	class Pin4 : public Port::D::Pin4 {};
	class Pin5 : public Port::D::Pin5 {};
	class Pin6 : public Port::D::Pin6 {};
	class Pin7 : public Port::D::Pin7 {};
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
	class UartRx0 : public Port::D::Pin0 {};
	class UartTx0 : public Port::D::Pin1 {};
	
	/** Hardware SPI 0 **/
	class SpiMosi0 : public Port::B::Pin3 {};
	class SpiMiso0 : public Port::B::Pin4 {}; //< SPI Master-In Slave-Out
	class SpiSck0 : public Port::B::Pin5 {};
	
} //END: BareCopper

#endif /* BARECPPER_IOM329P_H_ */