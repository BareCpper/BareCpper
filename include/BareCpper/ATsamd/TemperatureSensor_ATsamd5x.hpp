#ifndef TEMPERATURE_SENSOR_SAMD51_HPP
#define TEMPERATURE_SENSOR_SAMD51_HPP

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <cstdint>
#include <atomic>

#include "../TemperatureSensor.hpp"
#include "AnalogToDigitalConverter_ATsamd5x.hpp"

#include <sam.h>

// @todo This is some hack for Ardunio CMSIS-Atmel?
#ifndef NVMCTRL_TEMP_LOG
#define NVMCTRL_TEMP_LOG NVMCTRL_TEMP_LOG_W0
#endif

namespace BareCpper
{
	namespace SAMD51
	{
		/**
		* @brief Read the internal SAMD51 temperature sensor
		*/
		class TemperatureSensor
			: BareCpper::TemperatureSensor<float>
		{
		public:
			TemperatureSensor();

			~TemperatureSensor();

			/**
			 * @brief Initialize the required peripherals for
			 * tempreature measuring.
			 * 
			 */
			void init();

			/**
			 * @brief Enable the internal Temperature sensors of the MCU
			*/
			void enable();

			/**
			 * @brief Disable the internal Temperature sensors of the MCU
			*/
			void disable();

			bool getTemperatureAsync(float& temperature) final;

			/**
			 * @brief Read the internal temperature sensors and return the MCU temperature
			 * @return The temperature of the MCU
			*/
			float getTemperature() final;

		private:
			float temperatureFromAdc(const uint32_t TC, const uint32_t TP) const;

		private:
			SAMD51::ADC<1> adc_; //< Use ADC1 instance for temperature reading

			/** Asyncronous state container
			*/
			struct Async
			{
				enum class State : std::uint8_t { Inactive, Pending, Done };

				uint32_t TP = 0; //< Last reading of TemperatureSensorPTAT
				uint32_t TC = 0; //< Last reading of TemperatureSensorCTAT
				std::atomic<State> state = Async::State::Inactive; //< Async status: Pending, Inactive, Done 
			};
			volatile Async async_ = {};

			struct TemperatureCalibrationParameters
			{
				uint8_t TLI, TLD;
				uint8_t THI, THD;
				uint16_t VPL, VPH;
				uint16_t VCL, VCH;
				float TL, TH;
			} calibrationParameters_; //< structure holding the calibration parameters stored in NVM needed to obtain the temperature reading
		};
	}
}
#endif //TEMPERATURE_SENSOR_SAMD51_HPP