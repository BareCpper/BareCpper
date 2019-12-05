/*
 * io.h
 *
 * Created: 02/07/2016 22:59:00
 *  Author: CraigH
 */ 


#ifndef BARECPPER_GPIO_H_
#define BARECPPER_GPIO_H_

#include <stdint.h>
#include "common.h"

#include <avr/io.h>

namespace BareCpper
{
	/** Each GPIO port drives up to eight or so pins and is controlled by three registers:
	- DDRx:   Data Direction Register, configures the pins as either inputs or outputs.
	- PORTx:  Output port register. Sets the output value on pins configured as outputs. 
				Enables or disables the pull-up resistor on pins configured as inputs.
	- PINx:   Input register, used to read an input signal. 
				On some devices (but not all, check the datasheet), this register can be used
				for pin toggling: writing a logic one to a PINx bit toggles the corresponding
				bit in PORTx, irrespective of the setting of the DDRx bit. 
				(http://www.atmel.com/Images/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf)
	*/
	namespace Port
	{
		/** Enumerator for identifying device GPIO ports
		Each port maps A to PORTA, B to PORTB etc
		*/
		enum class Id
		{
			  A
			, B
			, C
			, D
		};
				
		/** Gets the register used to configure/use the specified port 
		*/
		template<Id cPortId>
		struct Registers
		{
			static Register direction();
			static Register in();
			static Register out();
		};

		/** Access the pins on a port 
		i.e. Port::B::Pin3
		*/
		template<Id cPortId>
		struct PortPinList;
		
		/** Port type available from Port::B, Port::C etc
		*/
		template<Id cPortId>
		struct Interface : public Registers<cPortId>, public PortPinList<cPortId>
		{
		};


	#if DOXYGEN
		/** Port types 
			Port::B::read();
		*/
		typedef Interface<Id::B> B; ///< Supported on: Atmega328p
		typedef Interface<Id::C> C; ///< Supported on: Atmega328p
		typedef Interface<Id::D> D; ///< Supported on: Atmega328p
	#endif
	
	template<typename Pin> struct InputPin;
	template<typename Pin> struct OutputPin;
		
		template<const Port::Id cPortIdT, const uint8_t cPinIndexT >
		struct Pin
		{
			static const Port::Id cPortId = cPortIdT;
			static const uint8_t cPinIndex = cPinIndexT;
			static const uint8_t cPinMask = (uint8_t(1) << cPinIndex);

			typedef Interface<cPortId> Port;
			typedef InputPin<Pin<cPortIdT,cPinIndexT>> Input;
			typedef OutputPin<Pin<cPortIdT,cPinIndexT>> Output;

			static Port port() { return Port(); }
			static uint8_t index() { return cPinIndex; }
			static uint8_t mask() { return cPinMask; }

			
			//#define digitalBitRead( bit) bitRead( getDataRegister(bit), bit%8 )
			//#define digitalBitSet( bit ) bitSet( getDataRegister(bit), bit%8 )
			//#define digitalBitClear( bit ) bitClear( getDataRegister(bit), bit%8 )
			//#define digitalBitWrite( bit, bitValue) bitWrite( getDataRegister(bit), bit%8, bitValue )
			//#define digitalBitToggle( bit ) bitToggle( getDataRegister(bit), bit%8 )
		};

		template<typename Pin>
		struct OutputPin : public Pin
		{
			OutputPin()
			{
				Pin::Port::direction() |= Pin::cPinMask; //< Set pin as output
			}
		};
		
		template<typename Pin>
		struct InputPin : public Pin
		{
			InputPin()
			{
				Pin::Port::direction() &= ~Pin::cPinMask; //< Set pin as input
				Pin::Port::out() |= Pin::cPinMask;// turn On the Pull-up, now inputs with pull-up enabled
			}
		};
			

				
		/** Pins Entry stores the pin type using the index as the member name i.e. 3 pin had member name Pin3
		*/
		template< uint8_t cPinIndex, typename PinT > struct PinX;
		template< typename PinT > struct PinX<0u,PinT> { typedef PinT Pin0; };
		template< typename PinT > struct PinX<1u,PinT> { typedef PinT Pin1; };
		template< typename PinT > struct PinX<2u,PinT> { typedef PinT Pin2; };
		template< typename PinT > struct PinX<3u,PinT> { typedef PinT Pin3; };
		template< typename PinT > struct PinX<4u,PinT> { typedef PinT Pin4; };
		template< typename PinT > struct PinX<5u,PinT> { typedef PinT Pin5; };
		template< typename PinT > struct PinX<6u,PinT> { typedef PinT Pin6; };
		template< typename PinT > struct PinX<7u,PinT> { typedef PinT Pin7; };
		
		/** PinListGenerator */
		template< typename...>	struct Pins_Next; ///< List of arbitrary length
		template<>
		struct Pins_Next<> //< End of list generation
		{
			static const uint8_t cPinMaskWithNext = 0u;
		}; 
		template< typename Pin, typename... PinsRemaining >
		struct Pins_Next<Pin,PinsRemaining...> 
			: public PinX<sizeof...(PinsRemaining),Pin>
			, public Pins_Next<PinsRemaining...>			
		{
			/** Mask of this pins and all remaining pins in the list */
			static const uint8_t cPinMaskWithNext = Pin::cPinMask | Pins_Next<PinsRemaining...>::cPinMaskWithNext;
		};
	
		/** List of pins from any port selected by type
		@tparam  PinList  Variable length list of pin types.
		*/
		template< typename... PinList >
		struct Pins : public Pins_Next< PinList... >
		{
			static const uint8_t cLength = sizeof...(PinList); //< Number of pins in the list
			static const uint8_t cPinMask = Pins_Next< PinList... >::cPinMaskWithNext;

			inline static uint8_t read() 
			{
				//typedef typename Pins_Next< PinList... >::Pin0::Port Port_t;
				//TODO: Deal with mixed ports!?
				return Pins_Next< PinList... >::Pin0::Port::in() & cPinMask; 
			}
		};

		/** List of pins on the same port selected via index
		@tparam  cPortId  The Port id from those available in Port::Id
		@tparam  cPinIndices  Variable length list of pin indices.
		*/
		template<Port::Id cPortId, uint8_t... cPinIndices >
		struct PortPins : public Pins<Pin<cPortId,cPinIndices>...>
		{
		};


	} //END: Port
	
} //END: BareCpper


#if !DOXYGEN ///< Platform specific interface definitions

	#if _AVR_IOM328P_H_
		#include "iom328p.h" ///< Definitions for ATmega328
	#endif

#endif

#endif /* BARECPPER_GPIO_H_ */