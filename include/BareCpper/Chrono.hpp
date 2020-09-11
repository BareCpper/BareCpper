#ifndef BARECPPER_CHRONO_HPP
#define BARECPPER_CHRONO_HPP


#include "MonotonicClock.hpp"
#include "RealtimeClock.hpp"

namespace BareCpper {

    /** Clocks for std::chrono providing a flexible collection of types that track time with varying degrees of precision
    * @see https://en.cppreference.com/w/cpp/chrono
    */
    namespace Chrono
    {
        /**  @see MonotonicClock 
        */
        using SteadyClock = MonotonicClock;

        /**  @see RealtimeClock
        */
        using SystemClock = RealtimeClock;
    }

} //END: BareCpper

#endif