#define _VARIANT_FEATHER52832_
#include "include/BareCpper/gpio.hpp"

#if 1
using namespace BareCpper;
Port<0> port0;

Port<0>::Pin<0> pin0;
//Pin9 encoder1A;
//Pin8 encoder1B;
//Pins<Pin8, Pin9> encoderPins; //< Both mins as zero-shifted mask
#endif

void setup() {
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second
}
