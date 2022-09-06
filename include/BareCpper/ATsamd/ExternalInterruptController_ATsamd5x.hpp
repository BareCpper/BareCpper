#ifndef EXTERNAL_INTERRUPT_CONTROLLER_SAMD51_HPP
#define EXTERNAL_INTERRUPT_CONTROLLER_SAMD51_HPP

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <functional>

#include "../Gpio.hpp"
#include <sam.h>

namespace BareCpper
{
  namespace SAMD51
  {
    /**
     * Edge or level configuration for
     * the external interrupt/event
     * 
     */
    enum class ExternalInterruptSense : uint8_t
    {
      None  = 0x0,
      Rise  = 0x1,
      Fall  = 0x2,
      Both  = 0x3,
      High  = 0x4,
      Low   = 0x5
    };

    enum class ExternalInterruptDebouncer : uint8_t
    {
      Off = 0xFF,
      Prescaler2    = 0x0,
      Prescaler4    = 0x1,
      Prescaler8    = 0x2,
      Prescaler16   = 0x3,
      Prescaler32   = 0x4,
      Prescaler64   = 0x5,
      Prescaler128  = 0x6,
      Prescaler256  = 0x7,
    };

    class ExternalInterruptController
    {
      public:
        static constexpr uint8_t NumberChannels = 16;
        /**
         * Initialize the EIC
         * 
         */
        void init()
        {
          // EIC MCLK is enabled by default
          // Use GCLK1 for EIC (max is 100 MHz)
          GCLK->PCHCTRL[EIC_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
          // Wait for clock synchronization
				  while (GCLK->SYNCBUSY.reg);
          // Reset the EIC
          reset();
          // Enable the EIC
          enable();
        }

        using CallbackT = std::function<void()>;
        
        template<typename PinT>
        void attachInterrupt(const PinT& pin
                            , const CallbackT& callback
                            , const ExternalInterruptSense& mode
                            , const ExternalInterruptDebouncer& debounce = ExternalInterruptDebouncer::Off)
        {
          // registers are enable-protected, disable EIC before configuring if it is enabled
          const bool shouldRestart = EIC->CTRLA.reg & EIC_CTRLA_ENABLE;
          if(shouldRestart) disable();
          // set the Pin alternative function to peripheral A (EIC)
          BareCpper::gpioFunction<PinT>(pin, std::optional<ATsamd5x::Peripheral>{ATsamd5x::Peripheral::A});
          // configure the EIC sense mode
          constexpr auto channel = (PinT::id().pin > 15) ? (PinT::id().pin - 16) : PinT::id().pin;
          constexpr uint8_t configRegNum = (channel > 7);
          constexpr uint8_t configRegShift = (channel - 8*configRegNum) << 2; //< channel config starts at bit 4^channel
          EIC->CONFIG[configRegNum].reg &= ~(0x7 << configRegShift);
          EIC->CONFIG[configRegNum].reg |= (static_cast<uint8_t>(mode) << configRegShift);
          // configure debouncer
          if(debounce == ExternalInterruptDebouncer::Off)
          {
            EIC->DEBOUNCEN.reg &= ~(0x1 << channel);
          }
          else
          {
            constexpr uint8_t dpresclareRegShift = configRegNum << 2;  //< dpreslacer for pins 0-7 start at bit 0, for pins 8-15 at bit 4 (4^n)
            // set debounce prescaler
            EIC->DPRESCALER.reg &= ~(0x7 << dpresclareRegShift);
            EIC->DPRESCALER.reg |= (static_cast<uint8_t>(debounce) << dpresclareRegShift);
            EIC->DEBOUNCEN.reg |= (0x1 << channel);
          }
          // enable interrupt in NVIC
          constexpr auto irqNumber = getIrqNumber(channel);
          NVIC_ClearPendingIRQ(irqNumber);
          NVIC_SetPriority(irqNumber, 0);
          NVIC_EnableIRQ(irqNumber);
          // enable interrupt in EIC
          EIC->INTENSET.reg = (0x1 << channel);
          // set callback
          ExternalInterruptController::callbacks_[channel] = callback;
          // reenable EIC
          if(shouldRestart) enable();
        }

        template<typename PinT>
        void detachInterrupt(const PinT& pin)
        {
          // registers are enable-protected, disable EIC before configuring if it is enabled
          const bool shouldRestart = EIC->CTRLA.reg & EIC_CTRLA_ENABLE;
          if(shouldRestart) disable();
          constexpr auto channel = PinT::id().pin;
          constexpr auto irqNumber = getIrqNumber(channel);
          // disable interrupt in EIC
          EIC->INTENCLR.reg = (0x1 << channel);
          // disable interrupt in NVIC
          NVIC_DisableIRQ(irqNumber);
          ExternalInterruptController::callbacks_[channel] = nullptr;
          // reenable EIC
          if(shouldRestart) enable();
        }

        /**
         * Enable the EIC
         * 
         */
        void enable()
        {
          // enable EIC
          EIC->CTRLA.reg |= EIC_CTRLA_ENABLE;
          // wait for sync
          while(EIC->SYNCBUSY.reg & EIC_SYNCBUSY_ENABLE);
        }

        /**
         * Disable the EIC
         * 
         */
        void disable()
        {
          // disable EIC
          EIC->CTRLA.reg &= ~EIC_CTRLA_ENABLE;
          // wait for sync
          while(EIC->SYNCBUSY.reg & EIC_SYNCBUSY_ENABLE);
        }

        /**
         * Software reset the EIC
         * 
         */
        void reset()
        {
          EIC->CTRLA.reg |= EIC_CTRLA_SWRST;
          while((EIC->CTRLA.reg & EIC_CTRLA_SWRST) && (EIC->SYNCBUSY.reg & EIC_SYNCBUSY_SWRST));
        }

        // callback function holders
        static CallbackT callbacks_[NumberChannels];

        // function to call inside EIC IRQ handlers
        static void irqHandler(const uint8_t channel)
        {
          // call the callback function
          if(ExternalInterruptController::callbacks_[channel]) ExternalInterruptController::callbacks_[channel];
          // clear the interrupt flag
          EIC->INTFLAG.reg |= (0x1 << channel);
        }
      private:
        static constexpr IRQn_Type getIrqNumber(const uint8_t channel)
        {
          return static_cast<IRQn_Type>(EIC_0_IRQn + channel);
        }
    };
  }
}

#endif //EXTERNAL_INTERRUPT_CONTROLLER_SAMD51_HPP