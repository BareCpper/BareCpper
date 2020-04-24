/*
 * Created: 02/07/2016 22:59:00
 *  Author: CraigH
 */


#ifndef BARECPPER_COMMON_H_
#define BARECPPER_COMMON_H_

#include <cstdint>
#include <type_traits> //< std::integral_constant
#include "Delay.hpp" //< BareCpper::delay

namespace BareCpper
{
    //using size_t = std::size_t;
   // using uint8_t = std::uint8_t;

#if 0 //~todo Necessary>?
    enum class RegisterType
    {
        portDdr,
        portIn,
        portOut,
    };

    /** The type for a processor register which refer  to memory locations where the appropriate
    registers reside i.e. GPIO ports such as PORTB, PORTD etc
    */
    template< typename Impl >
    class RegisterInterface
    {
        uint8_t operator &(const uint8_t rhs) { return Impl::ref() & rhs; }
    };


    typedef volatile uint8_t& Register;
#endif



#if 0 //< gpio_legacy.hpp

#define bitRead(val, bit) (((val) >> (bit)) & 0x01)
#define bitSet(val, bit) ((val) |= (1 << (bit)))
#define bitClear(val, bit) ((val) &= ~(1 << (bit)))
#define bitWrite(val, bit, bitvalue) (bitvalue ? bitSet(val, bit) : bitClear(val, bit))


#define getDataRegister( bit ) PORTB
#define getDataDirectionRegister( bit ) DDRB

#define digitalBitRead( bit) bitRead( getDataRegister(bit), bit%8 )
#define digitalBitSet( bit ) bitSet( getDataRegister(bit), bit%8 )
#define digitalBitClear( bit ) bitClear( getDataRegister(bit), bit%8 )
#define digitalBitWrite( bit, bitValue) bitWrite( getDataRegister(bit), bit%8, bitValue )

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


#endif


    /** Synchronous polling for a condition to be true at a specified interval
     *
     * @param[in]  condition  Function of functor object whose return type evaluates to a boolean e.g. [](){ return rand() > 255; }
     * @param[in]  pollAttempts  Maximum number of condition checks. Must not be 0.
     * @param[in]  tickTimer  Timer used to measure ticks e.g. CycleCounter, MsTimer etc
     * @param[in]  delayTickCount  Delay between consecutive checks, in unit of Timer tick e.g. ClockCycle, Millisecond, Microsecond etc.
     * @return Set to true if the condition is met or false otherwise.
     */
    template<typename ConditionFn, typename Timer >
    bool pollForCondition(const ConditionFn& condition, uint32_t pollAttempts, Timer tickTimer, uint32_t delayTickCount)
    {
        for (; pollAttempts != 0; --pollAttempts)
        {
            if (condition())
            {
                return true;
            }
            delay(tickTimer, delayTickCount);
        }

        return false;
    }

    template <typename... >
    struct IndexOf;

    /** Specialisation of found type-index
    */
    template <typename FindT, typename... RemainT>
    struct IndexOf<FindT, FindT, RemainT...>
        : std::integral_constant<std::size_t, 0>
    {};

    /** Recursive search for type-index 
    */
    template <typename FindT, typename FirstT, typename... RemainT>
    struct IndexOf<FindT, FirstT, RemainT...>
        : std::integral_constant<std::size_t, 1 + IndexOf<FindT, RemainT...>::value>
    { };

} //END: BareCpper


#endif /* BARECPPER_REGISTERS_H_ */