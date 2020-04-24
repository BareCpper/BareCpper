#ifndef BARECPPER_GPIO_ATSAM5X_H_
#define BARECPPER_GPIO_ATSAM5X_H_

#include "../Gpio.hpp" //< 'temp?

#ifndef BARECPPER_GPIO_H_
#  error "Include <BareCpper/Gpio.hpp> instead of this file."
#endif

#include "sam.h"

#if   defined(__SAMD51G18A__) || defined(__ATSAMD51G18A__) || defined(__SAMD51G19A__) || defined(__ATSAMD51G19A__)
#   define SAM_IC_PINCOUNT 48 ///< VQFN 48 
#elif defined(__SAMD51J18A__) || defined(__ATSAMD51J18A__) || defined(__SAMD51J19A__) || defined(__ATSAMD51J19A__) || defined(__SAMD51J20A__) || defined(__ATSAMD51J20A__)
#   define SAM_IC_PINCOUNT 64 ///< TQFP/VQFN/WLCSP 64 
#elif defined(__SAMD51N19A__) || defined(__ATSAMD51N19A__) || defined(__SAMD51N20A__) || defined(__ATSAMD51N20A__)
#   define SAM_IC_PINCOUNT 100 ///< TQFP 100
#elif defined(__SAMD51P19A__) || defined(__ATSAMD51P19A__) || defined(__SAMD51P20A__) || defined(__ATSAMD51P20A__)
#   if SAM_IC_TQFP
#       define SAM_IC_PINCOUNT 128 ///< TQFP 128
#   else 
#       define SAM_IC_PINCOUNT 120 ///< TFBGA 120
#   endif
#else
#   error Library does not support the specified device.
#endif

#if SAM_IC_PINCOUNT >= 48
#   define SAM_G( T_ ) T_
#else
#   define SAM_G( T_ )
#endif
#if SAM_IC_PINCOUNT >= 64
#   define SAM_J( T_ ) T_
#else
#   define SAM_J( T_ )
#endif
#if SAM_IC_PINCOUNT >= 100
#   define SAM_N( T_ ) T_
#else
#   define SAM_N( T_ )
#endif
#if SAM_IC_PINCOUNT >= 120
#   define SAM_P( T_ ) T_
#else
#   define SAM_P( T_ )
#endif

namespace BareCpper {

    template<> template<size_t PinIndex>
    struct PortPins<0>::Pin ///< @todo limit Valid port indices to supported range in GCC 'compatible' manner /**<PinIndex, Valid<(0 <= PinIndex && PinIndex <= 31)>*/
    {
        constexpr static uint8_t port = 0U;
        constexpr static uint8_t pin = PinIndex;
        constexpr static PortRegister_t mask = (PortRegister_t(1U) << PinIndex);

