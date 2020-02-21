#pragma once

#ifndef BARECPPER_RTCTIMER_H_
#  error "Include <BareCpper/RtcCounter.hpp> instead of this file."
#endif

#if !NRF52
#  error "nRF52 library error, please check and update accordingly."
#endif


#include <nrf.h>

namespace BareCpper {

    constexpr uint32_t RtcCounter::ticksPerSecond()
    {
        return 32768U;
    }

    void RtcCounter::initialise(void)
    {
        // Start 32 MHz crystal oscillator
        NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
        NRF_CLOCK->TASKS_LFCLKSTART = 1;

        // Wait for the external oscillator to start up
        while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0) {
            // Do nothing.
        }

        NRF_RTC2->PRESCALER = 0; //32.768 / (PRESCALER + 1 )
        NRF_RTC2->CC[0] = 1;    
    }

    void RtcCounter::start(void)
    {
        NRF_RTC2->TASKS_START = 1;
    }

    void RtcCounter::stop(void)
    {
        NRF_RTC2->TASKS_START = 0;
    }

    uint32_t RtcCounter::count(void)
    {
        return NRF_RTC2->COUNTER;
    }

  } //END: BareCpper