#if ARDUINO
#include "BareCpper/include/BareCpper/ATsamd/TwiAsync_ATsamd5x.hpp"
#else
#include "BareCpper/ATsamd/TwiAsync_ATsamd5x.hpp"
#endif

#if __has_include(<component-version.h>) 
#include <component-version.h> //< Atmel SAM version defines
#else
#define COMPONENT_VERSION_MAJOR 1 //< Arduino has v1 headers
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
#if COMPONENT_VERSION_MAJOR == 1
  void SERCOM2_3_Handler(void)
#elif COMPONENT_VERSION_MAJOR == 2
  void SERCOM2_OTHER_Handler(void)
#endif
  {
    BareCpper::TwiAsync::SERCOM2_Handler();
  }
}