/*
 * iom329p.h
 *
 * Created: 02/07/2016 22:53:56
 *  Author: CraigH
 */
 /* barecpper/iom328p.h - definitions for ATmega328P. */
#ifndef BARECPPER_GPIO_NRF52832_H_
#define BARECPPER_GPIO_NRF52832_H_

#ifndef BARECPPER_GPIO_H_
#  error "Include <barecpper/gpio.h> instead of this file."
#endif

// @todo
//#if !_AVR_IOM328P_H_
//#  error "AVR library error, please check and update accordingly."
//#endif

namespace BareCpper {

    template<> template<size_t PinIndex>
    struct PortPins<0>::Pin ///< @todo limit Valid port indices to supported range in GCC 'compatible' manner /**<PinIndex, Valid<(0 <= PinIndex && PinIndex <= 31)>*/
    {
        constexpr static uint8_t portIndex = 0U;
        constexpr static uint8_t index = PinIndex;

        using Input = InputPin<PortPins<0>::Pin< PinIndex > >;
    };

    template<>
    struct Port<0> : AllPortPins<0/*Port*/
        , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
        , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
        , 20, 21, 22, 23, 24, 25, 26, 27, 28, 29
        , 30, 31>
    {
    };
    using Port0 = Port<0>;

    /** Mapping from ardunio pin index to Port::Pin
    */
    using Pin0  = Port0::Pin0;  // xtal 1
    using Pin1  = Port0::Pin1;  // xtal 2
    using Pin2  = Port0::Pin2;  // a0
    using Pin3  = Port0::Pin3;  // a1
    using Pin4  = Port0::Pin4;  // a2
    using Pin5  = Port0::Pin5;  // a3
    using Pin6  = Port0::Pin6;  // TXD
    using Pin7  = Port0::Pin7;  // GPIO #7
    using Pin8  = Port0::Pin8;  // RXD
    using Pin9  = Port0::Pin9;  // NFC1
    using Pin10 = Port0::Pin10; // NFC2
    using Pin11 = Port0::Pin11; // GPIO11
    using Pin12 = Port0::Pin12; // SCK
    using Pin13 = Port0::Pin13; // MOSI
    using Pin14 = Port0::Pin14; // MISO
    using Pin15 = Port0::Pin15; // GPIO #15
    using Pin16 = Port0::Pin16; // GPIO #16
    using Pin17 = Port0::Pin17; // LED #1 (red)
    using Pin18 = Port0::Pin18; // SWO
    using Pin19 = Port0::Pin19; // LED #2 (blue)
    using Pin20 = Port0::Pin20; // DFU
    using Pin21 = Port0::Pin21; // Reset
    using Pin22 = Port0::Pin22; // Factory Reset
    using Pin23 = Port0::Pin23; // N/A
    using Pin24 = Port0::Pin24; // N/A
    using Pin25 = Port0::Pin25; // SDA
    using Pin26 = Port0::Pin26; // SCL
    using Pin27 = Port0::Pin27; // GPIO #27
    using Pin28 = Port0::Pin28; // A4
    using Pin29 = Port0::Pin29; // A5
    using Pin30 = Port0::Pin30; // A6
    using Pin31 = Port0::Pin31; // A7

    /** @{ Analog pins  */
    using PinA0 = Pin2;
    using PinA1 = Pin3;
    using PinA2 = Pin4;
    using PinA3 = Pin5;
    using PinA4 = Pin28;
    using PinA5 = Pin29;
    using PinA6 = Pin30;
    using PinA7 = Pin31;
    /** @} */

    /** @{ LEDs */
    using Led0 = Pin17;
    using Led1 = Pin19;

    using LedBuiltin = Led0;
    using LedConn = Led1;
    
    using LedRed = Led0;
    using LedBlue = Led1;
    /** @} */

    /** @{ Other pins */
    using PinAREF = Pin24;
    using PinVBAT = PinA7;
    using PinNFC1 = Pin9;
    using PinNFC2 = Pin10;
    /** @} */


#if 0
    template<> template<>
    struct PortPins<0>::Pin<0>
    {
        static const size_t foo = 222;
    };

    template<> template<>
    struct PortPins<0>::Pin<2>
    {
        static const size_t foo = 333;
    };
#endif


    /** Hardware Uart 0 **/
    //class UartRx0 : public Port::D::Pin0 {};
    //class UartTx0 : public Port::D::Pin1 {};

    /** Hardware SPI 0 **/
    //class SpiMosi0 : public Port::B::Pin3 {};
    //class SpiMiso0 : public Port::B::Pin4 {}; //< SPI Master-In Slave-Out
    //class SpiSck0 : public Port::B::Pin5 {};

} //END: BareCopper

#endif