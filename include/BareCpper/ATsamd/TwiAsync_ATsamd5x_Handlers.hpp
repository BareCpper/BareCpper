//INCLUDE THIS FILE ONLY ONCE, IN ONE SOURCE FILE IN YOUR PROJECT
//ENSURE THAT THE TWI HEADER IS ALSO INCLUDED
// multiple inclusions problem
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