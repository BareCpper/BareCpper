#if ARDUINO
#include "BareCpper/include/BareCpper/ATsamd/TwiAsync_ATsamd5x.hpp"
#else
#include "BareCpper/ATsamd/TwiAsync_ATsamd5x.hpp"
#endif

extern "C"
{
  void SERCOM2_0_Handler(void)
  {
    BareCpper::TwiAsync::SERCOM2_Handler();
  }

  void SERCOM2_1_Handler(void)
  {
    BareCpper::TwiAsync::SERCOM2_Handler();
  }

  void SERCOM2_2_Handler(void)
  {
    BareCpper::TwiAsync::SERCOM2_Handler();
  }

  void SERCOM2_3_Handler(void)
  {
    BareCpper::TwiAsync::SERCOM2_Handler();
  }
}