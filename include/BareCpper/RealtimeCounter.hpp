#ifndef BARECPPER_CHRONO_REALTIMECOUNTER_HPP
#define BARECPPER_CHRONO_REALTIMECOUNTER_HPP

#include <cstdint> //< uint32_t
#include "MonotonicCounter.hpp"

namespace BareCpper {

    /** Wall-clock adjusted real-time clock counter (Based on MonotionicCounter with correction/skew)
    * @note The Wall-Clock time value can be internally adjusted at any time, for example due to UTC synchronization or the user changing the system's clock.
    * @note This clock is related to the wall-clock. For small phase adjustments of the clock (up to 10 minutes difference), the frequency (rate) of this clock
    * will be increased or decreased by up to 1% until both clocks show identical times. For larger adjustments (which are only to be expected when the system is
    * first installed), WallClock jumps directly to UTC and the system administrator should be warned about this unusual event.
    */
    class RealtimeCounter
    {
    public:
        using Count = std::uint32_t; ///< Runtime Counter
        using EpochCount = std::uint64_t; ///< Offset for Count==0 to J2000

        /** Count of clock tick per second
        * @param[in]  count  Count of seconds to return count for
        * @return 32768 * count
        */
        static constexpr Count seconds(const size_t count) noexcept
        {
            return MonotonicCounter::seconds(count); 
        }

    public:

        /** Initializing the RTC driver instance.
        */
        static inline void initialise() noexcept
        {
            MonotonicCounter::initialise();
        }

        /** Start the counter.
        * @warning Starts MonotonicCounter
        */
        static inline void start() noexcept
        {
            MonotonicCounter::start();
        }

        /** Adjust the time of a clock
        */
       // static inline void adjust( int32_t nanosecondsPerTick) noexcept;

        /** Stop the counter.
        * @warning Stops MonotonicCounter
        */
        static inline void stop() noexcept
        {
            MonotonicCounter::stop();
        }

        /** Retrieve the counter value.
         * @details The counter increments by N ticks-per-second where N == MonotonicCounter::seconds(1)
         */
        static inline Count count() noexcept
        { 
            return MonotonicCounter::count() + countOffset_; 
        }

        static inline EpochCount epochOffset() noexcept
        { 
            return epochOffset_;
        }

    private:
        static int32_t countOffset_;
        static EpochCount epochOffset_;
    };

} //END: BareCpper

#endif