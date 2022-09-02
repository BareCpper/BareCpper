#include "BareCpper/ATsamd/AnalogToDigitalConverter_ATsamd5x.hpp"

namespace BareCpper
{
	namespace SAMD51
	{
		// Hold functions to be called when conversion done interrupt fires
		std::function<void()> ADC0_ConversionDoneCallback;
		std::function<void()> ADC1_ConversionDoneCallback;
	}
}

// ADC0 RESRDY interrupt handler
void ADC0_1_Handler()
{
	using namespace BareCpper::SAMD51;
	// call callback
	if (ADC0_ConversionDoneCallback) ADC0_ConversionDoneCallback();
	// Clear the interrupt flag
	ADC0->INTFLAG.bit.RESRDY = 1;
}

// ADC1 RESRDY interrupt handler
void ADC1_1_Handler()
{
	using namespace BareCpper::SAMD51;
	// call callback
	if (ADC1_ConversionDoneCallback) ADC1_ConversionDoneCallback();
	// Clear the interrupt flag
	ADC1->INTFLAG.bit.RESRDY = 1;
}