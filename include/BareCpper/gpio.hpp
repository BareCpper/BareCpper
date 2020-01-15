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

    template< size_t PinIndex, typename PinT > struct PinN;
    template< typename PinT > struct PinN<0u, PinT> { typedef PinT Pin0; };
    template< typename PinT > struct PinN<1u, PinT> { typedef PinT Pin1; };
    template< typename PinT > struct PinN<2u, PinT> { typedef PinT Pin2; };
    template< typename PinT > struct PinN<3u, PinT> { typedef PinT Pin3; };
    template< typename PinT > struct PinN<4u, PinT> { typedef PinT Pin4; };
    template< typename PinT > struct PinN<5u, PinT> { typedef PinT Pin5; };
    template< typename PinT > struct PinN<6u, PinT> { typedef PinT Pin6; };
    template< typename PinT > struct PinN<7u, PinT> { typedef PinT Pin7; };
    template< typename PinT > struct PinN<8u, PinT> { typedef PinT Pin8; };
    template< typename PinT > struct PinN<9u, PinT> { typedef PinT Pin9; };
    template< typename PinT > struct PinN<10u, PinT> { typedef PinT Pin10; };
    template< typename PinT > struct PinN<11u, PinT> { typedef PinT Pin11; };
    template< typename PinT > struct PinN<12u, PinT> { typedef PinT Pin12; };
    template< typename PinT > struct PinN<13u, PinT> { typedef PinT Pin13; };
    template< typename PinT > struct PinN<14u, PinT> { typedef PinT Pin14; };
    template< typename PinT > struct PinN<15u, PinT> { typedef PinT Pin15; };
    template< typename PinT > struct PinN<16u, PinT> { typedef PinT Pin16; };
    template< typename PinT > struct PinN<17u, PinT> { typedef PinT Pin17; };
    template< typename PinT > struct PinN<18u, PinT> { typedef PinT Pin18; };
    template< typename PinT > struct PinN<19u, PinT> { typedef PinT Pin19; };
    template< typename PinT > struct PinN<20u, PinT> { typedef PinT Pin20; };
    template< typename PinT > struct PinN<21u, PinT> { typedef PinT Pin21; };
    template< typename PinT > struct PinN<22u, PinT> { typedef PinT Pin22; };
    template< typename PinT > struct PinN<23u, PinT> { typedef PinT Pin23; };
    template< typename PinT > struct PinN<24u, PinT> { typedef PinT Pin24; };
    template< typename PinT > struct PinN<25u, PinT> { typedef PinT Pin25; };
    template< typename PinT > struct PinN<26u, PinT> { typedef PinT Pin26; };
    template< typename PinT > struct PinN<27u, PinT> { typedef PinT Pin27; };
    template< typename PinT > struct PinN<28u, PinT> { typedef PinT Pin28; };
    template< typename PinT > struct PinN<29u, PinT> { typedef PinT Pin29; };
    template< typename PinT > struct PinN<30u, PinT> { typedef PinT Pin30; };
    template< typename PinT > struct PinN<31u, PinT> { typedef PinT Pin31; };


   // template<bool> struct Valid; ///< SFINAE

    template< size_t PortIndex >
    struct PortPins
    {
        template<size_t PinIndex> ///< @todo limit Valid port indices to supported range in GCC 'compatible' manner /*, typename = Valid<true>*/
        struct Pin;
    };

    /** Helper types to typedef Pin<N> to a different index of PinY
     * @tparam[in] AliasIndex  Index to map pin to i.e. Pin0 where the '0' is the cIndex
    */
    template< size_t AliasIndex, size_t PortIndex, size_t PinIndex >
    using PinIndexAlias = PinN< AliasIndex, typename PortPins<PortIndex>::template Pin<PinIndex> >;

    /** Helper types to typedef Pin<N> to member PinN at the same index the pin exists on a port
    */
    template< size_t PortIndex, size_t PinIndex >
    using PortPinAlias = PinN< PinIndex, typename PortPins<PortIndex>::template Pin<PinIndex> >;


    /** Define instantiation of all pins available on a port by index
    @tparam  cPortIndex  The Port id from those available in Port::Id
    @tparam  cPinIndices  Variable length list of pin indices.

    @remark Shall be instantiated by gpio_PLATFORM.hpp header for each available port:
        template<>
        struct Port<0> : AllPortPins<0
            , { 0..N indices} >
        {
        };
    */
    template< size_t PortIndex, size_t ...PinIndices >
    struct AllPortPins
        : PortPins<PortIndex>
        , PortPinAlias< PortIndex, PinIndices >...
    {
    };


    template< size_t FirstIndex, typename ...PinsT >
    struct SequencePortPinsN;

    template< size_t FirstIndex, typename FirstPinT, typename ...PinsT >
    struct SequencePortPinsN< FirstIndex, FirstPinT, PinsT...>
        : PinN< FirstIndex, FirstPinT >
        , SequencePortPinsN< FirstIndex + 1U, PinsT... >
    {
    };

    template< size_t FirstIndex, typename FirstPinT>
    struct SequencePortPinsN< FirstIndex, FirstPinT >
        : PinN< FirstIndex, FirstPinT >
    {
    };

    template< typename ...PinsT >
    using SequencePortPins = SequencePortPinsN< 0, PinsT... >;

    /** Brings a collection of Pins into a set
    * @warning All pins must reside on the same port
    */
    template< typename ...PinsT >
    struct Pins
        : SequencePortPins< PinsT... >
    {
        //using Input = SequencePortPins< typename PinsT::Input... >;
    };

    template< size_t PortIndex>
    struct Port;

    /** Pin assignement for output
    */
    template<typename Pin>
    struct OutputPin : Pin
    {
        OutputPin()
        {
            //Pin::Port::direction() |= Pin::cPinMask; //< Set pin as output
        }
    };


    /** Pin assignement for input
     * @note Enabled pull-up resistor (TBD behaviour)
     */
    template<typename Pin>
    struct InputPin : Pin
    {
        InputPin()
        {
            // Pin::Port::direction() &= ~Pin::cPinMask; //< Set pin as input
            // Pin::Port::out() |= Pin::cPinMask;// turn On the Pull-up, now inputs with pull-up enabled
        }
    };

    typedef uint32_t PortRegister_t;

    template<typename...PinsT> constexpr PortRegister_t mask(const PinsT&...);

    template< typename PinsT >
    constexpr PortRegister_t mask(const PinsT& pin)
    {
        return PortRegister_t(1U) << pin.index;
    }

    template< typename FirstPinT, typename ...PinsT >
    constexpr PortRegister_t mask(const FirstPinT& first, const PinsT& ...rest)
    {
        return (mask(first) | mask(rest...));
    }

    template<typename ...PinsT> constexpr PortRegister_t mask()
    {
        return  mask(PinsT()...); //< @todo pre c++17 requires recusrion
    }

    template< typename ...PinsT >
    constexpr PortRegister_t mask(const Pins< PinsT...>&)
    {
        return mask<PinsT...>();
    }


