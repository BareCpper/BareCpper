#ifndef BARECPPER_REALTIMECOUNTER_HPP
#define BARECPPER_REALTIMECOUNTER_HPP

#include <cstdint> //< uint32_t

namespace BareCpper {

    /** Real-time clock counter using 32768Hz clock timer
    */
    class RealtimeCounter
    {
    public:
        using Count = std::uint32_t;

        /** Count of clock tick per second
        * @param[in]  count  Count of seconds to return count for
        * @return 32768 * count
        */
        static constexpr Count seconds(const size_t count) noexcept;

    public:

        /** Initializing the RTC driver instance.
        */
        static inline void initialise() noexcept;

        /** Start the counter. 
        */
        static inline void start() noexcept;

        /** Stop the counter. 
        */
        static inline void stop() noexcept;

        /** Retrieve the counter value.
         * @details The counter increments by N ticks-per-second where N == RealtimeCounter::seconds(1)
         */
        static inline Count count() noexcept;
    };

} //END: BareCpper

#if !DOXYGEN ///< Platform specific interface definitions
    #if NRF52_SERIES
        #include "nRF52/RealtimeCounter_nRF52.hpp"
    #elif __SAMD51__
        #include "ATsamd/RealtimeCounter_ATsamd5x.hpp"
    #endif
#endif

#endif