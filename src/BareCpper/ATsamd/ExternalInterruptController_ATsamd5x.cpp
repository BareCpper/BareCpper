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
void EIC_0_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(0);
}

void EIC_1_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(1);
}

void EIC_2_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(2);
}

void EIC_3_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(3);
}

void EIC_4_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(4);
}

void EIC_5_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(5);
}

void EIC_6_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(6);
}

void EIC_7_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(7);
}

void EIC_8_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(8);
}

void EIC_9_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(9);
}

void EIC_10_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(10);
}

void EIC_11_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(11);
}

void EIC_12_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(12);
}

void EIC_13_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(13);
}

void EIC_14_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(14);
}

void EIC_15_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::irqHandler(15);
}

}