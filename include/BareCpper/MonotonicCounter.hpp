#ifndef BARECPPER_CHRONO_MONOTONICCOUNTER_HPP
#define BARECPPER_CHRONO_MONOTONICCOUNTER_HPP

#include <cstdint> //< uint32_t

namespace BareCpper {

    /** Real-time clock counter using 32768Hz clock timer
    */
    class MonotonicCounter
    {
    public:
        using Count = std::uint32_t;

        /** Count of clock tick per second
        * @param[in]  duration  Duration of seconds to return count for
        * @return 32768 * count
        */
        static constexpr Count seconds(const size_t duration) noexcept;

    public:

        /** Initializing and start the RTC driver instance.
        */
        static inline void initialise() noexcept;

        /** Start the counter. 
        */
        static inline void start() noexcept;

        /** Stop the counter. 
        */
        static inline void stop() noexcept;

        /** Retrieve the counter value.
         * @details The counter increments by N ticks-per-second where N == MonotonicCounter::seconds(1)
         */
        static inline Count count() noexcept;
    };

} //END: BareCpper

#if !DOXYGEN ///< Platform specific interface definitions
    #if NRF52_SERIES
        #include "nRF52/MonotonicCounter_nRF52.hpp"
    #elif __SAMD51__
        #include "ATsamd/MonotonicCounter_ATsamd5x.hpp"
    #endif
#endif

#endif