#ifndef BARECPPER_GPIO_ATSAM5X_H_
#define BARECPPER_GPIO_ATSAM5X_H_

#ifndef BARECPPER_GPIO_H_
#  error "Include <BareCpper/Gpio.hpp> instead of this file."
#endif

#include <cassert>

#include "Common_ATsamd5x.hpp"

namespace BareCpper {

    template<std::size_t PortIndex> 
    template<std::size_t PinIndex>
    struct PortPins<PortIndex>::Pin ///< @todo limit Valid port indices to supported range in GCC 'compatible' manner /**<PinIndex, Valid<(0 <= PinIndex && PinIndex <= 31)>*/
    {
        //struct Functions;

        constexpr static uint8_t port = PortIndex;
        constexpr static uint8_t pin = PinIndex;
        constexpr static PortRegister_t mask = (PortRegister_t(1U) << PinIndex);

        using Input = InputPin<PortPins<PortIndex>::Pin<PinIndex> >;
    };
  
    template<>
    struct Port<0> : AllPortPins<0/*Port*/
        ,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15
        , 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31> {};
    using Port0 = Port<0>;
    using PortA = Port0;

    template<>
    struct Port<1> : AllPortPins<1/*Port*/
        , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
        , 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31> {};
    using Port1 = Port<1>;
    using PortB = Port1;

    template<>
    struct Port<2> : AllPortPins<2/*Port*/
        , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
        , 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31> {};
    using Port2 = Port<2>;
    using PortC = Port2;

    template<>
    struct Port<3> : AllPortPins<3/*Port*/
        , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
        , 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31> {};
    using Port3 = Port<3>;
    using PortD = Port3;

