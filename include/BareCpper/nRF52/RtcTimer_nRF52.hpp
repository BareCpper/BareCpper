#pragma once

#ifndef BARECPPER_RTCTIMER_H_
#  error "Include <BareCpper/RtcTimer.hpp> instead of this file."
#endif

#if !NRF52
#  error "nRF52 library error, please check and update accordingly."
#endif


#include <nrf.h>

namespace BareCpper {

    constexpr uint32_t RtcTimer::ticksPerSecond()
    {
        return 32768U;
    }

    void RtcTimer::initialise(void)
    {}


    void RtcTimer::start(void)
    {
        NRF_RTC2->TASKS_START = 1;
    }

    void RtcTimer::stop(void)
    {
        NRF_RTC2->TASKS_START = 0;
    }

    uint32_t RtcTimer::count(void)
    {
        return NRF_RTC2->COUNTER;
    }

  } //END: BareCpper