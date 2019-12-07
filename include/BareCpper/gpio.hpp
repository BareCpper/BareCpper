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

/** Each GPIO port drives 8, 16, 32 or so pins and is controlled by three registers in common. 
@note Some platforms also allow extra port capability as exetension to these:
- DDRx:   Data Direction Register, configures the pins as either inputs or outputs.
- PORTx:  Output port register. Sets the output value on pins configured as outputs. 
			Enables or disables the pull-up resistor on pins configured as inputs.
- PINx:   Input register, used to read an input signal. 
			On some devices (but not all, check the datasheet), this register can be used
			for pin toggling: writing a logic one to a PINx bit toggles the corresponding
			bit in PORTx, irrespective of the setting of the DDRx bit. 
			(http://www.atmel.com/Images/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf)
*/
namespace BareCpper
{			
	/** Gets the register used to configure/use the specified port 
	*/
	template<uint8_t cPortIndex>
    struct PortRegisters;

	/** Access the pins on a port 
	i.e. Port::B::Pin3
	*/
	template<uint8_t cPortIndex>
	struct PortPinList;
		
	/** Port type available from Port::B, Port::C etc
	*/
	template<uint8_t cPortIndex>
	struct Port : public PortRegisters<cPortIndex>, public PortPinList<cPortIndex>
	{
	};


#if DOXYGEN
	/** Port types 
		Port::B::read();
	*/
	typedef Port<1> PortB; ///< Supported on: Atmega328p
	typedef Port<2> PortC; ///< Supported on: Atmega328p
	typedef Port<3> PortD; ///< Supported on: Atmega328p
#endif
	
	template<typename Pin> struct InputPin;
	template<typename Pin> struct OutputPin;
		
	template<const uint8_t cPortIndexT, const uint8_t cPinIndexT >
	struct Pin
	{
		static const uint8_t cPortIndexT = cPortIndexT;
		static const uint8_t cPinIndex = cPinIndexT;
		static const uint8_t cPinMask = (uint8_t(1) << cPinIndex);

		typedef PortT<cPortIndex> Port;
		typedef InputPin<Pin<cPortIndex,cPinIndexT>> AsInput;
		typedef OutputPin<Pin<cPortIndex,cPinIndexT>> AsOutput;

        constexpr static Port port() { return Port(); }
		constexpr static uint8_t index() { return cPinIndex; }
        constexpr static uint8_t mask() { return cPinMask; }

			
		//#define digitalBitRead( bit) bitRead( getDataRegister(bit), bit%8 )
		//#define digitalBitSet( bit ) bitSet( getDataRegister(bit), bit%8 )
		//#define digitalBitClear( bit ) bitClear( getDataRegister(bit), bit%8 )
		//#define digitalBitWrite( bit, bitValue) bitWrite( getDataRegister(bit), bit%8, bitValue )
		//#define digitalBitToggle( bit ) bitToggle( getDataRegister(bit), bit%8 )
	};

    /** Pin assignement for output
    */
	template<typename Pin>
	struct OutputPin : public Pin
	{
		OutputPin()
		{
			Pin::Port::direction() |= Pin::cPinMask; //< Set pin as output
		}
	};

    /** Pin assignement for input
     * @note Enabled pull-up resistor (TBD behaviour)
     */
	template<typename Pin>
	struct InputPin : public Pin
	{
		InputPin()
		{
			Pin::Port::direction() &= ~Pin::cPinMask; //< Set pin as input
			Pin::Port::out() |= Pin::cPinMask;// turn On the Pull-up, now inputs with pull-up enabled
		}
	};
					