#if 0 ///WIP
    /** Read state of multiple pins from port
        @tparam  PinList  Variable length list of pin types.
        @todo We should have option to have the bits sparse, packed, ordered?
        @warning Does not cater for pins being on separate ports at this time
        */
    read();
#endif



#if 0 && DOXYGEN
	/** Port types 
		Port::B::read();
	*/
	typedef Port<1> PortB; ///< Supported on: Atmega328p
	typedef Port<2> PortC; ///< Supported on: Atmega328p
	typedef Port<3> PortD; ///< Supported on: Atmega328p
#endif
	
	

	
} //END: BareCpper


#if !DOXYGEN ///< Platform specific interface definitions

	#if _AVR_IOM328P_H_ //Defined by #include <avr/io.h>?        
		#include "ATmega/Gpio_ATmega328P.hpp" ///< Definitions for ATmega328
	#endif

    #ifdef NRF52
        #include "nRF52/Gpio_nRF52832.hpp"
    #endif

#endif

namespace BareCpper
{
    static_assert(mask<Pin0>() != mask<Pin1>(), "mask<PinA> != mask<PinB>() implementation error");
    static_assert((mask<Pin0>() | mask<Pin1>()) == mask<Pins<Pin0,Pin1>>(), "mask<Pins>( implementation error");

} //END: BareCpper

#endif /* BARECPPER_GPIO_H_ */