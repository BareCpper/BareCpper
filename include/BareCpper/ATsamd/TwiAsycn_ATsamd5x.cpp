#include "TwiAsync_ATsamd5x.hpp"

extern "C" void SERCOM2_0_Handler( void )
{
    BareCpper::TwiAsync::SERCOM2_Handler();
}
extern "C" void SERCOM2_1_Handler( void )
{
    BareCpper::TwiAsync::SERCOM2_Handler();
}
extern "C" void SERCOM2_2_Handler( void )
{
    BareCpper::TwiAsync::SERCOM2_Handler();
}
extern "C" void SERCOM2_3_Handler( void )
{
    BareCpper::TwiAsync::SERCOM2_Handler();
}