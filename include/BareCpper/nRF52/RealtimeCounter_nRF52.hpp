#pragma once

#ifndef BARECPPER_REALTIMECOUNTER_HPP
#  error "Include <BareCpper/RealtimeCounter.hpp> instead of this file."
#endif

#if !NRF52
#  error "nRF52 library error, please check and update accordingly."
#endif


#include <nrf.h>

namespace BareCpper {

    constexpr RealtimeCounter::Count RealtimeCounter::seconds(const size_t count)
    {
        return 32768U * count;
    }

    void RealtimeCounter::initialise()
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

    void RealtimeCounter::start()
    {
        NRF_RTC2->TASKS_START = 1;
    }

    void RealtimeCounter::stop()
    {
        NRF_RTC2->TASKS_START = 0;
    }

    RealtimeCounter::Count RealtimeCounter::count()
    {
        return NRF_RTC2->COUNTER;
    }

  } //END: BareCpper