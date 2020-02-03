#ifndef BARECPPER_CYCLETIMER_HPP
#define BARECPPER_CYCLETIMER_HPP

namespace BareCpper
{
    //Clock ticks
    class CycleTimer
    {
    public:

        static constexpr uint32_t usToTicks(const uint32_t timeUs);

        static constexpr uint32_t msToTicks(const uint32_t timeMs)
        {
            return usToTicks(timeMs * 1000U);
        }

    public:
        CycleTimer();
        
        bool initialise();

        void stop();

        void reset();

        /** @note 5-ticks on nrf52 */
        void start();

        uint32_t count();
    };

} //END: BareCpper


#if !DOXYGEN ///< Platform specific interface definitions

    #if NRF52
        #include "nRF52/CycleTimer_nRF52.hpp"
    #endif

#endif

#endif