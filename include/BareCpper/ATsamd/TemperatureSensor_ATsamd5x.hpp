#ifndef TEMPERATURE_SENSOR_SAMD51_HPP
#define TEMPERATURE_SENSOR_SAMD51_HPP

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <cstdint>

#include "../TemperatureSensor.hpp"
#include "AnalogToDigitalConverter_ATsamd5x.hpp"

#include <sam.h>

#define NVMCTRL_TEMP_LOG NVMCTRL_TEMP_LOG_W0

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
			TemperatureSensor()
			{
				// Read the calibration parameters stored in NVM
				calibrationParameters_.TLI = (*reinterpret_cast<uint32_t*>(FUSES_ROOM_TEMP_VAL_INT_ADDR) & FUSES_ROOM_TEMP_VAL_INT_Msk) >> FUSES_ROOM_TEMP_VAL_INT_Pos;
				calibrationParameters_.TLD = (*reinterpret_cast<uint32_t*>(FUSES_ROOM_TEMP_VAL_DEC_ADDR) & FUSES_ROOM_TEMP_VAL_DEC_Msk) >> FUSES_ROOM_TEMP_VAL_DEC_Pos;
				calibrationParameters_.THI = (*reinterpret_cast<uint32_t*>(FUSES_HOT_TEMP_VAL_INT_ADDR) & FUSES_HOT_TEMP_VAL_INT_Msk) >> FUSES_HOT_TEMP_VAL_INT_Pos;
				calibrationParameters_.THD = (*reinterpret_cast<uint32_t*>(FUSES_HOT_TEMP_VAL_DEC_ADDR) & FUSES_HOT_TEMP_VAL_DEC_Msk) >> FUSES_HOT_TEMP_VAL_DEC_Pos;
				calibrationParameters_.VPL = (*reinterpret_cast<uint32_t*>(FUSES_ROOM_ADC_VAL_PTAT_ADDR) & FUSES_ROOM_ADC_VAL_PTAT_Msk) >> FUSES_ROOM_ADC_VAL_PTAT_Pos;
				calibrationParameters_.VPH = (*reinterpret_cast<uint32_t*>(FUSES_HOT_ADC_VAL_PTAT_ADDR) & FUSES_HOT_ADC_VAL_PTAT_Msk) >> FUSES_HOT_ADC_VAL_PTAT_Pos;
				calibrationParameters_.VCL = (*reinterpret_cast<uint32_t*>(FUSES_ROOM_ADC_VAL_CTAT_ADDR) & FUSES_ROOM_ADC_VAL_CTAT_Msk) >> FUSES_ROOM_ADC_VAL_CTAT_Pos;
				calibrationParameters_.VCH = (*reinterpret_cast<uint32_t*>(FUSES_HOT_ADC_VAL_CTAT_ADDR) & FUSES_HOT_ADC_VAL_CTAT_Msk) >> FUSES_HOT_ADC_VAL_CTAT_Pos;
				// calculate the TL and TH from the integer and decimal parts
				auto decimal2float = [](const uint32_t val) {
					if (val < 10) return val / 10.0f;
					else if (val < 100) return val / 100.0f;
					else return val / 1000.0f;
				};
				calibrationParameters_.TL = calibrationParameters_.TLI + decimal2float(calibrationParameters_.TLD);
				calibrationParameters_.TH = calibrationParameters_.THI + decimal2float(calibrationParameters_.THD);
			}

			~TemperatureSensor()
			{
				disable();
			}

			/**
			 * Initialize the peripherals needed
			 * for temperature reading.
			 * 
			 */
			void init()
			{
				// initialize the adc
				adc_.init(ADCresolution::BIT12);
				adc_.setVoltageReference(ADCreference::Internal3V3);

				// SUPC must be in ON-DEMAND mode (it is set in ADC, but overwriting here too for now), needed because
				// the internal voltage reference is also used,
				// check datasheet, p.222, section 19.6.2.2
				SUPC->VREF.bit.ONDEMAND = 1;
			}

			/**
			 * @brief Enable the internal Temperature sensors of the MCU
			*/
			void enable()
			{
				SUPC->VREF.bit.TSEN = 1;
				adc_.enable();
			}

			/**
			 * @brief Disable the internal Temperature sensors of the MCU
			*/
			void disable()
			{
				SUPC->VREF.bit.TSEN = 0;
				adc_.disable();
			}

			/**
			 * @brief Read the internal temperature sensors and return the MCU temperature
			 * @return The temperature of the MCU
			*/
			float getTemperature() final
			{
				// read the internal temperature sensors using the ADC
				uint32_t TP = adc_.getSingleConversion(SAMD51::ADCpins::TemperatureSensorPTAT);
				uint32_t TC = adc_.getSingleConversion(SAMD51::ADCpins::TemperatureSensorCTAT);
				// convert the adc reading into temperature, using the calibration data, datahseet, p.1441, section 45.6.3.1
				float temperature =  (calibrationParameters_.TL * calibrationParameters_.VPH * TC - calibrationParameters_.VPL * calibrationParameters_.TH * TC - 
									calibrationParameters_.TL * calibrationParameters_.VCH * TP + calibrationParameters_.TH * calibrationParameters_.VCL * TP) / 
									(calibrationParameters_.VCL * TP - calibrationParameters_.VCH * TP - calibrationParameters_.VPL * TC + calibrationParameters_.VPH * TC);
				return temperature;

			}
		private:
			SAMD51::ADC<1> adc_; //< Use ADC1 instance for temperature reading
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