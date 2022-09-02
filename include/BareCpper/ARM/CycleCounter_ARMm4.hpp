#ifndef CYCLE_COUNTER_ARMM4_HPP
#define CYCLE_COUNTER_ARMM4_HPP

#ifndef CORE_FREQUENCY
#ifndef F_CPU
# error "Macro F_CPU or CORE_FREQUENCY must be defined"
#else
#define CORE_FREQUENCY F_CPU
#endif
#else

#endif

namespace BareCpper
{
    inline constexpr uint32_t CycleCounter::usToTicks(const uint16_t timeUs)
    {
        return timeUs * (CORE_FREQUENCY / 1000000);
    }
    
    inline CycleCounter::CycleCounter()
    {
        initialise();
    }

    inline bool CycleCounter::initialise()
    {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; //< Enable DWT
        start();
        return true;
    }

    inline void CycleCounter::stop()
    {
        DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //< Disable cycle counter
    }

    inline void CycleCounter::reset()
    {
        DWT->CYCCNT = 0; //< Reset counter
    }

    /** @note 5-ticks on nrf52 */
    inline void CycleCounter::start()
    {
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; //< Enable cycle counter
    }

    inline uint32_t CycleCounter::count()
    {
        return DWT->CYCCNT;
    }

} //END: BareCpper

#endif //CYCLE_COUNTER_ARMM4_HPP