#pragma once
namespace BareCpper
{
    constexpr uint32_t CycleTimer::usToTicks(const uint32_t timeUs)
    {
        /// @todo Hard coded clock frequency!!
        return timeUs * 64U;
    }

    CycleTimer::CycleTimer()
    {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; //< Enable DWT
        start();
    }

    inline void CycleTimer::stop()
    {
        DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //< Disable cycle counter
    }

    inline void CycleTimer::reset()
    {
        DWT->CYCCNT = 0; //< Reset counter
    }

    /** @note 5-ticks on nrf52 */
    inline void CycleTimer::start()
    {
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; //< Enable cycle counter
    }

    inline uint32_t CycleTimer::count()
    {
        return DWT->CYCCNT;
    }

} //END: BareCpper