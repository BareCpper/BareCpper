#include "include/BareCpper/gpio.hpp"
#include "include/BareCpper/CycleCounter.hpp"


#include <nrfx.h>

#if 1
namespace Bare = BareCpper;
Bare::Port<0> port0;

//Port<0>::Pin<0> pin0;
Bare::Pin9 encoder1A;
Bare::Pin8 encoder1B;
//Pins<Pin8, Pin9> encoderPins; //< Both mins as zero-shifted mask
#endif

void setup() {
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(1000000);
}

// the loop function runs over and over again forever
void loop()
{
    Bare::CycleCounter timer;
    timer.reset();


#if 0
    const uint count = 100;
    uint32_t tickN[count];
    for (int i = 0; i < count; ++i)
    {
        tickN[i] = timer.count();
        delay(timer, timer.usToTicks(1) );
    }

    for (int i = 0; i < count; ++i)
    {
        Serial.print(tickN[i] - tickbase);
        Serial.print(',');
    }
#endif

    //64000000 / 10 = 64000(00)
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    const uint32_t tick1 = timer.count();
    Bare::delay(timer, Bare::CycleCounter::msToTicks(100)); // wait for a second
    const uint32_t tick2 = timer.count();
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    const uint32_t tick3 = timer.count();
    Bare::delay(timer, Bare::CycleCounter::msToTicks(100)); // wait for a second
    const uint32_t tick4 = timer.count();

    char buff[64];
    snprintf( buff, sizeof(buff), "%u, %u, %u, %u\n"
        , tick1, tick2, tick3, tick4 );
    Serial.print(buff);


}
