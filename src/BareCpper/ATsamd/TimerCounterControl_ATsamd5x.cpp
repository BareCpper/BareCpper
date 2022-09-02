#include "BareCpper/include/BareCpper/ATsamd/TimerCounterControl_ATsamd5x.hpp"

namespace BareCpper
{
	namespace SAMD51
	{
		// Hold functions to be called when overflow and match/compare interrupts fire
		std::array<std::function<void()>, numTccDevices> TCC_OverflowCallback;
		std::array<std::array<std::function<void()>, 5>, numTccDevices> TCC_McCallback;
	}
}

///@todo refactor, duplicate code
extern "C"
{
  // TCC0 overflow interrupt handler
  void TCC0_OTHER_Handler()
  {
    using namespace BareCpper::SAMD51;
    if(TCC0->INTFLAG.bit.OVF)
    {
      // call the callback
      if(TCC_OverflowCallback[0]) TCC_OverflowCallback[0]();
      // reset the interrupt flag
      TCC0->INTFLAG.bit.OVF = 1;
    }
  }

  // TCC1 overflow interrupt handler
  void TCC1_OTHER_Handler()
  {
    using namespace BareCpper::SAMD51;
    if(TCC1->INTFLAG.bit.OVF)
    {
      // call the callback
      if(TCC_OverflowCallback[1]) TCC_OverflowCallback[1]();
      // reset the interrupt flag
      TCC1->INTFLAG.bit.OVF = 1;
    }
  }

  // TCC2 overflow interrupt handler
  void TCC2_OTHER_Handler()
  {
    using namespace BareCpper::SAMD51;
    if(TCC2->INTFLAG.bit.OVF)
    {
      // call the callback
      if(TCC_OverflowCallback[2]) TCC_OverflowCallback[2]();
      // reset the interrupt flag
      TCC2->INTFLAG.bit.OVF = 1;
    }
  }

  // TCC3 overflow interrupt handler
  void TCC3_OTHER_Handler()
  {
    using namespace BareCpper::SAMD51;
    if(TCC3->INTFLAG.bit.OVF)
    {
      // call the callback
      if(TCC_OverflowCallback[3]) TCC_OverflowCallback[3]();
      // reset the interrupt flag
      TCC0->INTFLAG.bit.OVF = 1;
    }
  }

  // TCC4 overflow interrupt handler
  void TCC4_OTHER_Handler()
  {
    using namespace BareCpper::SAMD51;
    if(TCC4->INTFLAG.bit.OVF)
    {
      // call the callback
      if(TCC_OverflowCallback[4]) TCC_OverflowCallback[4]();
      // reset the interrupt flag
      TCC4->INTFLAG.bit.OVF = 1;
    }
  }

  // Match/Capture interrupt handlers
  ///@todo
}