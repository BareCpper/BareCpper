#ifndef ANALOG_TO_DIGITAL_CONVERTER_SAMD51_HPP
#define ANALOG_TO_DIGITAL_CONVERTER_SAMD51_HPP

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <array>
#include "../AnalogToDigitalConverter.hpp"
#include <sam.h>
#if __has_include(<component-version.h>) 
#include <component-version.h> //< Atmel SAM version defines
#else
#define COMPONENT_VERSION_MAJOR 1 //< Artdunio has v1 headers
#endif

namespace BareCpper
{
	namespace SAMD51
	{

		/**
		 * @brief Get requested MCU ADC instance
		 * @param adcIndex The index of the ADC instance
		 * @return Pointer to the requested ADC instance
		*/
		inline Adc* adcInstance(const uint8_t adcIndex)
		{
			if (adcIndex >= ADC_INST_NUM)
			{
				return nullptr;
			}
			std::array adcDevices = ADC_INSTS;
			return adcDevices[adcIndex];
		}

		// Functions to be called when conversion done interrupt fires
		extern std::function<void()> ADC0_ConversionDoneCallback;
		extern std::function<void()> ADC1_ConversionDoneCallback;

		/**
		 * Supported analog pins and channels
		*/
		enum class ADCpins : uint32_t
		{
			BroadbandPhotodiode = ADC_INPUTCTRL_MUXPOS_AIN14_Val,
			IRcutPhotodiode = ADC_INPUTCTRL_MUXPOS_AIN15_Val,
			TemperatureSensorPTAT = ADC_INPUTCTRL_MUXPOS_PTAT_Val,
			TemperatureSensorCTAT = ADC_INPUTCTRL_MUXPOS_CTAT_Val
		};

		/**
		 * @brief Supported voltage references for the ADC
		*/
		enum class ADCreference : uint32_t
		{
			Internal1V0 = SUPC_VREF_SEL_1V0_Val,
			Internal1V1 = SUPC_VREF_SEL_1V1_Val,
			Internal1V2 = SUPC_VREF_SEL_1V2_Val,
			Internal1V25 = SUPC_VREF_SEL_1V25_Val,
			Internal2V0 = SUPC_VREF_SEL_2V0_Val,
			Internal2V2 = SUPC_VREF_SEL_2V2_Val,
			Internal2V4 = SUPC_VREF_SEL_2V4_Val,
			Internal2V5 = SUPC_VREF_SEL_2V4_Val,
			Internal1V65 = ADC_REFCTRL_REFSEL_INTVCC0_Val,
			Internal3V3 = ADC_REFCTRL_REFSEL_INTVCC1_Val
		};

