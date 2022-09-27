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

#include "../Gpio.hpp"

namespace BareCpper
{
	namespace SAMD51
	{
    /**
     * DAC voltage reference.
     * @note Because of silicon bug, the reference
     * selection for VoltageSupply (VDDANA) is not working.
     * The voltage supply should be tied to VREF pin and
     * the reference selection should be UnbufferedExternalReference.
     * 
     */
    enum class DacReference : uint32_t
    {
      UnbufferedExternalReference,
      VoltageSupply,
      BufferedExternalReference,
      Internal1V0                 = SUPC_VREF_SEL_1V0_Val,
			Internal1V1                 = SUPC_VREF_SEL_1V1_Val,
			Internal1V2                 = SUPC_VREF_SEL_1V2_Val,
			Internal1V25                = SUPC_VREF_SEL_1V25_Val,
			Internal2V0                 = SUPC_VREF_SEL_2V0_Val,
			Internal2V2                 = SUPC_VREF_SEL_2V2_Val,
			Internal2V4                 = SUPC_VREF_SEL_2V4_Val,
			Internal2V5                 = SUPC_VREF_SEL_2V5_Val,
    };

    /**
     * Possible oversampling DAC ratios.
     * Multiplies the sample rate.
     * 
     */
    enum class DacOversamplingRatio : uint8_t
    {
      Osr1  = 0x0,
      Osr2  = 0x1,
      Osr4  = 0x2,
      Osr8  = 0x3,
      Osr16 = 0x4,
      Osr32 = 0x5
    };

    /**
     * Set output buffer current based on
     * DAC clock.
     * 
     */
    enum class DacCurrentControl : uint8_t
    {
      Cc100k  = 0x0,
      Cc1M    = 0x1,
      Cc12M   = 0x2
    };

    /**
     * DAC controller interface.
     * Controls the two DACs inside the MCU.
     * 
     */
    class DigitalToAnalogConverter
    {
      public:
        static constexpr uint16_t maxValue = 4095;  //< 12-bit resolution

