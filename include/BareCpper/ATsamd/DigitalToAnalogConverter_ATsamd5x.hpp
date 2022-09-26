#ifndef DIGITAL_TO_ANALOG_CONVERTER_SAMD51_HPP
#define DIGITAL_TO_ANALOG_CONVERTER_SAMD51_HPP

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <cstdint>
#include <sam.h>
#if __has_include(<component-version.h>) 
#include <component-version.h> //< Atmel SAM version defines
#else
#define COMPONENT_VERSION_MAJOR 1 //< Arduino has v1 headers
#endif

namespace BareCpper
{
	namespace SAMD51
	{
    enum class DacReference : uint8_t
    {
      UnbufferedExternalReference = 0x0,
      VoltageSupply               = 0x1,
      BufferedExternalReference   = 0x2,
      InternalReference           = 0x3
    };

    enum class DacOversamplingRatio : uint8_t
    {
      Osr1  = 0x0,
      Osr2  = 0x1,
      Osr4  = 0x2,
      Osr8  = 0x3,
      Osr16 = 0x4,
      Osr32 = 0x5
    };

    enum class DacCurrentControl : uint8_t
    {
      Cc100k  = 0x0,
      Cc1M    = 0x1,
      Cc12M   = 0x2
    };

    class DAC
    {
      public:
        void init()
        {
          // enable clocks for DAC
          MCLK->APBDMASK.reg |= MCLK_APBDMASK_DAC;
          // ensure GCLK4 is configured for 12 MHz clock output
					GCLK->PCHCTRL[DAC_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK4_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
          // setup current control
          DAC->DACCTRL[0].reg = static_cast<uint8_t>(DacCurrentControl::Cc12M) << DAC_DACCTRL_CCTRL_Pos;
          DAC->DACCTRL[1].reg = static_cast<uint8_t>(DacCurrentControl::Cc12M) << DAC_DACCTRL_CCTRL_Pos;
          // setup voltage reference (must be set to VDDANA - refer to errata)
          DAC->CTRLB.reg |= static_cast<DacReference::VoltageSupply> << DAC_CTRLB_REFSEL_Pos;
        }

        void enable()
        {
          DAC->CTRLA.bit.ENABLE = 1;
          while(DAC->SYNCBUSY.bit.ENABLE);
        }

        void disable()
        {
          DAC->CTRLA.bit.ENABLE = 0;
          while(DAC->SYNCBUSY.bit.ENABLE);
        }

        void reset()
        {
          DAC->CTRLA.bit.SWRST = 1;
          while(DAC->SYNCBUSY.bit.SWRST);
        }

        template<uint8_t DacChannel>
        void configureChannel(const DacOversamplingRatio& osr = DacOversamplingRatio::Osr1
                            , const uint8_t refreshPeriod = 0
                            , const bool dither = false
                            , const bool externalFilter = false
                            , const bool leftAdjustment = false)
        {
          static_assert(DacChannel < 2, "There are only two dac channels available, 0 and 1");
          if(refreshPeriod > 15) refreshPeriod = refreshPeriod % 16; //< refresh period must be between 0 and 15
          uint8_t dacCtrl = DAC->DACCTRL[DacIndex].reg;
          // set oversampling
          dacCtrl &= ~DAC_DACCTRL_OSR_Msk;
          dacCtrl |= static_cast<uint8_t>(osr) << DAC_DACCTRL_OSR_Pos;
          // refresh period
          dacCtrl &= ~DAC_DACCTRL_REFRESH_Msk;
          dacCtrl |= static_cast<uint8_t>(refreshPeriod) << DAC_DACCTRL_REFRESH_Pos;
          // dither
          if(dither) dacCtrl |= DAC_DACCTRL_DITHER;
          else dacCtrl &= ~DAC_DACCTRL_DITHER;
          // filter
          if(externalFilter) dacCtrl |= DAC_DACCTRL_FEXT;
          else dacCtrl &= ~DAC_DACCTRL_FEXT;
          // left adjustment
          if(leftAdjustment) dacCtrl |= DAC_DACCTRL_LEFTADJ;
          else dacCtrl &= ~DAC_DACCTRL_LEFTADJ;
          DAC->DACCTRL[DacIndex].reg = dacCtrl;
        }

        template<uint8_t DacChannel>
        void enableChannel()
        {
          static_assert(DacChannel < 2, "There are only two dac channels available, 0 and 1");
          DAC->DACCTRL[DacChannel].reg |= DAC_DACCTRL_ENABLE;

          // config pins
        }

        template<uint8_t DacChannel>
        void disableChannel()
        {
          static_assert(DacChannel < 2, "There are only two dac channels available, 0 and 1");
          DAC->DACCTRL[DacChannel].reg &= ~DAC_DACCTRL_ENABLE;

          // config pins
        }
    };
  }
}

#endif //DIGITAL_TO_ANALOG_CONVERTER_SAMD51_HPP