		/**
		 * @brief SAMD51 ADC interface class
		 * @tparam adcIndex ADC instance index
		*/
		template < uint8_t adcIndex >
		class ADC
			: BareCpper::AdcBase
		{
			Adc* const adcInstance_ = adcInstance(adcIndex);  //< the ADC instance pointer

			static constexpr IRQn_Type irqConversionDoneInterrupt()
			{
				// @note Based on atmel reference hpl_adc.c : _adc_get_irq_num();
#if COMPONENT_VERSION_MAJOR == 1
				return static_cast<IRQn_Type>(ADC0_1_IRQn + (adcIndex << 1));
#elif COMPONENT_VERSION_MAJOR == 2
				return static_cast<IRQn_Type>(ADC0_RESRDY_IRQn + (adcIndex << 1));
#endif
			}

		public:
			ADC()
			{
				static_assert(adcIndex < ADC_INST_NUM, "Invalid ADC instance requested.");
			}

			~ADC()
			{
				// disable the interrupt
				disableConversionDoneInterrupt();
				// clear the global callback function wrapper
				if constexpr (adcIndex == 0)
				{
					ADC0_ConversionDoneCallback = nullptr;
				}
				else if constexpr (adcIndex == 1)
				{
					ADC1_ConversionDoneCallback = nullptr;
				}
				// disable the ADC
				disable();
			}

			void init(const ADCresolution& resolution = ADCresolution::BIT12) override
			{
				 /*
				  * ADC conversion time = Sampling time + <ADC resolution> * CLK_ADC
				  * 
				  * Sampling time is chosen based on the following formula
				  * (54.10.4 Analog-to-Digital Converter (ADC) Characteristics, p.1800 in the datasheet)
				  * 
				  * tsamplehold >= (Rsample + Rsource) x Csample x 9.7
				  * 
				  * Rsample is 2kOhms and Csample is 3 pF, in the worst case, Rsource is the output impedance
				  * of the op-amp circuit, in the op-amp linear range it is negligible, we obtain
				  * tsamlehold >= 60 ns.
				  * !!!!!!!!!!! SOMETHING IS MISSING, IT IS NOT WORKING AS IT SHOULD BE USING THIS RESULT !!!!!!!!!!!!!!
				  * !!!!!!!!!!! Maybe the op-amp saturating !!!!!!!!!!!!
				  * 
				  * We can set CLK_ADC to 48 MHz / 64 = 750 kHz (period of 1.333... us),
				  * the sampling time to 54 * CLK_ADC = 72 us.
				  * 
				  * Assuming 12-bit resolution.
				  * One conversion will be done in 72 + 12 * 1.333 = 88 us.
				 */
				if constexpr (adcIndex == 0)
				{
					MCLK->APBDMASK.reg |= MCLK_APBDMASK_ADC0;
					GCLK->PCHCTRL[ADC0_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
				}
				else if constexpr (adcIndex == 1)
				{
					MCLK->APBDMASK.reg |= MCLK_APBDMASK_ADC1;
					GCLK->PCHCTRL[ADC1_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
				}
				// Wait for clock synchronization
				while (GCLK->SYNCBUSY.reg);

				// Reset ADC registers
				adcInstance_->CTRLA.bit.SWRST = 1;
				while (adcInstance_->CTRLA.bit.SWRST);

				// Set voltage reference to 2.5 V
				setVoltageReference(ADCreference::Internal2V5);

				// change to old reference (1.65 V)
				// setVoltageReference(ADCreference::Internal1V65);

				//// enable 4 samples averaging
				// adcInstance_->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_4 |    // N oversampling
				// ADC_AVGCTRL_ADJRES(0x2);   // Adjusting result to be 12bits as before

				// while(adcInstance_->SYNCBUSY.reg & ADC_SYNCBUSY_AVGCTRL);  //wait for sync

				// Set the clock divider
				adcInstance_->CTRLA.bit.PRESCALER = ADC_CTRLA_PRESCALER_DIV64_Val;

				// Set the sampling time
				adcInstance_->SAMPCTRL.bit.SAMPLEN = 53;
				// Wait for synchronization
				while (adcInstance_->SYNCBUSY.bit.SAMPCTRL);

				// Set the ADC resolution
				setResolution(resolution);

				// Configure analog pins here
				// Problem: May be board dependent, right now it looks same
				// on every variant
				using Port = ::Port;
				// LR (broadband photodiode after analog processing) is on PB02 (AIN[14])
				// Configure PB02 as peripheral function B (ADC)
				PORT->Group[1].PINCFG[2].bit.PMUXEN = 1;
				PORT->Group[1].PMUX[1].reg |= PORT_PMUX_PMUXE(1);
				// LL (ir-cut photodiode after analog processing) is on PB03 (AIN[15])
				// Configure PB03 as peripheral function B (ADC)
				PORT->Group[1].PINCFG[3].bit.PMUXEN = 1;
				PORT->Group[1].PMUX[1].reg |= PORT_PMUX_PMUXO(1);
			}

			/**
			 * @brief Start the ADC peripheral
			*/
			void enable() override
			{
				// Enable ADC
				adcInstance_->CTRLA.bit.ENABLE = 1;
				while (adcInstance_->SYNCBUSY.bit.ENABLE);
			}

			/**
			 * @brief Stop the ADC peripheral
			*/
			void disable() override
			{
				// Disable ADC
				adcInstance_->CTRLA.bit.ENABLE = 0;
				while (adcInstance_->SYNCBUSY.bit.ENABLE);
			}

			void setVoltageReference(const ADCreference& voltageReference)
			{
				const bool shouldRestart = adcInstance_->CTRLA.bit.ENABLE;
				if (shouldRestart)
				{
					disable();
				}
				// Select the voltage reference
				if (voltageReference == ADCreference::Internal1V65)
				{
					adcInstance_->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INTVCC0_Val;
				}
				else if (voltageReference == ADCreference::Internal3V3)
				{
					adcInstance_->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INTVCC1_Val;
				}
				else
				{
					// switch SUPC VREF in on-demand mode, so VREF can work together with temp sensors
					SUPC->VREF.bit.ONDEMAND = 1;
					// set the internal voltage reference voltage
					SUPC->VREF.bit.SEL = static_cast<uint32_t>(voltageReference);
					// route it to the ADC
					SUPC->VREF.bit.VREFOE = 1;
					adcInstance_->REFCTRL.bit.REFSEL = ADC_REFCTRL_REFSEL_INTREF_Val;
				}
				// Wait for synchronization
				while (adcInstance_->SYNCBUSY.bit.REFCTRL);
				// reenable the ADC if it was running
				if (shouldRestart)
				{
					enable();
				}
			}
			
			/**
			 * @brief Read the ADC result register and scale it for the correct resolution
			 * @return The ADC result register
			*/
			uint32_t getLastConversionResult() override
			{
				// if resolution is 16-bit, for now just shift the 12 bit result
				if (resolution_ == ADCresolution::BIT16)
				{
					return (adcInstance_->RESULT.reg) << 4;
				}
				return adcInstance_->RESULT.reg;
			}

			/**
			 * @brief Get a single conversion result
			 * @param inputPin The input pin to sample
			 * @return The conversion result
			*/
			uint32_t getSingleConversion(const ADCpins& inputPin)
			{
				return getSingleConversion(static_cast<uint32_t>(inputPin));
			}

			/**
			 * @brief Start conversion (non-blocking)
			 * @param inputPin The analog pin to sample
			*/
			void startConversion(const ADCpins& inputPin)
			{
				startConversion(static_cast<uint32_t>(inputPin));
			}

			/**
			 * @brief Check if the conversion is done
			 * @return true if conversion is done, false otherwise
			*/
			bool isConversionDone() override
			{
				return adcInstance_->INTFLAG.bit.RESRDY;
			}

			/**
			 * @brief Change the ADC resolution
			 * @param resolution The new ADC resolution
			*/
			void setResolution(const ADCresolution& resolution) override
			{
				// Call the base class function
				BareCpper::AdcBase::setResolution(resolution);
				// Change the ADC resolution
				// disable the ADC if it is running
				const bool shouldRestart = adcInstance_->CTRLA.bit.ENABLE;
				if (shouldRestart)
				{
					disable();
				}
				// change the resolution
				uint32_t ADC_CTRLB_RESSEL_Val;
				if (resolution == ADCresolution::BIT8)
				{
					ADC_CTRLB_RESSEL_Val = ADC_CTRLB_RESSEL_8BIT_Val;
				}
				else if (resolution == ADCresolution::BIT10)
				{
					ADC_CTRLB_RESSEL_Val = ADC_CTRLB_RESSEL_10BIT_Val;
				}
				else
				{
					ADC_CTRLB_RESSEL_Val = ADC_CTRLB_RESSEL_12BIT_Val;
				}
				// Set ADC resolution
				adcInstance_->CTRLB.bit.RESSEL = ADC_CTRLB_RESSEL_Val;
				// Wait for clock synchronization
				while (adcInstance_->SYNCBUSY.bit.CTRLB);
				// reenable the ADC if it was running
				if (shouldRestart)
				{
					enable();
				}
			}



			/**
			 * @brief Enable conversion done interrupt
			*/
			void enableConversionDoneInterrupt() override
			{
				// Enable ADC result ready interrupt
				adcInstance_->INTENSET.bit.RESRDY = 1;
				// Enable the interrupt in the NVIC
				constexpr auto irq = irqConversionDoneInterrupt();
				NVIC_ClearPendingIRQ(irq);
				NVIC_EnableIRQ(irq);
			}

			/**
			 * @brief Disable conversion done interrupt
			*/
			void disableConversionDoneInterrupt() override
			{
				// Disable ADC result ready interrupt
				adcInstance_->INTENCLR.bit.RESRDY = 1;
				// Disable the interrupt in the NVIC
				constexpr auto irq = irqConversionDoneInterrupt();
				NVIC_DisableIRQ(irq);
			}

			/**
			 * @brief Set callback function to be called when result ready interrupt fires
			 * @param conversionDoneCallback The callback function
			*/
			void setConversionDoneCallback(const std::function<void()>& conversionDoneCallback) override
			{
				if constexpr (adcIndex == 0)
				{
					ADC0_ConversionDoneCallback = conversionDoneCallback;
				}
				else if constexpr (adcIndex == 1)
				{
					ADC1_ConversionDoneCallback = conversionDoneCallback;
				}
			}

			private:
				/**
				 * @brief Get single ADC conversion result (blocking)
				 * @param inputPin The analog pin to sample
				 * @return The ADC conversion result
				*/
				uint32_t getSingleConversion(const uint32_t inputPin) override
				{
					// start conversion in software and wait for result
					// assumes interrupts are disabled and/or no callbacks are used
					/// @todo: disable adc irq inside this function?
					adcInstance_->INPUTCTRL.bit.MUXPOS = inputPin;
					while (adcInstance_->SYNCBUSY.bit.INPUTCTRL);
					adcInstance_->SWTRIG.bit.START = 1;
					while (!adcInstance_->INTFLAG.bit.RESRDY);
					return getLastConversionResult();
				}

				/**
				 * @brief Start conversion (non-blocking)
				 * @param inputPin The analog pin to sample
				*/
				void startConversion(const uint32_t inputPin) override
				{
					adcInstance_->INPUTCTRL.bit.MUXPOS = inputPin;
					//@note Could wait here for sync, should be unecessary i.e. 
					// while (adcInstance_->SYNCBUSY.bit.INPUTCTRL);
					adcInstance_->SWTRIG.bit.START = 1;
				}
		};
	}
}

#endif //ANALOG_TO_DIGITAL_CONVERTER_SAMD51_HPP