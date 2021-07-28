#ifndef BARECPPER_DELAY_HPP
#define BARECPPER_DELAY_HPP

#include <cstdint>
#include "CycleCounter.hpp" //< CycleCounter

namespace BareCpper
{

    /** Wait for a duration of timer.count()
    * @todo Setup a time event ... __WFI()
    * @note ALWAYS_INLINE
    */
    template<typename Timer >
    __attribute__((always_inline)) static inline void continueAt(Timer& timer, const uint32_t tickEnd)
    {
        while (timer.count() < tickEnd) 
        {
#if 0
            __NOP(); ///< @todo NOP or not-to NOP
#endif
        };
    }

    /** Wait for a duration of time
     * @param[in]  tickTimer  Timer used to measure ticks e.g. CycleCounter, MsTimer etc
     * @param[in]  timerTickCount  Delay between consecutive checks, in unit of Timer tick e.g. ClockCycle, Millisecond, Microsecond etc.
     * @return Set to true if the condition is met or false otherwise.
    * @note ALWAYS_INLINE
     */
    template<typename Timer >
    __attribute__((always_inline)) static inline void delay(Timer& timer, const uint32_t timerTickCount)
    {
        continueAt(timer, timer.count() + timerTickCount);
    }

    /// @todo Cleanup impl re comments!
    inline void delayNs(const uint16_t ns)
    {
        static CycleCounter timer; //< @todo Multiple initialise unnecessarily! delayUs/ms/Ns
        delay(timer, CycleCounter::nsToTicks(ns));
    }

    /// @todo Cleanup impl re comments!
    inline void delayUs(const uint16_t us)
    {
        static CycleCounter timer; //< @todo Multiple initialise unnecessarily! delayUs/ms/Ns
        delay(timer, CycleCounter::usToTicks(us));
    }

    /// @todo Cleanup impl re comments!
    inline void delayMs(const uint16_t ms)
    {
        static CycleCounter timer; //< @todo Multiple initialise unnecessarily! delayUs/ms/Ns
        delay(timer, CycleCounter::msToTicks(ms));
    }

} //END: BareCpper
#endif
