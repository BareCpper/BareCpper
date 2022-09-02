#ifndef TIMER_COUNTER_SAMD51_HPP
#define TIMER_COUNTER_SAMD51_HPP

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <array>
#include <limits>

#include "../TimerCounter.hpp"
#include <sam.h>

#define GCLK1_HZ 48000000U  //< Clock generator 1 frequency, in Hz
														/// @todo get this from other place

namespace BareCpper
{
	namespace SAMD51
	{
		/**
		 * @brief Get requested MCU TimerCounter instance
		 * @param tcIndex The index of the TC instance
		 * @return Pointer to the requested TC instance
		*/
		inline Tc* tcInstance(const uint8_t tcIndex)
		{
			if (tcIndex >= TC_INST_NUM)
			{
				return nullptr;
			}
			std::array tcDevices = TC_INSTS;
			return tcDevices[tcIndex];
		}

		// Functions to be called when period and capture/compare interrupts fire
		extern std::function<void()> TC0_PeriodCallback;
		extern std::function<void()> TC0_CaptureCompareCallback;

		extern std::function<void()> TC1_PeriodCallback;
		extern std::function<void()> TC1_CaptureCompareCallback;

		extern std::function<void()> TC2_PeriodCallback;
		extern std::function<void()> TC2_CaptureCompareCallback;

		extern std::function<void()> TC3_PeriodCallback;
		extern std::function<void()> TC3_CaptureCompareCallback;

		/**
		 * Counter mode specifies the width of the counter
		*/
		enum class CounterMode
		{
			BIT8,
			BIT16,
			BIT32
		};

		/**
		 * Waveform generation mode
		*/
		enum class WaveformMode
		{
			NFRQ,  ///< Normal frequency mode
			MFRQ,  ///< Match frequency mode
			NPWM,  ///< Normal PWM mode
			MPWM   ///< Match PWM mode
		};

		enum class TimerEventAction : uint8_t
		{
			Off 			= 0x0,
			Retrigger	= 0x1,
			Count			= 0x2,
			Start 		= 0x3,
			Stamp 		= 0x4,
			Ppw 			= 0x5,
			Pwp 			= 0x6,
			Pw 				= 0x7
		};

		enum class TimerCounterCommand : uint8_t
		{
			None				= 0x0,
			Retrigger		= 0x1,
			Stop				= 0x2,
			Update			= 0x3,
			ReadSync		= 0x4
		};

