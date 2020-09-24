#pragma once

#ifndef BARECPPER_CHRONO_MONOTONICCOUNTER_HPP
#  error "Include <BareCpper/MonotonicCounter.hpp> instead of this file."
#endif

#if !NRF52
#  error "nRF52 library error, please check and update accordingly."
#endif


#include <nrf.h>

namespace BareCpper {

    constexpr MonotonicCounter::Count MonotonicCounter::seconds(const size_t duration)
    {
        return 32768U * duration;
    }

    void MonotonicCounter::initialise()
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

    void MonotonicCounter::start()
    {
        NRF_RTC2->TASKS_START = 1;
    }

    void MonotonicCounter::stop()
    {
        NRF_RTC2->TASKS_START = 0;
    }

    MonotonicCounter::Count MonotonicCounter::count()
    {
        return NRF_RTC2->COUNTER;
    }

  } //END: BareCpper