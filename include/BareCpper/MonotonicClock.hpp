#ifndef BARECPPER_CHRONO_MONOTONICCLOCK_HPP
#define BARECPPER_CHRONO_MONOTONICCLOCK_HPP

#include <chrono>
#include <cstdint>

#include "MonotonicCounter.hpp"

namespace BareCpper {

    /** Clocks for std::chrono providing a flexible collection of types that track time with varying degrees of precision
    * @see https://en.cppreference.com/w/cpp/chrono
    */
    namespace Chrono
    {

        /** Monotonic clock using MonotonicCounter and compatible as std::chrono TrivialClock (see https://en.cppreference.com/w/cpp/named_req/TrivialClock)
        * @note The time points of this clock cannot decrease as physical time moves forward and the time between ticks of this clock is constant.
        * @note This clock is not related to wall clock time (for example, it can be time since last reboot), and is most suitable for measuring intervals.
        *
        * @see Posix CLOCK_MONOTONIC http://www.qnx.com/developers/docs/7.0.0/index.html#com.qnx.doc.neutrino.getting_started/topic/s1_timer_CLOCK_MONOTONIC.html
        *
        * @remark MonotonicCounter hardware must be initialised and started at power-on
        * @warning MonotonicCounter must not be reset/stop/started otherwise this clock will not perform in a monotonic manner
        */
        struct MonotonicClock
        {
            using rep = MonotonicCounter::Count;
            using period = std::ratio<1, MonotonicCounter::seconds(1) >;
            using duration = std::chrono::duration<rep, period>;
            using time_point = std::chrono::time_point<MonotonicClock>;
            static constexpr bool is_steady = true;

            static time_point now() noexcept
            {
                return time_point{duration{ MonotonicCounter::count() }};
            }
        };

    } //END: Chrono
} //END: BareCpper

#endif