	/** Pins Entry stores the pin type using the index as the member name i.e. Pin<0> in a list if accessed at Pin3
	*/
	template< uint8_t cPinIndex, typename PinT > struct ListPin;
	template< typename PinT > struct ListPin<0u,PinT> { typedef PinT Pin0; };
	template< typename PinT > struct ListPin<1u,PinT> { typedef PinT Pin1; };
	template< typename PinT > struct ListPin<2u,PinT> { typedef PinT Pin2; };
	template< typename PinT > struct ListPin<3u,PinT> { typedef PinT Pin3; };
	template< typename PinT > struct ListPin<4u,PinT> { typedef PinT Pin4; };
	template< typename PinT > struct ListPin<5u,PinT> { typedef PinT Pin5; };
	template< typename PinT > struct ListPin<6u,PinT> { typedef PinT Pin6; };
	template< typename PinT > struct ListPin<7u,PinT> { typedef PinT Pin7; };
    template< typename PinT > struct ListPin<8u,PinT> { typedef PinT Pin8; };
    template< typename PinT > struct ListPin<9u,PinT> { typedef PinT Pin9; };
    template< typename PinT > struct ListPin<10u, PinT> { typedef PinT Pin10; };
    template< typename PinT > struct ListPin<11u, PinT> { typedef PinT Pin11; };
    template< typename PinT > struct ListPin<12u, PinT> { typedef PinT Pin12; };
    template< typename PinT > struct ListPin<13u, PinT> { typedef PinT Pin13; };
    template< typename PinT > struct ListPin<14u, PinT> { typedef PinT Pin14; };
    template< typename PinT > struct ListPin<15u, PinT> { typedef PinT Pin15; };
    template< typename PinT > struct ListPin<16u, PinT> { typedef PinT Pin16; };
    template< typename PinT > struct ListPin<17u, PinT> { typedef PinT Pin17; };
    template< typename PinT > struct ListPin<18u, PinT> { typedef PinT Pin18; };
    template< typename PinT > struct ListPin<19u, PinT> { typedef PinT Pin19; };
    template< typename PinT > struct ListPin<20u, PinT> { typedef PinT Pin20; };
    template< typename PinT > struct ListPin<21u, PinT> { typedef PinT Pin21; };
    template< typename PinT > struct ListPin<22u, PinT> { typedef PinT Pin22; };
    template< typename PinT > struct ListPin<23u, PinT> { typedef PinT Pin23; };
    template< typename PinT > struct ListPin<24u, PinT> { typedef PinT Pin24; };
    template< typename PinT > struct ListPin<25u, PinT> { typedef PinT Pin25; };
    template< typename PinT > struct ListPin<26u, PinT> { typedef PinT Pin26; };
    template< typename PinT > struct ListPin<27u, PinT> { typedef PinT Pin27; };
    template< typename PinT > struct ListPin<28u, PinT> { typedef PinT Pin28; };
    template< typename PinT > struct ListPin<29u, PinT> { typedef PinT Pin29; };
    template< typename PinT > struct ListPin<30u, PinT> { typedef PinT Pin30; };
    template< typename PinT > struct ListPin<31u, PinT> { typedef PinT Pin31; };
		
	/** PinListGenerator */
	template< typename...>	struct NextPinInList; ///< List of arbitrary length
	template<>
	struct NextPinInList<> //< End of list generation
	{
		static const uint8_t cPinMaskWithNext = 0u;
	}; 
	template< typename Pin, typename... PinsRemaining >
	struct NextPinInList<Pin,PinsRemaining...> 
		: public ListPin<sizeof...(PinsRemaining),Pin>
		, public NextPinInList<PinsRemaining...>			
	{
		/** Mask of this pins and all remaining pins in the list */
		static const uint8_t cPinMaskWithNext = Pin::cPinMask | NextPinInList<PinsRemaining...>::cPinMaskWithNext;
	};
	
	/** Read state of multiple pins from port
	@tparam  PinList  Variable length list of pin types.
    @todo We should have option to have the bits sparse, packed, ordered?
    @warning Does not cater for pins being on separate ports at this time
	*/
	template< typename... PinList >
	struct Pins : public NextPinInList< PinList... >
	{
		static const uint8_t cLength = sizeof...(PinList); //< Number of pins in the list
		static const uint8_t cPinMask = NextPinInList< PinList... >::cPinMaskWithNext;

		inline uint8_t read() 
		{
			//typedef typename NextPinInList< PinList... >::Pin0::Port Port_t;
			//TODO: Deal with mixed ports!?
			return Pin0::Port::in() & cPinMask; 
		}
	};

	/** List of pins on the same port selected via index
	@tparam  cPortIndex  The Port id from those available in Port::Id
	@tparam  cPinIndices  Variable length list of pin indices.
	*/
	template<uint8_t cPortIndex, uint8_t... cPinIndices >
	struct PortPins : public Pins<Pin<cPortIndex,cPinIndices>...>
	{
	};
	
} //END: BareCpper

#define BARECPPER_PORT_REGISTERS( PortIndex_, DDR_, WRITE_, READ_ ) \
	template<> struct Registers<PortIndex_> { \
		class DDR : public RegisterInterface<DDR> { static volatile uint8_t& ref() { return DDR_; } }; \
		class PortWrite : public RegisterInterface<PortWrite> { static volatile uint8_t& ref() { return WRITE_; } }; \
		static Register direction() { return DDR_; } \
		static Register out() { return WRITE_; } \
		static Register in() { return READ_; } \
	}; \
	typedef Interface<PortIndex_> Port ## PortIndex_;

#define BARECPPER_PORT_PINS( PortIndex_, .../*PIN0/1/2 etc*/ ) \
	template<> struct PortPinList<PortIndex_> : public PortPins<PortIndex_, __VA_ARGS__> {};


#if !DOXYGEN ///< Platform specific interface definitions

	#if _AVR_IOM328P_H_ //Defined by #include <avr/io.h>?        
		#include "gpio_ATmega328P.hpp" ///< Definitions for ATmega328
	#endif

#endif

#endif /* BARECPPER_GPIO_H_ */