    /** Multiplexed peripheral pins
    * G= 48, J=64, N=100, P=120/128
    */
    SAM_G_if(using PB03 = PortB::Pin< 3>);
    SAM_G_if(using PA00 = PortA::Pin< 0>);
    SAM_G_if(using PA01 = PortA::Pin< 1>);
    SAM_N_if(using PC00 = PortC::Pin< 0>);
    SAM_N_if(using PC01 = PortC::Pin< 1>);
    SAM_N_if(using PC02 = PortC::Pin< 2>);
    SAM_N_if(using PC03 = PortC::Pin< 3>);
    SAM_G_if(using PA02 = PortA::Pin< 2>);
    SAM_G_if(using PA03 = PortA::Pin< 3>);
    SAM_J_if(using PB04 = PortB::Pin< 4>);
    SAM_J_if(using PB05 = PortB::Pin< 5>);
    SAM_P_if(using PD00 = PortD::Pin< 0>);
    SAM_P_if(using PD00 = PortD::Pin< 0>);
    SAM_J_if(using PB06 = PortB::Pin< 6>);
    SAM_J_if(using PB07 = PortB::Pin< 7>);
    SAM_G_if(using PB08 = PortB::Pin< 8>);
    SAM_G_if(using PB09 = PortB::Pin< 9>);
    SAM_G_if(using PA04 = PortA::Pin< 4>);
    SAM_G_if(using PA05 = PortA::Pin< 5>);
    SAM_G_if(using PA06 = PortA::Pin< 6>);
    SAM_G_if(using PA07 = PortA::Pin< 7>);
    SAM_P_if(using PC04 = PortC::Pin< 4>);
    SAM_N_if(using PC05 = PortC::Pin< 5>);
    SAM_N_if(using PC06 = PortC::Pin< 6>);
    SAM_N_if(using PC07 = PortC::Pin< 7>);
    SAM_G_if(using PA08 = PortA::Pin< 8>);
    SAM_G_if(using PA09 = PortA::Pin< 9>);
    SAM_G_if(using PA10 = PortA::Pin<10>);
    SAM_G_if(using PA11 = PortA::Pin<11>);
    SAM_G_if(using PB10 = PortB::Pin<10>);
    SAM_G_if(using PB11 = PortB::Pin<11>);
    SAM_J_if(using PB12 = PortB::Pin<12>);
    SAM_J_if(using PB13 = PortB::Pin<13>);
    SAM_J_if(using PB14 = PortB::Pin<14>);
    SAM_J_if(using PB15 = PortB::Pin<15>);
    SAM_P_if(using PD08 = PortD::Pin< 8>);
    SAM_P_if(using PD09 = PortD::Pin< 9>);
    SAM_P_if(using PD10 = PortD::Pin<10>);
    SAM_P_if(using PD11 = PortD::Pin<11>);
    SAM_P_if(using PD12 = PortD::Pin<12>);
    SAM_N_if(using PC10 = PortC::Pin<10>);
    SAM_N_if(using PC11 = PortC::Pin<11>);
    SAM_N_if(using PC12 = PortC::Pin<12>);
    SAM_N_if(using PC13 = PortC::Pin<13>);
    SAM_N_if(using PC14 = PortC::Pin<14>);
    SAM_N_if(using PC15 = PortC::Pin<15>);
    SAM_G_if(using PA12 = PortA::Pin<12>);
    SAM_G_if(using PA13 = PortA::Pin<13>);
    SAM_G_if(using PA14 = PortA::Pin<14>);
    SAM_G_if(using PA15 = PortA::Pin<15>);
    SAM_G_if(using PA16 = PortA::Pin<16>);
    SAM_G_if(using PA17 = PortA::Pin<17>);
    SAM_G_if(using PA18 = PortA::Pin<18>);
    SAM_G_if(using PA19 = PortA::Pin<19>);
    SAM_N_if(using PC16 = PortC::Pin<16>);
    SAM_N_if(using PC17 = PortC::Pin<17>);
    SAM_N_if(using PC18 = PortC::Pin<18>);
    SAM_N_if(using PC19 = PortC::Pin<19>);
    SAM_N_if(using PC20 = PortC::Pin<20>);
    SAM_N_if(using PC21 = PortC::Pin<21>);
    SAM_P_if(using PC22 = PortC::Pin<22>);
    SAM_P_if(using PC23 = PortC::Pin<23>);
    SAM_P_if(using PD20 = PortD::Pin<20>);
    SAM_P_if(using PD21 = PortD::Pin<21>);
    SAM_J_if(using PB16 = PortB::Pin<16>);
    SAM_J_if(using PB17 = PortB::Pin<17>);
    SAM_N_if(using PB18 = PortB::Pin<18>);
    SAM_N_if(using PB19 = PortB::Pin<19>);
    SAM_N_if(using PB20 = PortB::Pin<20>);
    SAM_G_if(using PA20 = PortA::Pin<20>);
    SAM_G_if(using PA21 = PortA::Pin<21>);
    SAM_G_if(using PA22 = PortA::Pin<22>);
    SAM_G_if(using PA23 = PortA::Pin<23>);
    SAM_G_if(using PA24 = PortA::Pin<24>);
    SAM_G_if(using PA25 = PortA::Pin<25>);
    SAM_G_if(using PB22 = PortB::Pin<22>);
    SAM_G_if(using PB23 = PortB::Pin<23>);
    SAM_N_if(using PB24 = PortB::Pin<24>);
    SAM_N_if(using PB25 = PortB::Pin<25>);
    SAM_P_if(using PB26 = PortB::Pin<26>);
    SAM_P_if(using PB27 = PortB::Pin<27>);
    SAM_P_if(using PB28 = PortB::Pin<28>);
    SAM_P_if(using PB29 = PortB::Pin<29>);
    SAM_N_if(using PC24 = PortC::Pin<24>);
    SAM_N_if(using PC25 = PortC::Pin<25>);
    SAM_N_if(using PC26 = PortC::Pin<26>);
    SAM_N_if(using PC27 = PortC::Pin<27>);
    SAM_N_if(using PC28 = PortC::Pin<28>);
    SAM_G_if(using PA27 = PortA::Pin<27>);
    //SAM_G_if(using RESET_N = RESET_N);
    SAM_G_if(using PA30 = PortA::Pin<30>);
    SAM_G_if(using PA31 = PortA::Pin<31>);
    SAM_J_if(using PB30 = PortB::Pin<30>);
    SAM_J_if(using PB31 = PortB::Pin<31>);
    SAM_P_if(using PC30 = PortC::Pin<30>);
    SAM_P_if(using PC31 = PortC::Pin<31>);
    SAM_J_if(using PB00 = PortB::Pin< 0>);
    SAM_J_if(using PB01 = PortB::Pin< 1>);
    SAM_G_if(using PB02 = PortB::Pin< 2>);


    enum class Peripheral : uint8_t
    {
          Off = 0xFF
        , A = 0x0
        , B = 0x1
        , C = 0x2
        , D = 0x3
        , E = 0x4
        , F = 0x5
        , G = 0x6
        , H = 0x7
        , I = 0x8
        , J = 0x9
        , K = 0xA
        , L = 0xB
        , M = 0xC
        , N = 0xD
    };

#if 0
    template<Peripheral id>
    struct PeripheralMux;

    template<>
    struct PeripheralMux<C>
    {
        using Pins_t = Pins<PC04,PC05,PC06,PC07,PA08,PA09 
    };
#endif

#if 0
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


    /** @{ Other pins */
    //TODO
    /** @} */

#endif

#if 0
    template<> template<>
    struct PortPins<0>::Pin< >
    {
        static const std::size_t foo = 222;
    };

    template<> template<>
    struct PortPins<0>::Pin<2>
    {
        static const std::size_t foo = 333;
    };
#endif


    /** Hardware Uart 0 **/
    //class UartRx0 : public Port::D::Pin0 {};
    //class UartTx0 : public Port::D::Pin1 {};

