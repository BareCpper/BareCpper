#if ARDUINO
#include "BareCpper/include/BareCpper/ATsamd/TemperatureSensor_ATsamd5x.hpp"
#else
#include "BareCpper/ATsamd/TemperatureSensor_ATsamd5x.hpp"
#endif

namespace BareCpper
{
namespace SAMD51
{
	TemperatureSensor::TemperatureSensor()
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

	TemperatureSensor::~TemperatureSensor()
	{
		disable();
	}

  void TemperatureSensor::init()
  {
    // initialize the adc
		adc_.init(ADCresolution::BIT12);
		adc_.setVoltageReference(ADCreference::Internal3V3);

		// SUPC must be in ON-DEMAND mode (it is set in ADC, but overwriting here too for now), needed because
		// the internal voltage reference is also used,
		// check datasheet, p.222, section 19.6.2.2
		SUPC->VREF.bit.ONDEMAND = 1;
  }

	void TemperatureSensor::enable()
	{
		SUPC->VREF.bit.TSEN = 1;
		adc_.enableConversionDoneInterrupt();
		adc_.enable();				
	}

	void TemperatureSensor::disable()
	{
		SUPC->VREF.bit.TSEN = 0;
		adc_.disable();
		adc_.disableConversionDoneInterrupt();
		async_.state = {};
	}

	bool TemperatureSensor::getTemperatureAsync( float& temperature )
	{
		auto step1_onPTAT = [this]() {
			async_.TP = adc_.getLastConversionResult();

			auto step2_onCTAT = [this]() {
				async_.TC = adc_.getLastConversionResult();
				async_.state = Async::State::Done;
			};
			adc_.setConversionDoneCallback(step2_onCTAT);
			adc_.startConversion(SAMD51::ADCpins::TemperatureSensorCTAT);
		};

		const auto currentState = async_.state;
		switch (currentState)
		{
      case Async::State::Pending:
        return false;

      case Async::State::Done:
        async_.state = Async::State::Inactive;
        temperature = temperatureFromAdc(async_.TC, async_.TP);
        return true;

      default:
      case Async::State::Inactive:
        async_.state = Async::State::Pending;
        adc_.setConversionDoneCallback(step1_onPTAT);
        adc_.startConversion(SAMD51::ADCpins::TemperatureSensorPTAT);
        return false;
		}
	}

	float TemperatureSensor::getTemperature()
	{
		// read the internal temperature sensors using the ADC
		const uint32_t TP = adc_.getSingleConversion(SAMD51::ADCpins::TemperatureSensorPTAT);
		const uint32_t TC = adc_.getSingleConversion(SAMD51::ADCpins::TemperatureSensorCTAT);
		return temperatureFromAdc( TC, TP );
	}

	float TemperatureSensor::temperatureFromAdc( const uint32_t TC, const uint32_t TP ) const
	{
		// convert the adc reading into temperature, using the calibration data, datahseet, p.1441, section 45.6.3.1
		float temperature = (calibrationParameters_.TL * calibrationParameters_.VPH * TC - calibrationParameters_.VPL * calibrationParameters_.TH * TC -
							calibrationParameters_.TL * calibrationParameters_.VCH * TP + calibrationParameters_.TH * calibrationParameters_.VCL * TP) /
			(calibrationParameters_.VCL * TP - calibrationParameters_.VCH * TP - calibrationParameters_.VPL * TC + calibrationParameters_.VPH * TC);

		return temperature;
	}

}
}