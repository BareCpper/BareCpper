#ifndef BARECPPER_STEADYCLOCK_HPP
#define BARECPPER_STEADYCLOCK_HPP

#include <chrono>
#include <cstdint>

#include "RealtimeCounter.hpp"

namespace BareCpper {
    
    /** Monotonic clock using RealtimeCounter and compatible as std::chrono TrivialClock (see https://en.cppreference.com/w/cpp/named_req/TrivialClock)
    * @note The time points of this clock cannot decrease as physical time moves forward and the time between ticks of this clock is constant.
    * @note This clock is not related to wall clock time (for example, it can be time since last reboot), and is most suitable for measuring intervals.
    * 
    * @remark RealtimeCounter hardware must be initialised and started at power-on
    * @warning RealtimeCounter must not be reset/stop/started otherwise this clock will not perform in a monotonic manner
    */
    struct SteadyClock
    {
        using rep = RealtimeCounter::Count;
        using period = std::ratio<1,  RealtimeCounter::seconds(1) >;
        using duration = std::chrono::duration<rep, period>;
        using time_point = std::chrono::time_point<SteadyClock>;
        static constexpr bool is_steady = true;

        static time_point now() noexcept
        {
            return time_point{ duration{ RealtimeCounter::count() } };
        }
    };

} //END: BareCpper

#endif