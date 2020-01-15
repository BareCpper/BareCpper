#include <cstdint>
#include <tuple>
#include "BareCpper/gpio.hpp"
using namespace BareCpper;

/** Use clock-cycle counter to perform high precision delay to blink an LED 
* @note Any Counting-Timer object may be used to generate a delay 
*/
BC::CycleTimer timer;
bool ledState = false; //< Toggle LED on/off

// Setup to drive builtin LED
void setup() 
{
    pinMode(LED_BUILTIN, OUTPUT);
}

// Runs over and over again indefinitly
void loop()
{
    ledState = !ledState; //< Toggle LED state
    digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW); //< Set Led state

    const uint32_t delayCycleCount = BC::CycleTimer::msToTicks(100); //< Delay for N clock ticks
    BC::delay(timer, delayCycleCount); // Delay for a 100ms
}
