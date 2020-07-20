#ifndef BARECPPER_RTCCOUNTER_HPP
#define BARECPPER_RTCCOUNTER_HPP

#include <cstdint> //< uint32_t

namespace BareCpper {

    /** Real-time clock counter using 32768Hz clock timer
    */
    class RtcCounter
    {
    public:

        /** Count of clock tick per second
        * @return 32768
        */
        static constexpr uint32_t ticksPerSecond();

        /** Get the duration of a clock tick as microseconds (us)
        * @remark The timer cannot represent time intervals smaller than this
        * @returns 30.517us resolution
        */
        static constexpr float32_t tickDurationUs()
        {
            return (1000000.0F / float32_t(ticksPerSecond())) + 0.5F;
        }

    public:

        /** Initializing the RTC driver instance.
        */
        inline void initialise(void);

        /** Start the counter. */
        inline void start(void);

        /** Stop the counter. */
        inline void stop(void);

        /** Retrieve the counter value.
         * @details The counter increments a tick for tickDurationUs()
         */
        inline uint32_t count(void);

        /** Retrieve the microsecond count value.
         * @details The counter is in Microseconds (us) with a resolution of microsecondsResolution() (i.e. 30.517us)
         */
        inline uint32_t countUs(void)
        {
            /// @ todo Integer expression with overflow handling! i.e. (NRF_RTC2->COUNTER*30517) / 1000
            return static_cast<uint32_t>(count() * tickDurationUs());
        }

    };

} //END: BareCpper

#if !DOXYGEN ///< Platform specific interface definitions
    #if NRF52_SERIES
        #include "nRF52/RtcCounter_nRF52.hpp"
    #elif __SAMD51__
        #include "ATsamd/RtcCounter_ATsamd5x.hpp"
    #endif
#endif

#endif