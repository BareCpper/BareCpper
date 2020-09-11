#ifndef BARECPPER_CHRONO_REALTIMECLOCK_HPP
#define BARECPPER_CHRONO_REALTIMECLOCK_HPP

#include "RealtimeCounter.hpp"

#include <cstdint> //< std::uint64_t
#include <ratio> //< std::ratio
#include <chrono> //< std::chrono::duration

namespace BareCpper {

    /** Clocks for std::chrono providing a flexible collection of types that track time with varying degrees of precision
    * @see https://en.cppreference.com/w/cpp/chrono
    */
    namespace Chrono
    {
        /** System Wall-clock using RealtimeCounter and compatible as std::chrono TrivialClock (see https://en.cppreference.com/w/cpp/named_req/TrivialClock)
        * @note The Wall-Clock time value can be internally adjusted at any time, for example due to UTC synchronization or the user changing the system's clock.
        * @note This clock is related to the wall-clock. For small phase adjustments of the clock (up to 10 minutes difference), the frequency (rate) of this clock
        * will be increased or decreased by up to 1% until both clocks show identical times. For larger adjustments (which are only to be expected when the system is
        * first installed), WallClock jumps directly to UTC and the system administrator should be warned about this unusual event.
        *
        * @see Posix CLOCK_REALTIME
        */
        struct RealtimeClock
        {
            using rep = RealtimeCounter::Count;
            using epoch_rep = std::uint64_t;

            using period = std::ratio<1, RealtimeCounter::seconds(1) >; ///< @note Sub-second resolution i.e. 32768 ticks/second

            using duration = std::chrono::duration<rep, period>; ///< Epoch expressed as time from runtime-defined epoch at start-up from RealtimeClock::epoch()
            using epoch_duration = std::chrono::duration<epoch_rep, period>; ///< Epoch expressed as time from J2000 (2000-01-01)

            using time_point = std::chrono::time_point<RealtimeClock, duration>;
            using epoch_time_point = std::chrono::time_point<RealtimeClock, epoch_duration>;

            static constexpr bool is_steady = false;

            static time_point now() noexcept
            {
                return time_point{ duration{ RealtimeCounter::count() } };
            }

            /** Offset from Epoch of J2000 (2000-01-01)
            * @note This offset is used to convert time_point returned by now() to epoch_time_point for J2000
            */
            static epoch_time_point epochOffset() noexcept
            {
                return epoch_time_point{epoch_duration{ RealtimeCounter::epochOffset() }};
            }

            /** Convert a time_point to an epoch_time_point
            */
            static epoch_time_point toEpoch( const time_point& timePoint) noexcept
            {
                return epochOffset() + timePoint.time_since_epoch();
            }
        };
    }

} //END: BareCpper

#endif