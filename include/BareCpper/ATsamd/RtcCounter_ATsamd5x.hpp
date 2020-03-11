#pragma once

#ifndef BARECPPER_RTCCOUNTER_HPP
#  error "Include <BareCpper/RtcCounter.hpp> instead of this file."
#endif

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include "sam.h"

namespace BareCpper {

    constexpr uint32_t RtcCounter::ticksPerSecond()
    {
        return 32768U;
    }

    void RtcCounter::initialise(void)
    {
        // turn on digital interface clocks
        MCLK->APBAMASK.bit.OSC32KCTRL_ = 1;
        MCLK->APBAMASK.bit.RTC_ = 1;

#if !defined(CRYSTALLESS) // Enable XOSC32K clock(External on - board 32.768Hz oscillator)
        OSC32KCTRL->XOSC32K.reg = OSC32KCTRL_XOSC32K_ENABLE
            | OSC32KCTRL_XOSC32K_EN32K
            | OSC32KCTRL_XOSC32K_CGM_XT
            | OSC32KCTRL_XOSC32K_XTALEN;
        while (!OSC32KCTRL->INTFLAG.bit.XOSC32KRDY);

        OSC32KCTRL->RTCCTRL.bit.RTCSEL = OSC32KCTRL_RTCCTRL_RTCSEL_XOSC32K_Val; // select 32KHz clock to drive RTC...

#else // Put OSCULP32K as source of Generic
        OSC32KCTRL->XOSC32K.bit.EN32K = true;
        OSC32KCTRL->RTCCTRL.bit.RTCSEL = OSC32KCTRL_RTCCTRL_RTCSEL_ULP32K_Val; // select 32KHz clock to drive RTC...
#endif

        stop();

        //Reset
        RTC->MODE0.CTRLA.bit.SWRST = 1;
        while (RTC->MODE0.SYNCBUSY.reg & RTC_MODE0_SYNCBUSY_SWRST);

        RTC->MODE0.DBGCTRL.bit.DBGRUN = 1; // set RTC to continue running when processor is halted by debugger
        RTC->MODE0.CTRLA.reg = RTC_MODE0_CTRLA_MODE_COUNT32 // put RTC in 32 bit counter mode
            | RTC_MODE0_CTRLA_PRESCALER_DIV1 // with prescaler set to 1 to get 30.517us interval
            | RTC_MODE0_SYNCBUSY_COUNTSYNC; // and COUNTSYNC set to get stable count values on reads.

        while (RTC->MODE0.SYNCBUSY.bit.COUNTSYNC);
    }

    void RtcCounter::start(void)
    {
        RTC->MODE0.CTRLA.bit.ENABLE = 1;
        while (RTC->MODE0.SYNCBUSY.bit.ENABLE);// wait for RTC settings for COUNTSYNC and ENABLE to be synced
    }

    void RtcCounter::stop(void)
    {
        RTC->MODE0.CTRLA.bit.ENABLE = 0;
        while (RTC->MODE0.SYNCBUSY.bit.ENABLE); // wait for RTC settings for COUNTSYNC and ENABLE to be synced
    }

    uint32_t RtcCounter::count(void)
    {
        while (RTC->MODE0.SYNCBUSY.bit.COUNT);                    // wait for COUNT sync before reading per SAMD5xE5x Family Datasheet...
        return RTC->MODE0.COUNT.reg;                                              //  then return it.
    }

} //END: BareCpper