#ifndef BARECPPER_CYCLETIMER_HPP
#define BARECPPER_CYCLETIMER_HPP

namespace BareCpper
{
    //Clock ticks
    class CycleCounter
    {
    public:

        static constexpr uint32_t usToTicks(const uint32_t timeUs);

        static constexpr uint32_t msToTicks(const uint32_t timeMs)
        {
            return usToTicks(timeMs * 1000U);
        }

    public:
        /** @note Implicit call to initialise() 
        */
        CycleCounter();
        
        bool initialise();

        void stop();

        void reset();

        /** @note 5-ticks on nrf52 */
        void start();

        uint32_t count();
    };

} //END: BareCpper


#if !DOXYGEN ///< Platform specific interface definitions

#if NRF52_SERIES
#include "nRF52/CycleCounter_nRF52.hpp"
#elif __SAMD51__
#include "ATsamd/CycleCounter_ATsamd5x.hpp"
#endif

#endif

#endif