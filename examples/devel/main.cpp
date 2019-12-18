#include <cstdint>
#include <tuple>
#include <utility>


#define _VARIANT_FEATHER52832_
#include "BareCpper/gpio.hpp"
using namespace BareCpper;

int main()
{
    // Port0::Pin0 pin0;
    // Port0::Pin31 pin31;

    Pin9::Input encoder1A;
    Pin8::Input encoder1B;
    Pins<Pin8, Pin9> encoderPins; //< Both mins as zero-shifted mask

    static_assert( mask(encoder1A) != mask(encoder1B), "mask(pin) != mask(pinB) implementation error" );
    static_assert( (mask(encoder1A) | mask(encoder1B)) == mask(encoderPins), "mask(Pins) implementation error" );
    //Pins<Pin8, Pin9>::Input::Pin31

    //Port0::Pin0 pin;
    PortRegister_t state = mask(encoderPins);
   // PortRegister_t statePack = encoderPins.readPacked();

 //BareCpper::Pins<Pin8,Pin9> encoderPins;

   // Port0::Pin<44> v;
     //std::cout << pin31.index() << '-';
    return state;
}