#if ARDUINO
#include "BareCpper/include/BareCpper/ATsamd/ExternalInterruptController_ATsamd5x.hpp"
#else
#include "BareCpper/ATsamd/ExternalInterruptController_ATsamd5x.hpp"
#endif

namespace BareCpper
{
  namespace SAMD51
  {
    typename ExternalInterruptController::CallbackT ExternalInterruptController::callbacks_[ExternalInterruptController::NumberChannels];
  }
}

extern "C"
{
// define interrupt handlers
#if COMPONENT_VERSION_MAJOR == 1
void EIC_0_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_0_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(0);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_1_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_1_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(1);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_2_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_2_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(2);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_3_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_3_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(3);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_4_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_4_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(4);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_5_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_5_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(5);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_6_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_6_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(6);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_7_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_7_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(7);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_8_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_8_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(8);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_9_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_9_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(9);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_10_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_10_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(10);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_11_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_11_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(11);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_12_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_12_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(12);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_13_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_13_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(13);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_14_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_14_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(14);
}

#if COMPONENT_VERSION_MAJOR == 1
void EIC_15_Handler()
#elif COMPONENT_VERSION_MAJOR == 2
void EIC_EXTINT_15_Handler()
#endif
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(15);
}

}