		/**
		 * @brief Timer/Counter peripheral functionalities for SAMD51
		 * @tparam TCinstance The TC peripheral instance
		 * @tparam CountMode Width of the counter
		 * @tparam WaveMode Waveform generation mode
		*/
		template <
			uint8_t TCindex,
			CounterMode TCmode = CounterMode::BIT16,
			WaveformMode TCwave = WaveformMode::MPWM >
			class TimerCounter
			: public BareCpper::TimerCounter
		{
			Tc* const tcInstance_ = tcInstance(TCindex);
		public:
			TimerCounter()
			{
				// Throw compile-error on modes that are currently not implemented
				constexpr int supportedTCinstances = 4;  //< number of currently supported TC instances (TC0 - TC3 should be on every samd51 variant)
				static_assert(TCindex < supportedTCinstances, "Invalid TimerCounter instance.");
				static_assert(TCmode == CounterMode::BIT16, "Only 16-bit TC counter mode can be used.");
				static_assert(TCwave == WaveformMode::MPWM, "Only Match PWM TC waveform generation mode can be used.");
			}

			virtual ~TimerCounter()
			{
				disablePeriodInterrupt();
				disableCaptureCompareInterrupt();
				// clear global callback function wrappers
				if constexpr (TCindex == 0)
				{
					TC0_PeriodCallback = nullptr;
					TC0_CaptureCompareCallback = nullptr;
				}
				else if constexpr (TCindex == 1)
				{
					TC1_PeriodCallback = nullptr;
					TC1_CaptureCompareCallback = nullptr;
				}
				if constexpr (TCindex == 2)
				{
					TC2_PeriodCallback = nullptr;
					TC2_CaptureCompareCallback = nullptr;
				}
				if constexpr (TCindex == 3)
				{
					TC3_PeriodCallback = nullptr;
					TC3_CaptureCompareCallback = nullptr;
				}
			}

			void init(uint32_t periodUs, uint32_t ccUs = 0)
			{
				// Allow writing to TC registers
				// Select generic clock generator 1 (48 MHz) for ADC
				if constexpr (TCindex == 0)
				{
					MCLK->APBAMASK.reg |= MCLK_APBAMASK_TC0;
					GCLK->PCHCTRL[TC0_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
				}
				else if constexpr (TCindex == 1)
				{
					MCLK->APBAMASK.reg |= MCLK_APBAMASK_TC1;
					GCLK->PCHCTRL[TC1_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
				}
				if constexpr (TCindex == 2)
				{
					MCLK->APBBMASK.reg |= MCLK_APBBMASK_TC2;
					GCLK->PCHCTRL[TC2_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
				}
				if constexpr (TCindex == 3)
				{
					MCLK->APBBMASK.reg |= MCLK_APBBMASK_TC3;
					GCLK->PCHCTRL[TC3_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
				}
				// wait for clock synchronization
				while (GCLK->SYNCBUSY.reg);
				// Set the other options based on count mode
				if constexpr (TCmode == CounterMode::BIT16)
				{
					if constexpr (TCwave == WaveformMode::MPWM)
					{
						// Match PWM mode (CC0 sets Top value, CC1 is toggle time)
						tcInstance_->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MPWM_Val;
					}
				}
				setPeriod(periodUs);
				setCaptureCompare(ccUs);
			}

			/**
			 * @brief Start the TC peripheral
			*/
			void start() override
			{
				if constexpr (TCmode == CounterMode::BIT16)
				{
					// Start the timer
					tcInstance_->COUNT16.CTRLA.bit.ENABLE = 1;
					// wait for synchronization
					while (tcInstance_->COUNT16.SYNCBUSY.bit.ENABLE);
				}
			}

			/**
			 * @brief Stop the TC peripheral
			*/
			void stop() override
			{
				if constexpr (TCmode == CounterMode::BIT16)
				{
					// Stop the timer
					tcInstance_->COUNT16.CTRLA.bit.ENABLE = 0;
					// wait for synchronization
					while (tcInstance_->COUNT16.SYNCBUSY.bit.ENABLE);
				}
			}

			/**
			 * @brief Read the current TC counter register value and convert it in us
			 * @return The current TC counter value, in us
			*/
			double getCounterValue() override
			{
				if constexpr (TCmode == CounterMode::BIT16)
				{
					// Force read synchronization of COUNT
					tcInstance_->COUNT16.CTRLBSET.bit.CMD = TC_CTRLBSET_CMD_READSYNC_Val;
					while (tcInstance_->COUNT16.SYNCBUSY.bit.COUNT);

					// Convert the count value in us
					return (static_cast<double>(tcInstance_->COUNT16.COUNT.reg) * 1000000) / (prescaler_ * GCLK1_HZ);
				}
			}

			/**
			 * @brief Change the TC period (overflow time)
			 * @param timeUs The period in us
			*/
			void setPeriod(uint32_t timeUs) override
			{
				if constexpr (TCmode == CounterMode::BIT16)
				{
					if constexpr (TCwave == WaveformMode::MPWM)
					{
						// stop the TC peripheral if it is running
						const bool shouldRestart = tcInstance_->COUNT16.CTRLA.bit.ENABLE;
						if (shouldRestart)
						{
							stop();
						}
						// Set prescaler based on period time
						if (timeUs > 300000)
						{
							tcInstance_->COUNT16.CTRLA.bit.PRESCALER = TC_CTRLA_PRESCALER_DIV1024_Val;
							prescaler_ = 1024;
						}
						else if (80000 < timeUs && timeUs <= 300000)
						{
							tcInstance_->COUNT16.CTRLA.bit.PRESCALER = TC_CTRLA_PRESCALER_DIV256_Val;
							prescaler_ = 256;
						}
						else if (20000 < timeUs && timeUs <= 80000)
						{
							tcInstance_->COUNT16.CTRLA.bit.PRESCALER = TC_CTRLA_PRESCALER_DIV64_Val;
							prescaler_ = 64;
						}
						else if (10000 < timeUs && timeUs <= 20000)
						{
							tcInstance_->COUNT16.CTRLA.bit.PRESCALER = TC_CTRLA_PRESCALER_DIV16_Val;
							prescaler_ = 16;
						}
						else if (5000 < timeUs && timeUs <= 10000)
						{
							tcInstance_->COUNT16.CTRLA.bit.PRESCALER = TC_CTRLA_PRESCALER_DIV8_Val;
							prescaler_ = 8;
						}
						else if (2500 < timeUs && timeUs <= 5000)
						{
							tcInstance_->COUNT16.CTRLA.bit.PRESCALER = TC_CTRLA_PRESCALER_DIV4_Val;
							prescaler_ = 4;
						}
						else if (1000 < timeUs && timeUs <= 2500)
						{
							tcInstance_->COUNT16.CTRLA.bit.PRESCALER = TC_CTRLA_PRESCALER_DIV2_Val;
							prescaler_ = 2;
						}
						else if (timeUs <= 1000)
						{
							tcInstance_->COUNT16.CTRLA.bit.PRESCALER = TC_CTRLA_PRESCALER_DIV1_Val;
							prescaler_ = 1;
						}

						// Calculate the value that needs to be stored in register CC0
						tcInstance_->COUNT16.CC[0].reg = ((GCLK1_HZ / 1000000) * timeUs) / prescaler_ - 1;
						while (tcInstance_->COUNT16.SYNCBUSY.bit.CC0);

						// reenable the TC peripheral, if needed
						if (shouldRestart)
						{
							start();
						}
					}
				}
			}

			void enableOneShotMode(bool enable=true)
			{
				if constexpr (TCmode == CounterMode::BIT16)
				{
					if constexpr (TCwave == WaveformMode::MPWM)
					{
						if(enable) tcInstance_->COUNT16.CTRLBSET.bit.ONESHOT = 1;
						else tcInstance_->COUNT16.CTRLBCLR.bit.ONESHOT = 1;

						// wait for synchronization
						while(tcInstance_->COUNT16.SYNCBUSY.bit.CTRLB);
					}
				}
			}

			void setOutputEvent(bool mc0, bool mc1, bool ovf)
			{
				if constexpr (TCmode == CounterMode::BIT16)
				{
					if constexpr (TCwave == WaveformMode::MPWM)
					{
						if(mc0) tcInstance_->COUNT16.EVCTRL.bit.MCEO0 = 1;
						else tcInstance_->COUNT16.EVCTRL.bit.MCEO0 = 0;
						
						if(mc1) tcInstance_->COUNT16.EVCTRL.bit.MCEO1 = 1;
						else tcInstance_->COUNT16.EVCTRL.bit.MCEO1 = 0;
						
						if(ovf) tcInstance_->COUNT16.EVCTRL.bit.OVFEO = 1;
						else tcInstance_->COUNT16.EVCTRL.bit.OVFEO = 0;
					}
				}
			}

			void setInputEvent(const TimerEventAction &action)
			{
				if constexpr (TCmode == CounterMode::BIT16)
				{
					if constexpr (TCwave == WaveformMode::MPWM)
					{
						if (action == TimerEventAction::Off)
						{
							tcInstance_->COUNT16.EVCTRL.bit.TCEI = 0;
						}
						else
						{
							tcInstance_->COUNT16.EVCTRL.bit.TCEI = 1;
							tcInstance_->COUNT16.EVCTRL.bit.EVACT = static_cast<uint8_t>(action);
						}
					}
				}
			}

			void writeCommand(const TimerCounterCommand &cmd)
			{
				if constexpr (TCmode == CounterMode::BIT16)
				{
					if constexpr (TCwave == WaveformMode::MPWM)
					{
						tcInstance_->COUNT16.CTRLBSET.bit.CMD = static_cast<uint8_t>(cmd);
					}
				}
			}

			/**
			 * @brief Set Capture/Compare match time
			 * @param timeUs The match time in us
			*/
			void setCaptureCompare(uint32_t timeUs) override
			{
				if constexpr (TCmode == CounterMode::BIT16)
				{
					if constexpr (TCwave == WaveformMode::MPWM)
					{
						// stop the TC peripheral if it is running
						const bool shouldRestart = tcInstance_->COUNT16.CTRLA.bit.ENABLE;
						if (shouldRestart)
						{
							stop();
						}
						
						tcInstance_->COUNT16.CC[1].reg = ((GCLK1_HZ / 1000000) * timeUs) / prescaler_ - 1;
						while (tcInstance_->COUNT16.SYNCBUSY.bit.CC1);
						
						// reenable the TC peripheral, if needed
						if (shouldRestart)
						{
							start();
						}
					}
				}
			}

			/**
			 * @brief Enable period interrupt (CC0 match interrupt)
			*/
			void enablePeriodInterrupt() override
			{
				if constexpr (TCmode == CounterMode::BIT16)
				{
					if constexpr (TCwave == WaveformMode::MPWM)
					{
						tcInstance_->COUNT16.INTENSET.bit.MC0 = 1;
						// Enable interrupt in NVIC, if not already enabled
						if constexpr (TCindex == 0)
						{
							if (!NVIC_GetEnableIRQ(TC0_IRQn)) NVIC_EnableIRQ(TC0_IRQn);
						}
						else if constexpr (TCindex == 1)
						{
							if (!NVIC_GetEnableIRQ(TC1_IRQn)) NVIC_EnableIRQ(TC1_IRQn);
						}
						else if constexpr (TCindex == 2)
						{
							if (!NVIC_GetEnableIRQ(TC2_IRQn)) NVIC_EnableIRQ(TC2_IRQn);
						}
						else if constexpr (TCindex == 3)
						{
							if (!NVIC_GetEnableIRQ(TC3_IRQn)) NVIC_EnableIRQ(TC3_IRQn);
						}
					}
				}
			}

			/**
			 * @brief Enable interrupt on compare match (CC1 match interrupt)
			*/
			void enableCaptureCompareInterrupt() override
			{
				if constexpr (TCmode == CounterMode::BIT16)
				{
					if constexpr (TCwave == WaveformMode::MPWM)
					{
						tcInstance_->COUNT16.INTENSET.bit.MC1 = 1;
						// Enable interrupt in NVIC, if not already enabled
						if constexpr (TCindex == 0)
						{
							if (!NVIC_GetEnableIRQ(TC0_IRQn)) NVIC_EnableIRQ(TC0_IRQn);
						}
						else if constexpr (TCindex == 1)
						{
							if (!NVIC_GetEnableIRQ(TC1_IRQn)) NVIC_EnableIRQ(TC1_IRQn);
						}
						else if constexpr (TCindex == 2)
						{
							if (!NVIC_GetEnableIRQ(TC2_IRQn)) NVIC_EnableIRQ(TC2_IRQn);
						}
						else if constexpr (TCindex == 3)
						{
							if (!NVIC_GetEnableIRQ(TC3_IRQn)) NVIC_EnableIRQ(TC3_IRQn);
						}
					}
				}
			}

			/**
			 * @brief Disable period interrupt (CC0 match interrupt)
			*/
			void disablePeriodInterrupt() override
			{
				if constexpr (TCmode == CounterMode::BIT16)
				{
					if constexpr (TCwave == WaveformMode::MPWM)
					{
						tcInstance_->COUNT16.INTENCLR.bit.MC0 = 1;
						// Disable interrupt in NVIC, if not needed anymore
						if constexpr (TCindex == 0)
						{
							if (!TC0->COUNT16.INTENSET.bit.MC1) NVIC_DisableIRQ(TC0_IRQn);
						}
						else if constexpr (TCindex == 1)
						{
							if (!TC1->COUNT16.INTENSET.bit.MC1) NVIC_DisableIRQ(TC1_IRQn);
						}
						else if constexpr (TCindex == 2)
						{
							if (!TC2->COUNT16.INTENSET.bit.MC1) NVIC_DisableIRQ(TC2_IRQn);
						}
						else if constexpr (TCindex == 3)
						{
							if (!TC3->COUNT16.INTENSET.bit.MC1) NVIC_DisableIRQ(TC3_IRQn);
						}
					}
				}
			}

			/**
			 * @brief Disable interrupt on compare match (CC1 match interrupt)
			*/
			void disableCaptureCompareInterrupt() override
			{
				if constexpr (TCmode == CounterMode::BIT16)
				{
					if constexpr (TCwave == WaveformMode::MPWM)
					{
						tcInstance_->COUNT16.INTENCLR.bit.MC1 = 1;
						// Disable interrupt in NVIC, if not needed anymore
						if constexpr (TCindex == 0)
						{
							if (!TC0->COUNT16.INTENSET.bit.MC0) NVIC_DisableIRQ(TC0_IRQn);
						}
						else if constexpr (TCindex == 1)
						{
							if (!TC1->COUNT16.INTENSET.bit.MC0) NVIC_DisableIRQ(TC1_IRQn);
						}
						else if constexpr (TCindex == 2)
						{
							if (!TC2->COUNT16.INTENSET.bit.MC0) NVIC_DisableIRQ(TC2_IRQn);
						}
						else if constexpr (TCindex == 3)
						{
							if (!TC3->COUNT16.INTENSET.bit.MC0) NVIC_DisableIRQ(TC3_IRQn);
						}
					}
				}
			}

			/**
			 * @brief Set callback function to be called when CC0 match interrupt fires
			 * @param callbackFunction The function to be called when cc0 interrupt happens
			*/
			void setPeriodCallback(const std::function<void()>& periodCallback)
			{
				if constexpr (TCindex == 0)
				{
					TC0_PeriodCallback = periodCallback;
				}
				else if constexpr (TCindex == 1)
				{
					TC1_PeriodCallback = periodCallback;
				}
				else if constexpr (TCindex == 2)
				{
					TC2_PeriodCallback = periodCallback;
				}
				else if constexpr (TCindex == 3)
				{
					TC3_PeriodCallback = periodCallback;
				}
			}

			/**
			 * @brief Set callback function to be called when CC1 match interrupt fires
			 * @param ccCallback The function to be called when cc1 interrupt happens
			*/
			void setCaptureCompareCallback(const std::function<void()>& ccCallback)
			{
				if constexpr (TCindex == 0)
				{
					TC0_CaptureCompareCallback = ccCallback;
				}
				else if constexpr (TCindex == 1)
				{
					TC1_CaptureCompareCallback = ccCallback;
				}
				else if constexpr (TCindex == 2)
				{
					TC2_CaptureCompareCallback = ccCallback;
				}
				else if constexpr (TCindex == 3)
				{
					TC3_CaptureCompareCallback = ccCallback;
				}
			}
		private:
			uint32_t prescaler_;  // < TC clock prescaler
		};
	}
}

#endif //TIMER_COUNTER_SAMD51_HPP