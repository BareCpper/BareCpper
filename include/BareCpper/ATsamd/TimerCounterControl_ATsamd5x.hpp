#ifndef TIMER_COUNTER_CONTROL_SAMD51_HPP
#define TIMER_COUNTER_CONTROL_SAMD51_HPP

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <array>
#include <cstdint>
#include <limits>
#include <functional>

#include <sam.h>

#define GCLK1_HZ 48000000U  //< Clock generator 1 frequency, in Hz

namespace BareCpper
{
	namespace SAMD51
	{
    static constexpr size_t numTccDevices = TCC_INST_NUM;
		/**
		 * @brief Get requested MCU TimerCounter instance
		 * @param tccIndex The index of the TCC instance
		 * @return Pointer to the requested TC instance
		*/
		inline Tcc* tccInstance(const uint8_t tccIndex)
		{
			if (tccIndex >= numTccDevices)
			{
				return nullptr;
			}
			std::array tccDevices = TCC_INSTS;
			return tccDevices[tccIndex];
		}

		inline uint8_t tccNumberCCchannels(const uint8_t tccIndex)
		{
			if (tccIndex >= numTccDevices)
			{
				return 0;
			}
			std::array numCCchannels = {TCC0_CC_NUM, TCC1_CC_NUM, TCC2_CC_NUM, TCC3_CC_NUM, TCC4_CC_NUM};
			return numCCchannels[tccIndex];
		}

		// Functions to be called when timer overflow or match/capture interrupts fire
		extern std::array<std::function<void()>, numTccDevices> TCC_OverflowCallback;
    extern std::array<std::array<std::function<void()>, 5>, numTccDevices> TCC_McCallback;

		enum class TimerCounterControlWaveformMode : uint8_t
		{
			NFRQ        = 0x0,
			MFRQ        = 0x1,
			NPWM        = 0x2,
			DSCRITICAL  = 0x4,
      DSBOTTOM    = 0x5,
      DSBOTH      = 0x6,
      DSTOP       = 0x7
		};

		enum class TimerCounterControlEventInput : uint8_t
		{
			Mc0 			= TCC_EVCTRL_MCEI0_Pos,
			Mc1				= TCC_EVCTRL_MCEI1_Pos,
			Mc2				= TCC_EVCTRL_MCEI2_Pos,
			Mc3 			= TCC_EVCTRL_MCEI3_Pos,
			Mc4 			= TCC_EVCTRL_MCEI4_Pos,
			Mc5 			= TCC_EVCTRL_MCEI5_Pos,
			Ev0				= TCC_EVCTRL_TCEI0_Pos,
			Ev1				= TCC_EVCTRL_TCEI1_Pos,
			EvAll,
			McAll
		};

		enum class TimerCounterControlEventOutput : uint8_t
		{
			Cycle 				= TCC_EVCTRL_CNTEO_Pos,
			Retrigger			=	TCC_EVCTRL_TRGEO_Pos,
			Overflow 			= TCC_EVCTRL_OVFEO_Pos,
			Mc0 					= TCC_EVCTRL_MCEO0_Pos, 
			Mc1 					= TCC_EVCTRL_MCEO1_Pos,
			Mc2 					= TCC_EVCTRL_MCEO2_Pos,
			Mc3 					= TCC_EVCTRL_MCEO3_Pos,
			Mc4 					= TCC_EVCTRL_MCEO4_Pos,
			Mc5 					= TCC_EVCTRL_MCEO5_Pos
		};

		///@todo support other input event actions
		enum class TimerCounterControlEventAction : uint8_t
		{
			Off 			= 0x0,
			Retrigger	= 0x1,
		};

		/**
		 * @brief Timer/Counter for Control Applications peripheral functionalities for SAMD51
		 * @tparam TCcindex The TC peripheral instance
		*/
		template <uint8_t TccIndex>
		class TimerCounterControl
		{
			Tcc* const tccInstance_ = tccInstance(TccIndex);
		public:
      TimerCounterControl(const TimerCounterControl &) = delete;
      TimerCounterControl(TimerCounterControl &&) = delete;
      TimerCounterControl& operator=(const TimerCounterControl &) = delete;
      TimerCounterControl&& operator=(TimerCounterControl &&) = delete;

      static TimerCounterControl& getInstance()
      {
        static TimerCounterControl tcc;
        return tcc;
      }

