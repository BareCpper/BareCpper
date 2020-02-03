#include "include/BareCpper/gpio.hpp"
#include "include/BareCpper/CycleTimer.hpp"


#include <nrfx.h>

#if 1
namespace BC = BareCpper;
BC::Port<0> port0;

//Port<0>::Pin<0> pin0;
BC::Pin9 encoder1A;
BC::Pin8 encoder1B;
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
    BC::CycleTimer timer;
    timer.reset();

    const uint32_t tickbase = timer.count();

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
    const uint32_t tick0 = timer.count()- tickbase;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    const uint32_t tick1 = timer.count() - tickbase;
    BC::delay(timer, BC::CycleTimer::msToTicks(100)); // wait for a second
    const uint32_t tick2 = timer.count() - tickbase;
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    const uint32_t tick3 = timer.count() - tickbase;
    BC::delay(timer, BC::CycleTimer::msToTicks(100)); // wait for a second
    const uint32_t tick4 = timer.count() - tickbase;

    char buff[64];
    snprintf( buff, sizeof(buff), "%u, %u, %u, %u\n"
        , tick1, tick2, tick3, tick4 );
    Serial.print(buff);


}
