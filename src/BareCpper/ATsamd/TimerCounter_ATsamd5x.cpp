#if ARDUINO
#include "BareCpper/include/BareCpper/ATsamd/TimerCounter_ATsamd5x.hpp"
#else
#include "BareCpper/ATsamd/TimerCounter_ATsamd5x.hpp"
#endif
namespace BareCpper
{
	namespace SAMD51
	{
		// Hold functions to be called when period and capture/compare interrupts fire
		std::function<void()> TC0_PeriodCallback;
		std::function<void()> TC0_CaptureCompareCallback;

		std::function<void()> TC1_PeriodCallback;
		std::function<void()> TC1_CaptureCompareCallback;

		std::function<void()> TC2_PeriodCallback;
		std::function<void()> TC2_CaptureCompareCallback;

		std::function<void()> TC3_PeriodCallback;
		std::function<void()> TC3_CaptureCompareCallback;
	}
}

///@todo Refactor duplicate code
extern "C"
{
	// TC0 interrupt handler
	void TC0_Handler()
	{
		using namespace BareCpper::SAMD51;
		// check if interrupt is caused by CC0 compare (period)
		if (TC0->COUNT16.INTFLAG.bit.MC0 == 1)
		{
			// call callback
			if (TC0_PeriodCallback) TC0_PeriodCallback();
			// reset the interrupt flag
			TC0->COUNT16.INTFLAG.bit.MC0 = 1;
		}
		// check if interrupt is caused by CC1 compare (duty cycle)
		else if (TC0->COUNT16.INTFLAG.bit.MC1 == 1)
		{
			// call callback
			if (TC0_CaptureCompareCallback) TC0_CaptureCompareCallback();
			// reset the interrupt flag
			TC0->COUNT16.INTFLAG.bit.MC1 = 1;
		}
	}

	// TC1 interrupt handler
	void TC1_Handler()
	{
		using namespace BareCpper::SAMD51;
		// check if interrupt is caused by CC0 compare (period)
		if (TC1->COUNT16.INTFLAG.bit.MC0 == 1)
		{
			// call callback
			if (TC1_PeriodCallback) TC1_PeriodCallback();
			// reset the interrupt flag
			TC1->COUNT16.INTFLAG.bit.MC0 = 1;
		}
		// check if interrupt is caused by CC1 compare (duty cycle)
		else if (TC1->COUNT16.INTFLAG.bit.MC1 == 1)
		{
			// call callback
			if (TC1_CaptureCompareCallback) TC1_CaptureCompareCallback();
			// reset the interrupt flag
			TC1->COUNT16.INTFLAG.bit.MC1 = 1;
		}
	}

	// TC2 interrupt handler
	void TC2_Handler()
	{
		using namespace BareCpper::SAMD51;
		// check if interrupt is caused by CC0 compare (period)
		if (TC2->COUNT16.INTFLAG.bit.MC0 == 1)
		{
			// call callback
			if (TC2_PeriodCallback) TC2_PeriodCallback();
			// reset the interrupt flag
			TC2->COUNT16.INTFLAG.bit.MC0 = 1;
		}
		// check if interrupt is caused by CC1 compare (duty cycle)
		else if (TC2->COUNT16.INTFLAG.bit.MC1 == 1)
		{
			// call callback
			if (TC2_CaptureCompareCallback) TC2_CaptureCompareCallback();
			// reset the interrupt flag
			TC2->COUNT16.INTFLAG.bit.MC1 = 1;
		}
	}

	// TC3 interrupt handler
	void TC3_Handler()
	{
		using namespace BareCpper::SAMD51;
		// check if interrupt is caused by CC0 compare (period)
		if (TC3->COUNT16.INTFLAG.bit.MC0 == 1)
		{
			// call callback
			if (TC3_PeriodCallback) TC3_PeriodCallback();
			// reset the interrupt flag
			TC3->COUNT16.INTFLAG.bit.MC0 = 1;
		}
		// check if interrupt is caused by CC1 compare (duty cycle)
		else if (TC3->COUNT16.INTFLAG.bit.MC1 == 1)
		{
			// call callback
			if (TC3_CaptureCompareCallback) TC3_CaptureCompareCallback();
			// reset the interrupt flag
			TC3->COUNT16.INTFLAG.bit.MC1 = 1;
		}
	}
}