        /**
         * Initialize the DAC by selecting the clock source
         * and setting the correct current control values and voltage reference
         */
        void init()
        {
          // enable clocks for DAC
          MCLK->APBDMASK.reg |= MCLK_APBDMASK_DAC;
          // ensure GCLK4 is configured for 12 MHz clock output
					GCLK->PCHCTRL[DAC_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK4_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
          // Wait for clock synchronization
				  while (GCLK->SYNCBUSY.reg);
          // reset the peripheral
          reset();
          // setup current control
          DAC->DACCTRL[0].reg = static_cast<uint8_t>(DacCurrentControl::Cc12M) << DAC_DACCTRL_CCTRL_Pos;
          DAC->DACCTRL[1].reg = static_cast<uint8_t>(DacCurrentControl::Cc12M) << DAC_DACCTRL_CCTRL_Pos;
        }

        /**
         * Enable the DAC controller and wait for the
         * startup time of the DACs to pass.
         * 
         */
        void enable()
        {
          DAC->CTRLA.bit.ENABLE = 1;
          while(DAC->SYNCBUSY.bit.ENABLE);
          // wait for the startup time to finish for both DACs
          ///@note due to silicon bug, check INTFLAG EMPTY 0 and EMPTY 1
          while(DAC->INTFLAG.bit.EMPTY0);
          while(DAC->INTFLAG.bit.EMPTY1);
        }

        /**
         * Disable the DAC controller.
         * 
         */
        void disable()
        {
          DAC->CTRLA.bit.ENABLE = 0;
          while(DAC->SYNCBUSY.bit.ENABLE);
        }

        /**
         * Software reset the DAC registers
         * to their reset values.
         */
        void reset()
        {
          DAC->CTRLA.bit.SWRST = 1;
          while(DAC->SYNCBUSY.bit.SWRST);
        }

        /**
         * @brief Set the Voltage Reference to be used by the DACs.
         * 
         * @param ref The requested voltage reference.
         * @param externalRefValue If using external voltage reference, its voltage.
         */
        void setVoltageReference(const DacReference& ref, const float externalRefValue = 0.0f)
        {
          DAC->CTRLB.reg &= ~DAC_CTRLB_REFSEL_Msk;
          if(ref == DacReference::UnbufferedExternalReference)
          {
            voltageReference_ = 3.3f;
          }
          else if(ref == DacReference::VoltageSupply)
          {
            // due to silicon error, VDDANA should be
            // connected to VREF and REFSEL should be 0b00
            voltageReference_ = externalRefValue;
          }
          else if(ref == DacReference::BufferedExternalReference)
          {
            DAC->CTRLB.reg |= DAC_CTRLB_REFSEL(DAC_CTRLB_REFSEL_VREFPB_Val);
            voltageReference_ = externalRefValue;
          }
          else
          {
            ///@todo separate SUPC class, SUPC is shared between ADC, DAC and temp sensor
            // enable and configure SUPC
            // switch SUPC VREF in on-demand mode, so VREF can work together with temp sensors
            SUPC->VREF.bit.ONDEMAND = 1;
            // set the internal voltage reference voltage
            SUPC->VREF.bit.SEL = static_cast<uint32_t>(ref);
            // route it to the DAC
            SUPC->VREF.bit.VREFOE = 1;
            // internal reference
            DAC->CTRLB.reg |= DAC_CTRLB_REFSEL(DAC_CTRLB_REFSEL_INTREF_Val);
            switch(ref)
            {
              case DacReference::Internal1V0:
                voltageReference_ = 1.0f;
              break;
              case DacReference::Internal1V1:
                voltageReference_ = 1.1f;
              break;
              case DacReference::Internal1V2:
                voltageReference_ = 1.2f;
              break;
              case DacReference::Internal1V25:
                voltageReference_ = 1.25f;
              break;
              case DacReference::Internal2V0:
                voltageReference_ = 2.0f;
              break;
              case DacReference::Internal2V2:
                voltageReference_ = 2.2f;
              break;
              case DacReference::Internal2V4:
                voltageReference_ = 2.4f;
              break;
              case DacReference::Internal2V5:
                voltageReference_ = 2.5f;
              break;
              default:
              break;
            }
            
          }
        }

        /**
         * @brief Configure the specified DAC instance.
         * @note There are two DACs inside the MCU.
         * 
         * @tparam DacIndex The index of the DAC.
         * @param osr Oversampling option.
         * @param refreshPeriod Refresh the output of the DAC
         * by starting conversion after this value * (30 us).
         * Must be used for lower sample rates because the DAC
         * can maintain output for maximum of ~100us.
         * @param dither If enabled, dithering mode can minimize quantization
         * error, DATA[15:4] represent the value to be converted and DATA[3:0]
         * represent the dither bits.
         * @param externalFilter If set to false, DAC takes the SINC filter output.
         * If set to true, RESULT reg contains the interpolated data.
         */
        template<uint8_t DacIndex>
        void configureChannel(const DacOversamplingRatio& osr = DacOversamplingRatio::Osr1
                            , const uint8_t refreshPeriod = 0
                            , const bool dither = false
                            , const bool externalFilter = false)
        {
          static_assert(DacIndex < 2, "There are only two dac channels available, 0 and 1");
          const bool shouldRestart = DAC->CTRLA.bit.ENABLE;
          // disable DAC controller if it is enabled
          if(shouldRestart) disable();
          // enable channel
          enableChannel<DacIndex>();
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
          DAC->DACCTRL[DacIndex].reg = dacCtrl;
          // reenable DAC controller if it was enabled before
          if(shouldRestart) enable();
        }

        /**
         * @brief Enable specified DAC instance and configure the
         * pin connected to DAC.
         * 
         * @tparam DacIndex The DAC instance (0 or 1)
         */
        template<uint8_t DacIndex>
        void enableChannel()
        {
          static_assert(DacIndex < 2, "There are only two dac channels available, 0 and 1");
          DAC->DACCTRL[DacIndex].reg |= DAC_DACCTRL_ENABLE;

          // config pins
          // PA02(DAC0) and PA05(DAC1)
          if constexpr(DacIndex == 0)
          {
            BareCpper::gpioFunction<PA02>(PA02{}, std::optional<ATsamd5x::Peripheral>{ATsamd5x::Peripheral::B});
          }
          else
          {
            BareCpper::gpioFunction<PA05>(PA05{}, std::optional<ATsamd5x::Peripheral>{ATsamd5x::Peripheral::B});
          }
        }

        /**
         * @brief Disable specified DAC instance and release
         * the pin associated with the DAC instance.
         * 
         * @tparam DacIndex The DAC instance (0 or 1)
         */
        template<uint8_t DacIndex>
        void disableChannel()
        {
          static_assert(DacIndex < 2, "There are only two dac channels available, 0 and 1");
          DAC->DACCTRL[DacIndex].reg &= ~DAC_DACCTRL_ENABLE;

          // config pins
          // PA02(DAC0) and PA05(DAC1)
          // disable pmuxen
          if constexpr(DacIndex == 0)
          {
            BareCpper::gpioFunction<PA02>(PA02{}, std::nullopt);
          }
          else
          {
            BareCpper::gpioFunction<PA05>(PA05{}, std::nullopt);
          }
        }

        /**
         * @brief Write data to be converted by the DAC and wait
         * for synchronization.
         * 
         * @tparam DacIndex The DAC instance (0 or 1)
         * @param sample The data for the DAC.
         * @param dither If dithering is used, the value of the dither bits.
         */
        template<uint8_t DacIndex>
        void writeDataSync(const uint16_t sample, const uint16_t dither = 0)
        {
          static_assert(DacIndex < 2, "There are only two dac channels available, 0 and 1");
          if(sample > maxValue) sample = maxValue;
          if(DAC->DACCTRL[DacIndex].bit.DITHER)
          {
            dither = dither % 16;  //< dither must be 4 bits wide (0 to 15)
            DAC->DATA[DacIndex].reg = (sample << 4) | dither;
          }
          else
          {
            DAC->DATA[DacIndex].reg = sample;
          }
          if constexpr(DacIndex == 0)
          {
            while(DAC->SYNCBUSY.bit.DATA0);
          }
          else
          {
            while(DAC->SYNCBUSY.bit.DATA1);
          }
        }

        /**
         * @brief Set the voltage on the specified
         * DAC instance output and wait for synchornization.
         * 
         * @tparam DacIndex The DAC instance (0 or 1) 
         * @param voltage The voltage to be set, in volts.
         */
        template<uint8_t DacIndex>
        void setVoltageSync(const float voltage)
        {
          static_assert(DacIndex < 2, "There are only two dac channels available, 0 and 1");
          // clamp the voltage between 0 and VREF
          if(voltage < 0.0f) voltage = 0.0f;
          if(voltage > voltageReference_) voltage = voltageReference_;
          writeDataSync<DacIndex>(static_cast<uint16_t>(maxValue*voltage/voltageReference_));  //< maybe use std::round
        }

         /**
         * @brief Write data to be converted by the DAC and don't
         * wait for synchronization.
         * 
         * @tparam DacIndex The DAC instance (0 or 1)
         * @param sample The data for the DAC.
         * @param dither If dithering is used, the value of the dither bits.
         */
        template<uint8_t DacIndex>
        void writeDataAsync(const uint16_t sample, const uint16_t dither = 0)
        {
          static_assert(DacIndex < 2, "There are only two dac channels available, 0 and 1");
          if(sample > maxValue) sample = maxValue;
          if(DAC->DACCTRL[DacIndex].bit.DITHER)
          {
            dither = dither % 16;  //< dither must be 4 bits wide (0 to 15)
            DAC->DATA[DacIndex].reg = (sample << 4) | dither;
          }
          else
          {
            DAC->DATA[DacIndex].reg = sample;
          }
        }

        /**
         * @brief Set the voltage on the specified
         * DAC instance output and don't wait for synchronization.
         * 
         * @tparam DacIndex The DAC instance (0 or 1) 
         * @param voltage The voltage to be set, in volts.
         */
        template<uint8_t DacIndex>
        void setVoltageAsync(const float voltage)
        {
          static_assert(DacIndex < 2, "There are only two dac channels available, 0 and 1");
          // clamp the voltage between 0 and VREF
          if(voltage < 0.0f) voltage = 0.0f;
          if(voltage > voltageReference_) voltage = voltageReference_;
          writeDataAsync<DacIndex>(static_cast<uint16_t>(maxValue*voltage/voltageReference_));  //< maybe use std::round
        }
      private:
        float voltageReference_ = 1e6f;  //< big value so when dividing and casting gives 0 value to write in data reg, if vref is invalid
    };
  }
}

#endif //DIGITAL_TO_ANALOG_CONVERTER_SAMD51_HPP