        using Input = InputPin<PortPins<0>::Pin<PinIndex> >;
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
    SAM_G(using PB03 = PortB::Pin< 3>);
    SAM_G(using PA00 = PortA::Pin< 0>);
    SAM_G(using PA01 = PortA::Pin< 1>);
    SAM_N(using PC00 = PortC::Pin< 0>);
    SAM_N(using PC01 = PortC::Pin< 1>);
    SAM_N(using PC02 = PortC::Pin< 2>);
    SAM_N(using PC03 = PortC::Pin< 3>);
    SAM_G(using PA02 = PortA::Pin< 2>);
    SAM_G(using PA03 = PortA::Pin< 3>);
    SAM_J(using PB04 = PortB::Pin< 4>);
    SAM_J(using PB05 = PortB::Pin< 5>);
    SAM_P(using PD00 = PortD::Pin< 0>);
    SAM_P(using PD00 = PortD::Pin< 0>);
    SAM_J(using PB06 = PortB::Pin< 6>);
    SAM_J(using PB07 = PortB::Pin< 7>);
    SAM_G(using PB08 = PortB::Pin< 8>);
    SAM_G(using PB09 = PortB::Pin< 9>);
    SAM_G(using PA04 = PortA::Pin< 4>);
    SAM_G(using PA05 = PortA::Pin< 5>);
    SAM_G(using PA06 = PortA::Pin< 6>);
    SAM_G(using PA07 = PortA::Pin< 7>);
    SAM_P(using PC04 = PortC::Pin< 4>);
    SAM_N(using PC05 = PortC::Pin< 5>);
    SAM_N(using PC06 = PortC::Pin< 6>);
    SAM_N(using PC07 = PortC::Pin< 7>);
    SAM_G(using PA08 = PortA::Pin< 8>);
    SAM_G(using PA09 = PortA::Pin< 9>);
    SAM_G(using PA10 = PortA::Pin<10>);
    SAM_G(using PA11 = PortA::Pin<11>);
    SAM_G(using PB10 = PortB::Pin<10>);
    SAM_G(using PB11 = PortB::Pin<11>);
    SAM_J(using PB12 = PortB::Pin<12>);
    SAM_J(using PB13 = PortB::Pin<13>);
    SAM_J(using PB14 = PortB::Pin<14>);
    SAM_J(using PB15 = PortB::Pin<15>);
    SAM_P(using PD08 = PortD::Pin< 8>);
    SAM_P(using PD09 = PortD::Pin< 9>);
    SAM_P(using PD10 = PortD::Pin<10>);
    SAM_P(using PD11 = PortD::Pin<11>);
    SAM_P(using PD12 = PortD::Pin<12>);
    SAM_N(using PC10 = PortC::Pin<10>);
    SAM_N(using PC11 = PortC::Pin<11>);
    SAM_N(using PC12 = PortC::Pin<12>);
    SAM_N(using PC13 = PortC::Pin<13>);
    SAM_N(using PC14 = PortC::Pin<14>);
    SAM_N(using PC15 = PortC::Pin<15>);
    SAM_G(using PA12 = PortA::Pin<12>);
    SAM_G(using PA13 = PortA::Pin<13>);
    SAM_G(using PA14 = PortA::Pin<14>);
    SAM_G(using PA15 = PortA::Pin<15>);
    SAM_G(using PA16 = PortA::Pin<16>);
    SAM_G(using PA17 = PortA::Pin<17>);
    SAM_G(using PA18 = PortA::Pin<18>);
    SAM_G(using PA19 = PortA::Pin<19>);
    SAM_N(using PC16 = PortC::Pin<16>);
    SAM_N(using PC17 = PortC::Pin<17>);
    SAM_N(using PC18 = PortC::Pin<18>);
    SAM_N(using PC19 = PortC::Pin<19>);
    SAM_N(using PC20 = PortC::Pin<20>);
    SAM_N(using PC21 = PortC::Pin<21>);
    SAM_P(using PC22 = PortC::Pin<22>);
    SAM_P(using PC23 = PortC::Pin<23>);
    SAM_P(using PD20 = PortD::Pin<20>);
    SAM_P(using PD21 = PortD::Pin<21>);
    SAM_J(using PB16 = PortB::Pin<16>);
    SAM_J(using PB17 = PortB::Pin<17>);
    SAM_N(using PB18 = PortB::Pin<18>);
    SAM_N(using PB19 = PortB::Pin<19>);
    SAM_N(using PB20 = PortB::Pin<20>);
    SAM_G(using PA20 = PortA::Pin<20>);
    SAM_G(using PA21 = PortA::Pin<21>);
    SAM_G(using PA22 = PortA::Pin<22>);
    SAM_G(using PA23 = PortA::Pin<23>);
    SAM_G(using PA24 = PortA::Pin<24>);
    SAM_G(using PA25 = PortA::Pin<25>);
    SAM_G(using PB22 = PortB::Pin<22>);
    SAM_G(using PB23 = PortB::Pin<23>);
    SAM_N(using PB24 = PortB::Pin<24>);
    SAM_N(using PB25 = PortB::Pin<25>);
    SAM_P(using PB26 = PortB::Pin<26>);
    SAM_P(using PB27 = PortB::Pin<27>);
    SAM_P(using PB28 = PortB::Pin<28>);
    SAM_P(using PB29 = PortB::Pin<29>);
    SAM_N(using PC24 = PortC::Pin<24>);
    SAM_N(using PC25 = PortC::Pin<25>);
    SAM_N(using PC26 = PortC::Pin<26>);
    SAM_N(using PC27 = PortC::Pin<27>);
    SAM_N(using PC28 = PortC::Pin<28>);
    SAM_G(using PA27 = PortA::Pin<27>);
    //SAM_G(using RESET_N = RESET_N);
    SAM_G(using PA30 = PortA::Pin<30>);
    SAM_G(using PA31 = PortA::Pin<31>);
    SAM_J(using PB30 = PortB::Pin<30>);
    SAM_J(using PB31 = PortB::Pin<31>);
    SAM_P(using PC30 = PortC::Pin<30>);
    SAM_P(using PC31 = PortC::Pin<31>);
    SAM_J(using PB00 = PortB::Pin< 0>);
    SAM_J(using PB01 = PortB::Pin< 1>);
    SAM_G(using PB02 = PortB::Pin< 2>);


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