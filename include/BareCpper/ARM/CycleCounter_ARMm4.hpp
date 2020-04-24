#pragma once

#ifndef BARECPPER_CYCLETIMER_HPP
#  error "Include <BareCpper/CycleCounter.h> instead of this file."
#endif

#ifndef F_CPU
# error "Macro F_CPU must be defined"
#endif

namespace BareCpper
{
    inline constexpr uint32_t CycleCounter::usToTicks(const uint16_t timeUs)
    {
        /// @todo Hard coded clock frequency!!
        return timeUs * (F_CPU / 1000000);
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