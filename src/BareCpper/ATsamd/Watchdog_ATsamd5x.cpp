#if ARDUINO
#include "BareCpper/include/BareCpper/ATsamd/Watchdog_ATsamd5x.hpp"
#else
#include "BareCpper/ATsamd/Watchdog_ATsamd5x.hpp"
#endif
#include <algorithm>

namespace BareCpper
{
  namespace SAMD51
  {
    void Watchdog::init(const uint32_t intervalUs)
    {
      // APBA clock for WDT is enabled by default
      const bool shouldRestart = WDT->CTRLA.bit.ENABLE;
      if(shouldRestart) stop();  //< stop watchdog if it is running
      // watchdog clk has frequency of 1024 Hz
      const uint32_t cycles = 1024 * (intervalUs / 1000000.0f);
      const uint8_t cyclesClockShift = 32 - __builtin_clz( std::min(cycles, uint32_t(16384U)) / 8 );

      WDT->CONFIG.bit.PER = cyclesClockShift + uint8_t(cyclesClockShift == 0);  //< set period for WDT issued reset
      WDT->EWCTRL.bit.EWOFFSET = cyclesClockShift - uint8_t(cyclesClockShift != 0);  //< set number of cycles between start od WDT timeout and 'early warning' IRQ

      WDT->INTENSET.bit.EW = 1;  //< enable 'early warning' interrupt
      WDT->CTRLA.bit.WEN = 0;  //< disable window mode    


      if(shouldRestart) start();  //< start watchdog if it was running
    }

    void Watchdog::start()
    {
      WDT->CLEAR.reg = WDT_CLEAR_CLEAR_KEY; //< clear watchdog interval	
      WDT->CTRLA.bit.ENABLE = 1;  //< start watchdog
      // while(!WDT->SYNCBUSY.bit.ENABLE);  //< don't wait for synchronization here, takes too long

      // enable WDT interrupt
      NVIC_SetPriority(WDT_IRQn, 0);
      NVIC_ClearPendingIRQ(WDT_IRQn);
      NVIC_EnableIRQ(WDT_IRQn);
    }

    void Watchdog::stop()
    {
      // disable WDT interrupt
      NVIC_DisableIRQ(WDT_IRQn);
      NVIC_ClearPendingIRQ(WDT_IRQn);
      
      WDT->CTRLA.bit.ENABLE = 0;  //< Stop watchdog
      // while(WDT->SYNCBUSY.bit.ENABLE);  //< don't wait for synchronization here, takes too long
    }

    void Watchdog::feed()
    {
      // if clear operation is not pending, clear the WDT
      if (!WDT->SYNCBUSY.bit.CLEAR)
      {
        WDT->CLEAR.bit.CLEAR = WDT_CLEAR_CLEAR_KEY; // clear watchdog
      }
    }
  }
}

// WDT Handler
extern "C" void WDT_Handler(void)
{
  // clear the 'Early Warning' interrupt flag
  WDT->INTFLAG.bit.EW = 1;
}