			void init(uint32_t periodUs, const TimerCounterControlWaveformMode &mode = TimerCounterControlWaveformMode::NFRQ)
			{
				// Allow writing to TCC registers
				// Select generic clock generator 1 (48 MHz) for ADC
				if constexpr (TccIndex == 0)
				{
					MCLK->APBBMASK.reg |= MCLK_APBBMASK_TCC0;
					GCLK->PCHCTRL[TCC0_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
				}
				else if constexpr (TccIndex == 1)
				{
					MCLK->APBBMASK.reg |= MCLK_APBBMASK_TCC1;
					GCLK->PCHCTRL[TCC1_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
				}
				else if constexpr (TccIndex == 2)
				{
					MCLK->APBCMASK.reg |= MCLK_APBCMASK_TCC2;
					GCLK->PCHCTRL[TCC2_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
				}
				else if constexpr (TccIndex == 3)
				{
					MCLK->APBCMASK.reg |= MCLK_APBCMASK_TCC3;
					GCLK->PCHCTRL[TCC3_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
				}
        else if constexpr (TccIndex == 4)
        {
          MCLK->APBDMASK.reg |= MCLK_APBDMASK_TCC4;
					GCLK->PCHCTRL[TCC4_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
        }
				// wait for clock synchronization
				while (GCLK->SYNCBUSY.reg);
				// set waveform mode
				setWaveformMode(mode);
				setPeriod(periodUs);
			}

      void reset()
      {
        tccInstance_->CTRLA.bit.SWRST = 1;
        while(tccInstance_->CTRLA.bit.SWRST);
      }


			void start()
			{
				tccInstance_->CTRLA.bit.ENABLE = 1;
        while(tccInstance_->SYNCBUSY.bit.ENABLE);
			}

			void stop()
			{
				tccInstance_->CTRLA.bit.ENABLE = 0;
        while(tccInstance_->SYNCBUSY.bit.ENABLE);
			}

			float getCounterValue()
			{
				// Force read synchronization of COUNT
				tccInstance_->CTRLBSET.bit.CMD = TCC_CTRLBSET_CMD_READSYNC_Val;
				while (tccInstance_->SYNCBUSY.bit.COUNT);

				// Convert the count value in us
				return (static_cast<float>(tccInstance_->COUNT.reg) * 1000000) / (prescaler_ * GCLK1_HZ);
			}

			void setPeriod(const uint32_t timeUs)
			{
        // stop the TC peripheral if it is running
        const bool shouldRestart = tccInstance_->CTRLA.bit.ENABLE;
        if (shouldRestart)
        {
          stop();
        }
        // Set prescaler based on period time
        if (timeUs > 300000)
        {
          tccInstance_->CTRLA.bit.PRESCALER = TCC_CTRLA_PRESCALER_DIV1024_Val;
          prescaler_ = 1024;
        }
        else if (80000 < timeUs && timeUs <= 300000)
        {
          tccInstance_->CTRLA.bit.PRESCALER = TCC_CTRLA_PRESCALER_DIV256_Val;
          prescaler_ = 256;
        }
        else if (20000 < timeUs && timeUs <= 80000)
        {
          tccInstance_->CTRLA.bit.PRESCALER = TCC_CTRLA_PRESCALER_DIV64_Val;
          prescaler_ = 64;
        }
        else if (10000 < timeUs && timeUs <= 20000)
        {
          tccInstance_->CTRLA.bit.PRESCALER = TCC_CTRLA_PRESCALER_DIV16_Val;
          prescaler_ = 16;
        }
        else if (5000 < timeUs && timeUs <= 10000)
        {
          tccInstance_->CTRLA.bit.PRESCALER = TCC_CTRLA_PRESCALER_DIV8_Val;
          prescaler_ = 8;
        }
        else if (2500 < timeUs && timeUs <= 5000)
        {
          tccInstance_->CTRLA.bit.PRESCALER = TCC_CTRLA_PRESCALER_DIV4_Val;
          prescaler_ = 4;
        }
        else if (1000 < timeUs && timeUs <= 2500)
        {
          tccInstance_->CTRLA.bit.PRESCALER = TCC_CTRLA_PRESCALER_DIV2_Val;
          prescaler_ = 2;
        }
        else if (timeUs <= 1000)
        {
          tccInstance_->CTRLA.bit.PRESCALER = TCC_CTRLA_PRESCALER_DIV1_Val;
          prescaler_ = 1;
        }

        // Calculate the value that needs to be stored in PER or CC0, depending on waveform mode
        const TimerCounterControlWaveformMode mode = static_cast<TimerCounterControlWaveformMode>(tccInstance_->WAVE.bit.WAVEGEN);
        if(mode == TimerCounterControlWaveformMode::NPWM)
				{
        	tccInstance_->CC[0].bit.CC = ((GCLK1_HZ / 1000000) * timeUs) / prescaler_ - 1;
				}
				else
				{
					tccInstance_->PER.bit.PER = ((GCLK1_HZ / 1000000) * timeUs) / prescaler_ - 1;
				}
        while (tccInstance_->SYNCBUSY.bit.CC0);

        // reenable the TC peripheral, if needed
        if (shouldRestart)
        {
          start();
        }
			}

			void setMatchCapture(const uint8_t chNum, const uint32_t timeUs)
			{
				if(chNum > tccNumberCCchannels(TccIndex)) return;
				tccInstance_->CC[chNum].bit.CC = ((GCLK1_HZ / 1000000) * timeUs) / prescaler_ - 1;
			}

      void setWaveformMode(const TimerCounterControlWaveformMode &mode)
      {
        tccInstance_->WAVE.bit.WAVEGEN = static_cast<uint8_t>(mode);
      }

			void enableOneShotMode(bool enable = true)
			{
				if(enable)
				{
					tccInstance_->CTRLBSET.bit.ONESHOT = 1;
					while(tccInstance_->SYNCBUSY.bit.CTRLB);
				}
				else
				{
					tccInstance_->CTRLBCLR.bit.ONESHOT = 1;
					while(tccInstance_->SYNCBUSY.bit.CTRLB);
				}
			}
			
			void enableEventInput(const TimerCounterControlEventInput &input, const TimerCounterControlEventAction &action, bool enable = true)
			{
				if(input == TimerCounterControlEventInput::EvAll)
				{
					tccInstance_->EVCTRL.bit.TCEI0 = enable;
					tccInstance_->EVCTRL.bit.TCEI1 = enable;
				}
				else if(input == TimerCounterControlEventInput::McAll)
				{
					tccInstance_->EVCTRL.bit.MCEI0 = enable;
					tccInstance_->EVCTRL.bit.MCEI1 = enable;
					tccInstance_->EVCTRL.bit.MCEI2 = enable;
					tccInstance_->EVCTRL.bit.MCEI3 = enable;
					tccInstance_->EVCTRL.bit.MCEI4 = enable;
					tccInstance_->EVCTRL.bit.MCEI5 = enable;
				}
				else
				{
					if(enable) tccInstance_->EVCTRL.reg |= 1 << static_cast<uint8_t>(input);
					else tccInstance_->EVCTRL.reg &= ~(1 << static_cast<uint8_t>(input));
				}

				// set the same action for both event inputs
				tccInstance_->EVCTRL.bit.EVACT0 = static_cast<uint8_t>(action);
				tccInstance_->EVCTRL.bit.EVACT1 = static_cast<uint8_t>(action);
			}

			void enableEventOutput(const TimerCounterControlEventOutput &event, bool enable = true)
			{
				if(enable) tccInstance_->EVCTRL.reg |= 1 << static_cast<uint8_t>(event);
				else tccInstance_->EVCTRL.reg &= ~(1 << static_cast<uint8_t>(event));
			}

			void enableOverflowInterrupt()
			{
				tccInstance_->INTENSET.bit.OVF = 1;
				if constexpr (TccIndex == 0)
				{
					NVIC_ClearPendingIRQ(TCC0_OTHER_IRQn);
					NVIC_EnableIRQ(TCC0_OTHER_IRQn);
				}
				else if constexpr (TccIndex == 1)
				{
					NVIC_ClearPendingIRQ(TCC1_OTHER_IRQn);
					NVIC_EnableIRQ(TCC1_OTHER_IRQn);
				}
				else if constexpr (TccIndex == 2)
				{
					NVIC_ClearPendingIRQ(TCC2_OTHER_IRQn);
					NVIC_EnableIRQ(TCC2_OTHER_IRQn);
				}
				else if constexpr (TccIndex == 3)
				{
					NVIC_ClearPendingIRQ(TCC3_OTHER_IRQn);
					NVIC_EnableIRQ(TCC3_OTHER_IRQn);
				}
				else if constexpr (TccIndex == 4)
				{
					NVIC_ClearPendingIRQ(TCC4_OTHER_IRQn);
					NVIC_EnableIRQ(TCC4_OTHER_IRQn);
				}
			}

			void disableOverflowInterrupt()
			{
				tccInstance_->INTENCLR.bit.OVF = 1;
				if constexpr (TccIndex == 0)
				{
					NVIC_DisableIRQ(TCC0_OTHER_IRQn);
				}
				else if constexpr (TccIndex == 1)
				{
					NVIC_DisableIRQ(TCC1_OTHER_IRQn);
				}
				else if constexpr (TccIndex == 2)
				{
					NVIC_DisableIRQ(TCC2_OTHER_IRQn);
				}
				else if constexpr (TccIndex == 3)
				{
					NVIC_DisableIRQ(TCC3_OTHER_IRQn);
				}
				else if constexpr (TccIndex == 4)
				{
					NVIC_DisableIRQ(TCC4_OTHER_IRQn);
				}
			}

			void enableMatchCaptureInterrupt(const uint8_t chNum)
			{
				if(chNum > tccNumberCCchannels(TccIndex)) return;
				///@todo
			}

			void disableMatchCaptureInterrupt(const uint8_t chNum)
			{
				if(chNum > tccNumberCCchannels(TccIndex)) return;
				///@todo
			}

			void setOverflowCallback(const std::function<void()>& overflowCallback)
			{
				TCC_OverflowCallback[TccIndex] = overflowCallback;
			}

			void setMatchCaptureCallback(const uint8_t chNum, const std::function<void()>& mcCallback)
			{
				if(chNum > tccNumberCCchannels(TccIndex)) return;
				TCC_McCallback[TccIndex][chNum] = mcCallback;
			}
			
		private:
      TimerCounterControl()
			{
				// Throw compile-error if invalid instance is requested
				static_assert(TccIndex < numTccDevices, "Invalid TimerCounterControl instance.");
			}
    private:
			uint32_t prescaler_;  // < TCC clock prescaler
		};
	}
}

#endif // TIMER_COUNTER_CONTROL_SAMD51_HPP