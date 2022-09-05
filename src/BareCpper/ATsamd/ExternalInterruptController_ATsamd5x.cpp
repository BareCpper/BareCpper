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
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(0);
}

void EIC_1_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(1);
}

void EIC_2_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(2);
}

void EIC_3_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(3);
}

void EIC_4_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(4);
}

void EIC_5_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(5);
}

void EIC_6_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(6);
}

void EIC_7_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(7);
}

void EIC_8_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(8);
}

void EIC_9_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(9);
}

void EIC_10_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(10);
}

void EIC_11_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(11);
}

void EIC_12_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(12);
}

void EIC_13_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(13);
}

void EIC_14_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(14);
}

void EIC_15_Handler()
{
  BareCpper::SAMD51::ExternalInterruptController::IrqHandler(15);
}

}