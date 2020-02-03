#pragma once

#ifndef BARECPPER_RTCTIMER_H_
#  error "Include <BareCpper/RtcTimer.hpp> instead of this file."
#endif

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include "sam.h"

namespace BareCpper {

    constexpr uint32_t RtcTimer::ticksPerSecond()
    {
        return 32768U;
    }

    void RtcTimer::initialise(void);

    /** Start the counter. */
    void RtcTimer::start(void)
    {
        RTC->MODE0.CTRLA.bit.ENABLE = 1;
        while (RTC->MODE0.SYNCBUSY.bit.ENABLE);// wait for RTC settings for COUNTSYNC and ENABLE to be synced
    }

    void RtcTimer::stop(void)
    {
        RTC->MODE0.CTRLA.bit.ENABLE = 0;
        while (RTC->MODE0.SYNCBUSY.bit.ENABLE); // wait for RTC settings for COUNTSYNC and ENABLE to be synced
    }

    uint32_t RtcTimer::count(void)
    {
        while (RTC->MODE0.SYNCBUSY.bit.COUNT);                    // wait for COUNT sync before reading per SAMD5xE5x Family Datasheet...
        return RTC->MODE0.COUNT.reg;                                              //  then return it.
    }

} //END: BareCpper