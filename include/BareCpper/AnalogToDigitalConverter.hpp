#pragma once

#include <cstdint>
#include <functional>

namespace BareCpper
{
	enum class ADCresolution : uint8_t
	{
		BIT8 = 8,
		BIT10 = 10,
		BIT12 = 12,
		BIT16 = 16
	};

	/**
	 * Interface class for ADC peripheral.
	 * Provides basic functions for ADC.
	*/
	class AdcBase
	{
	public:
		virtual ~AdcBase() { }

		/**
		 * @brief Initialize the ADC peripheral
		 * This function is needed instead of a constructor
		 * to be able to declare the object in global namespace
		 * to use it inside ISR, before other MCU initializations are done
		 * @param resolution The ADC resolution
		*/
		virtual void init(const ADCresolution &resolution = ADCresolution::BIT12) = 0;

		/**
		 * @brief Start the ADC peripheral
		*/
		virtual void enable() = 0;

		/**
		 * @brief Stop the ADC peripheral
		*/
		virtual void disable() = 0;

		/**
		 * @brief Read the ADC result register
		 * @return The ADC result register
		*/
		virtual uint32_t getLastConversionResult() = 0;

		/**
		 * @brief Get single ADC conversion result (blocking)
		 * @return The ADC conversion result
		*/
		virtual uint32_t getSingleConversion(const uint32_t inputPin) = 0;

		/**
		 * @brief Start conversion (non-blocking)
		*/
		virtual void startConversion(const uint32_t inputPin) = 0;

		/**
		 * @brief Check if the conversion is done
		 * @return true if conversion is done, false otherwise
		*/
		virtual bool isConversionDone() = 0;

		/**
		 * @brief Change the ADC resolution
		 * @param resolution The new ADC resolution
		*/
		virtual void setResolution(const ADCresolution &resolution)
		{
			resolution_ = resolution;
		}

		/**
		 * @brief Enable conversion done interrupt
		*/
		virtual void enableConversionDoneInterrupt() = 0;

		/**
		 * @brief Disable conversion done interrupt
		*/
		virtual void disableConversionDoneInterrupt() = 0;

		/**
		 * @brief Set callback function to be called when conversion done interrupt fires
		 * @param conversionDoneCallback The callback function
		*/
		virtual void setConversionDoneCallback(const std::function<void()>& conversionDoneCallback) = 0;
	protected:
		ADCresolution resolution_;
	};
}