    /** Hardware SPI 0 **/
    //class SpiMosi0 : public Port::B::Pin3 {};
    //class SpiMiso0 : public Port::B::Pin4 {}; //< SPI Master-In Slave-Out
    //class SpiSck0 : public Port::B::Pin5 {};


    template<typename Pin_t>
    void gpioEnableInput( const Pin_t& pin )
    {
        using ::Port;
        PORT->Group[pin.port].PINCFG[pin.pinport].bit.INEN = true;
    }

    template<typename Pin_t>
    void gpioDirectionIn( const Pin_t& pin )
    {
        using ::Port;
        PORT->Group[pin.port].DIRCLR.reg = (1 << pin.pin); // clear DIR
    }

    template<typename Pin_t>
    void gpioDirectionOut( const Pin_t& pin )
    {
        using ::Port;
        PORT->Group[pin.port].DIRSET.reg = (1 << pin.pin); // Set DIR
    }

    template<typename Pin_t>
    void gpioPullEnable( const Pin_t& pin )
    {
        using ::Port;
        PORT->Group[pin.port].PINCFG[pin.pin].bit.PULLEN = true;
    }

    template<typename Pin_t>
    void gpioPullDisable( const Pin_t& pin )
    {
        using ::Port;
        PORT->Group[pin.port].PINCFG[pin.pin].bit.PULLEN = false;
    }

    template<typename Pin_t>
    void gpioOutHigh( const Pin_t& pin )
    {
        using ::Port;
        PORT->Group[pin.port].OUTSET.reg = pin.mask; // Drive high
    }

    template<typename Pin_t>
    void gpioOutLow( const Pin_t& pin )
    {
        using ::Port;
        PORT->Group[pin.port].OUTCLR.reg = pin.mask; // Drive low
    }

    template<typename Pin_t>
    void gpioPullUp( const Pin_t& pin )
    {
        gpioDirectionIn( pin );
        gpioPullEnable( pin );
        gpioOutHigh( pin ); ///< OUT controls Pull High/Low
    }

    template<typename Pin_t>
    void gpioPullDown( const Pin_t& pin )
    {
        gpioDirectionIn( pin );
        gpioPullEnable( pin );
        gpioOutLow( pin ); ///< OUT controls Pull High/Low
    }

    template<typename Pin_t>
    void gpioPullOff( const Pin_t& pin )
    {
        gpioPullDisable( pin );
    }

    template<typename Pin_t>
    constexpr Peripheral spiPinPeripheral(const Pin_t& pin, const uint8_t sercomIndex)
    {
        assert(sercomIndex == 1 || sercomIndex == 0 || sercomIndex == 4 ); //< 'todo A proper impl!

        switch (sercomIndex)
        {
        case 0:
            return std::is_same_v<Pin_t, PA06> ? Peripheral::D :
                   std::is_same_v<Pin_t, PA04> ? Peripheral::C :
                   std::is_same_v<Pin_t, PA05> ? Peripheral::C :
                                                 Peripheral::Off;
        case 1:
            return std::is_same_v<Pin_t, PB22> ? Peripheral::C :
                   std::is_same_v<Pin_t, PB23> ? Peripheral::C :
                   std::is_same_v<Pin_t, PA17> ? Peripheral::C :
                                                 Peripheral::Off;
        case 4:
            return std::is_same_v<Pin_t, PB08> ? Peripheral::D :
                   std::is_same_v<Pin_t, PB11> ? Peripheral::D :
                   std::is_same_v<Pin_t, PB09> ? Peripheral::D :
                   Peripheral::Off;
        default: 
            return Peripheral::Off;
        }

    }

    template<typename Pin_t, typename PlatformOption_t>
    void gpioFunction(const Pin_t& pin, Function::Spi_t, const PlatformOption_t& platformOptions = {})
    {
        constexpr Peripheral peripheral = spiPinPeripheral(pin, platformOptions.sercomIndex);
        static_assert(peripheral != Peripheral::Off);

        using ::Port; //< Disambiguate Sam.h vs BareCpper::Gpio

        PORT->Group[pin.port].PINCFG[pin.pin].bit.PMUXEN = (peripheral != Peripheral::Off); //< 0xF is maximum valid function

        if (pin.pin & 0x1) // Odd numbered pin 
        {
            PORT->Group[pin.port].PMUX[pin.pin / 2].bit.PMUXO = static_cast<uint8_t>(peripheral);
        }
        else // Even numbered pin
        {
            PORT->Group[pin.port].PMUX[pin.pin / 2].bit.PMUXE = static_cast<uint8_t>(peripheral);
        }
    }


    //using Imu_Miso = BareCpper::PB22; //< PIN_SPI_MISO         (23u)
   // using Imu_Mosi = BareCpper::PB23; //< PIN_SPI_MOSI         (24u)
    //using Imu_Sck = BareCpper::PA17; //< PIN_SPI_SCK          (25u)
    //using Imu_ChipSelect = BareCpper::PA22; //< IMU_CS    12

} //END: BareCopper

#endif