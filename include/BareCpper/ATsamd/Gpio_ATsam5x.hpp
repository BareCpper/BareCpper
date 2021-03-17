#ifndef BARECPPER_GPIO_ATSAM5X_H_
#define BARECPPER_GPIO_ATSAM5X_H_

#ifndef BARECPPER_GPIO_H_
#  error "Include <BareCpper/Gpio.hpp> instead of this file."
#endif

#include <cassert>
#include <array>

#include "Common_ATsamd5x.hpp"

namespace BareCpper {

    template<std::size_t PortIndex> 
    template<std::size_t PinIndex>
    struct PortPins<PortIndex>::Pin ///< @todo limit Valid port indices to supported range in GCC 'compatible' manner /**<PinIndex, Valid<(0 <= PinIndex && PinIndex <= 31)>*/
    {
        static constexpr PinId id()
        { return PinId{ PortIndex, PinIndex }; }

        static constexpr PortRegister_t mask()
        { return PortRegister_t(1U) << PinIndex; }

        template<typename Pin_t>
        constexpr bool operator == (const Pin_t& rhs) const
        { return id() == rhs.id(); }

        template<typename Pin_t>
        constexpr bool operator <(const Pin_t& rhs) const
        { return id() <rhs.id(); }
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

    /// Platform specific namespace for helper functions
    namespace ATsamd5x
    {
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


        //TODO: DUplication..refartor
        /* @param[in] function  The pin function is given by a 32 - bit wide bitfield found in the header files for the device
                                e.g. PINMUX_PA12C_SERCOM2_PAD0
        */
        inline void setPinFunction(const PinId pinId, const uint32_t function)
        {
            using ::Port; //< Disambiguate Sam.h vs BareCpper::Gpio

            PORT->Group[pinId.port].PINCFG[pinId.pin].bit.PMUXEN = (function <= PORT_PMUX_PMUXE_Msk); //< 0xF is maximum valid function

            if (pinId.pin & 0x1) // Odd numbered pin 
            {
                PORT->Group[pinId.port].PMUX[pinId.pin / 2].bit.PMUXO = function;
            }
            else // Even numbered pin
            {
                PORT->Group[pinId.port].PMUX[pinId.pin / 2].bit.PMUXE = function;
            }
        }

        struct SercomMux
        {
            PinId pinId;
            Peripheral peripheral;
            uint8_t sercomIndex;
            uint8_t sercomPad;
        };
        constexpr auto sercomMux()
        {
            /// @todo This list is for ALL pins and not just those supported by a G, J, N, or P variant!
            return std::array{
                  SercomMux{ id<Port<0>::Pin< 4>>(), Peripheral::D, /*Sercom*/0, /*Pad*/0 } // PA04
                , SercomMux{ id<Port<2>::Pin<17>>(), Peripheral::D, /*Sercom*/0, /*Pad*/0 } // PC17
                , SercomMux{ id<Port<0>::Pin< 8>>(), Peripheral::C, /*Sercom*/0, /*Pad*/0 } // PA08
                , SercomMux{ id<Port<1>::Pin<24>>(), Peripheral::C, /*Sercom*/0, /*Pad*/0 } // PB24
                , SercomMux{ id<Port<0>::Pin< 5>>(), Peripheral::D, /*Sercom*/0, /*Pad*/1 } // PA05
                , SercomMux{ id<Port<2>::Pin<16>>(), Peripheral::D, /*Sercom*/0, /*Pad*/1 } // PC16
                , SercomMux{ id<Port<0>::Pin< 9>>(), Peripheral::C, /*Sercom*/0, /*Pad*/1 } // PA09
                , SercomMux{ id<Port<1>::Pin<25>>(), Peripheral::C, /*Sercom*/0, /*Pad*/1 } // PB25
                , SercomMux{ id<Port<0>::Pin< 6>>(), Peripheral::D, /*Sercom*/0, /*Pad*/2 } // PA06
                , SercomMux{ id<Port<2>::Pin<18>>(), Peripheral::D, /*Sercom*/0, /*Pad*/2 } // PC18
                , SercomMux{ id<Port<0>::Pin<10>>(), Peripheral::C, /*Sercom*/0, /*Pad*/2 } // PA10
                , SercomMux{ id<Port<2>::Pin<24>>(), Peripheral::C, /*Sercom*/0, /*Pad*/2 } // PC24
                , SercomMux{ id<Port<0>::Pin< 7>>(), Peripheral::D, /*Sercom*/0, /*Pad*/3 } // PA07
                , SercomMux{ id<Port<2>::Pin<19>>(), Peripheral::D, /*Sercom*/0, /*Pad*/3 } // PC19
                , SercomMux{ id<Port<0>::Pin<11>>(), Peripheral::C, /*Sercom*/0, /*Pad*/3 } // PA11
                , SercomMux{ id<Port<2>::Pin<25>>(), Peripheral::C, /*Sercom*/0, /*Pad*/3 } // PC25
                , SercomMux{ id<Port<0>::Pin< 0>>(), Peripheral::D, /*Sercom*/1, /*Pad*/0 } // PA00
                , SercomMux{ id<Port<0>::Pin<16>>(), Peripheral::C, /*Sercom*/1, /*Pad*/0 } // PA16
                , SercomMux{ id<Port<2>::Pin<22>>(), Peripheral::C, /*Sercom*/1, /*Pad*/0 } // PC22
                , SercomMux{ id<Port<2>::Pin<27>>(), Peripheral::C, /*Sercom*/1, /*Pad*/0 } // PC27
                , SercomMux{ id<Port<0>::Pin< 1>>(), Peripheral::D, /*Sercom*/1, /*Pad*/1 } // PA01
                , SercomMux{ id<Port<0>::Pin<17>>(), Peripheral::C, /*Sercom*/1, /*Pad*/1 } // PA17
                , SercomMux{ id<Port<2>::Pin<23>>(), Peripheral::C, /*Sercom*/1, /*Pad*/1 } // PC23
                , SercomMux{ id<Port<2>::Pin<28>>(), Peripheral::C, /*Sercom*/1, /*Pad*/1 } // PC28
                , SercomMux{ id<Port<0>::Pin<30>>(), Peripheral::D, /*Sercom*/1, /*Pad*/2 } // PA30
                , SercomMux{ id<Port<0>::Pin<18>>(), Peripheral::C, /*Sercom*/1, /*Pad*/2 } // PA18
                , SercomMux{ id<Port<1>::Pin<22>>(), Peripheral::C, /*Sercom*/1, /*Pad*/2 } // PB22
                , SercomMux{ id<Port<3>::Pin<20>>(), Peripheral::C, /*Sercom*/1, /*Pad*/2 } // PD20
                , SercomMux{ id<Port<0>::Pin<31>>(), Peripheral::D, /*Sercom*/1, /*Pad*/3 } // PA31
                , SercomMux{ id<Port<0>::Pin<19>>(), Peripheral::C, /*Sercom*/1, /*Pad*/3 } // PA19
                , SercomMux{ id<Port<1>::Pin<23>>(), Peripheral::C, /*Sercom*/1, /*Pad*/3 } // PB23
                , SercomMux{ id<Port<3>::Pin<21>>(), Peripheral::C, /*Sercom*/1, /*Pad*/3 } // PD21
                , SercomMux{ id<Port<0>::Pin< 9>>(), Peripheral::D, /*Sercom*/2, /*Pad*/0 } // PA09
                , SercomMux{ id<Port<1>::Pin<25>>(), Peripheral::D, /*Sercom*/2, /*Pad*/0 } // PB25
                , SercomMux{ id<Port<0>::Pin<12>>(), Peripheral::C, /*Sercom*/2, /*Pad*/0 } // PA12
                , SercomMux{ id<Port<1>::Pin<26>>(), Peripheral::C, /*Sercom*/2, /*Pad*/0 } // PB26
                , SercomMux{ id<Port<0>::Pin< 8>>(), Peripheral::D, /*Sercom*/2, /*Pad*/1 } // PA08
                , SercomMux{ id<Port<1>::Pin<24>>(), Peripheral::D, /*Sercom*/2, /*Pad*/1 } // PB24
                , SercomMux{ id<Port<0>::Pin<13>>(), Peripheral::C, /*Sercom*/2, /*Pad*/1 } // PA13
                , SercomMux{ id<Port<1>::Pin<27>>(), Peripheral::C, /*Sercom*/2, /*Pad*/1 } // PB27
                , SercomMux{ id<Port<0>::Pin<10>>(), Peripheral::D, /*Sercom*/2, /*Pad*/2 } // PA10
                , SercomMux{ id<Port<2>::Pin<24>>(), Peripheral::D, /*Sercom*/2, /*Pad*/2 } // PC24
                , SercomMux{ id<Port<1>::Pin<28>>(), Peripheral::C, /*Sercom*/2, /*Pad*/2 } // PB28
                , SercomMux{ id<Port<0>::Pin<14>>(), Peripheral::C, /*Sercom*/2, /*Pad*/2 } // PA14
                , SercomMux{ id<Port<0>::Pin<11>>(), Peripheral::D, /*Sercom*/2, /*Pad*/3 } // PA11
                , SercomMux{ id<Port<2>::Pin<25>>(), Peripheral::D, /*Sercom*/2, /*Pad*/3 } // PC25
                , SercomMux{ id<Port<1>::Pin<29>>(), Peripheral::C, /*Sercom*/2, /*Pad*/3 } // PB29
                , SercomMux{ id<Port<0>::Pin<15>>(), Peripheral::C, /*Sercom*/2, /*Pad*/3 } // PA15
                , SercomMux{ id<Port<0>::Pin<17>>(), Peripheral::D, /*Sercom*/3, /*Pad*/0 } // PA17
                , SercomMux{ id<Port<2>::Pin<23>>(), Peripheral::D, /*Sercom*/3, /*Pad*/0 } // PC23
                , SercomMux{ id<Port<0>::Pin<22>>(), Peripheral::C, /*Sercom*/3, /*Pad*/0 } // PA22
                , SercomMux{ id<Port<1>::Pin<20>>(), Peripheral::C, /*Sercom*/3, /*Pad*/0 } // PB20
                , SercomMux{ id<Port<0>::Pin<16>>(), Peripheral::D, /*Sercom*/3, /*Pad*/1 } // PA16
                , SercomMux{ id<Port<2>::Pin<22>>(), Peripheral::D, /*Sercom*/3, /*Pad*/1 } // PC22
                , SercomMux{ id<Port<0>::Pin<23>>(), Peripheral::C, /*Sercom*/3, /*Pad*/1 } // PA23
                , SercomMux{ id<Port<1>::Pin<21>>(), Peripheral::C, /*Sercom*/3, /*Pad*/1 } // PB21
                , SercomMux{ id<Port<0>::Pin<18>>(), Peripheral::D, /*Sercom*/3, /*Pad*/2 } // PA18
                , SercomMux{ id<Port<0>::Pin<20>>(), Peripheral::D, /*Sercom*/3, /*Pad*/2 } // PA20
                , SercomMux{ id<Port<3>::Pin<20>>(), Peripheral::D, /*Sercom*/3, /*Pad*/2 } // PD20
                , SercomMux{ id<Port<0>::Pin<24>>(), Peripheral::C, /*Sercom*/3, /*Pad*/2 } // PA24
                , SercomMux{ id<Port<0>::Pin<19>>(), Peripheral::D, /*Sercom*/3, /*Pad*/3 } // PA19
                , SercomMux{ id<Port<0>::Pin<21>>(), Peripheral::D, /*Sercom*/3, /*Pad*/3 } // PA21
                , SercomMux{ id<Port<3>::Pin<21>>(), Peripheral::D, /*Sercom*/3, /*Pad*/3 } // PD21
                , SercomMux{ id<Port<0>::Pin<25>>(), Peripheral::C, /*Sercom*/3, /*Pad*/3 } // PA25
                , SercomMux{ id<Port<0>::Pin<13>>(), Peripheral::D, /*Sercom*/4, /*Pad*/0 } // PA13
                , SercomMux{ id<Port<1>::Pin< 8>>(), Peripheral::D, /*Sercom*/4, /*Pad*/0 } // PB08
                , SercomMux{ id<Port<1>::Pin<27>>(), Peripheral::D, /*Sercom*/4, /*Pad*/0 } // PB27
                , SercomMux{ id<Port<1>::Pin<12>>(), Peripheral::C, /*Sercom*/4, /*Pad*/0 } // PB12
                , SercomMux{ id<Port<0>::Pin<12>>(), Peripheral::D, /*Sercom*/4, /*Pad*/1 } // PA12
                , SercomMux{ id<Port<1>::Pin< 9>>(), Peripheral::D, /*Sercom*/4, /*Pad*/1 } // PB09
                , SercomMux{ id<Port<1>::Pin<26>>(), Peripheral::D, /*Sercom*/4, /*Pad*/1 } // PB26
                , SercomMux{ id<Port<1>::Pin<13>>(), Peripheral::C, /*Sercom*/4, /*Pad*/1 } // PB13
                , SercomMux{ id<Port<0>::Pin<14>>(), Peripheral::D, /*Sercom*/4, /*Pad*/2 } // PA14
                , SercomMux{ id<Port<1>::Pin<10>>(), Peripheral::D, /*Sercom*/4, /*Pad*/2 } // PB10
                , SercomMux{ id<Port<1>::Pin<28>>(), Peripheral::D, /*Sercom*/4, /*Pad*/2 } // PB28
                , SercomMux{ id<Port<1>::Pin<14>>(), Peripheral::C, /*Sercom*/4, /*Pad*/2 } // PB14
                , SercomMux{ id<Port<1>::Pin<11>>(), Peripheral::D, /*Sercom*/4, /*Pad*/3 } // PB11
                , SercomMux{ id<Port<1>::Pin<29>>(), Peripheral::D, /*Sercom*/4, /*Pad*/3 } // PB29
                , SercomMux{ id<Port<0>::Pin<15>>(), Peripheral::D, /*Sercom*/4, /*Pad*/3 } // PA15
                , SercomMux{ id<Port<1>::Pin<15>>(), Peripheral::C, /*Sercom*/4, /*Pad*/3 } // PB15
                , SercomMux{ id<Port<0>::Pin<23>>(), Peripheral::D, /*Sercom*/5, /*Pad*/0 } // PA23
                , SercomMux{ id<Port<1>::Pin< 2>>(), Peripheral::D, /*Sercom*/5, /*Pad*/0 } // PB02
                , SercomMux{ id<Port<1>::Pin<31>>(), Peripheral::D, /*Sercom*/5, /*Pad*/0 } // PB31
                , SercomMux{ id<Port<1>::Pin<16>>(), Peripheral::C, /*Sercom*/5, /*Pad*/0 } // PB16
                , SercomMux{ id<Port<0>::Pin<22>>(), Peripheral::D, /*Sercom*/5, /*Pad*/1 } // PA22
                , SercomMux{ id<Port<1>::Pin< 3>>(), Peripheral::D, /*Sercom*/5, /*Pad*/1 } // PB03
                , SercomMux{ id<Port<1>::Pin<30>>(), Peripheral::D, /*Sercom*/5, /*Pad*/1 } // PB30
                , SercomMux{ id<Port<1>::Pin<17>>(), Peripheral::C, /*Sercom*/5, /*Pad*/1 } // PB17
                , SercomMux{ id<Port<0>::Pin<24>>(), Peripheral::D, /*Sercom*/5, /*Pad*/2 } // PA24
                , SercomMux{ id<Port<1>::Pin< 0>>(), Peripheral::D, /*Sercom*/5, /*Pad*/2 } // PB00
                , SercomMux{ id<Port<1>::Pin<22>>(), Peripheral::D, /*Sercom*/5, /*Pad*/2 } // PB22
                , SercomMux{ id<Port<0>::Pin<20>>(), Peripheral::C, /*Sercom*/5, /*Pad*/2 } // PA20
                , SercomMux{ id<Port<1>::Pin<18>>(), Peripheral::C, /*Sercom*/5, /*Pad*/2 } // PB18
                , SercomMux{ id<Port<0>::Pin<25>>(), Peripheral::D, /*Sercom*/5, /*Pad*/3 } // PA25
                , SercomMux{ id<Port<1>::Pin< 1>>(), Peripheral::D, /*Sercom*/5, /*Pad*/3 } // PB01
                , SercomMux{ id<Port<1>::Pin<23>>(), Peripheral::D, /*Sercom*/5, /*Pad*/3 } // PB23
                , SercomMux{ id<Port<0>::Pin<21>>(), Peripheral::C, /*Sercom*/5, /*Pad*/3 } // PA21
                , SercomMux{ id<Port<1>::Pin<19>>(), Peripheral::C, /*Sercom*/5, /*Pad*/3 } // PB19
                , SercomMux{ id<Port<3>::Pin< 9>>(), Peripheral::D, /*Sercom*/6, /*Pad*/0 } // PD09
                , SercomMux{ id<Port<2>::Pin<13>>(), Peripheral::D, /*Sercom*/6, /*Pad*/0 } // PC13
                , SercomMux{ id<Port<2>::Pin< 4>>(), Peripheral::C, /*Sercom*/6, /*Pad*/0 } // PC04
                , SercomMux{ id<Port<2>::Pin<16>>(), Peripheral::C, /*Sercom*/6, /*Pad*/0 } // PC16
                , SercomMux{ id<Port<3>::Pin< 8>>(), Peripheral::D, /*Sercom*/6, /*Pad*/1 } // PD08
                , SercomMux{ id<Port<2>::Pin<12>>(), Peripheral::D, /*Sercom*/6, /*Pad*/1 } // PC12
                , SercomMux{ id<Port<2>::Pin< 5>>(), Peripheral::C, /*Sercom*/6, /*Pad*/1 } // PC05
                , SercomMux{ id<Port<2>::Pin<17>>(), Peripheral::C, /*Sercom*/6, /*Pad*/1 } // PC17
                , SercomMux{ id<Port<2>::Pin<14>>(), Peripheral::D, /*Sercom*/6, /*Pad*/2 } // PC14
                , SercomMux{ id<Port<3>::Pin<10>>(), Peripheral::D, /*Sercom*/6, /*Pad*/2 } // PD10
                , SercomMux{ id<Port<2>::Pin< 6>>(), Peripheral::C, /*Sercom*/6, /*Pad*/2 } // PC06
                , SercomMux{ id<Port<2>::Pin<10>>(), Peripheral::C, /*Sercom*/6, /*Pad*/2 } // PC10
                , SercomMux{ id<Port<2>::Pin<18>>(), Peripheral::C, /*Sercom*/6, /*Pad*/2 } // PC18
                , SercomMux{ id<Port<2>::Pin<15>>(), Peripheral::D, /*Sercom*/6, /*Pad*/3 } // PC15
                , SercomMux{ id<Port<3>::Pin<11>>(), Peripheral::D, /*Sercom*/6, /*Pad*/3 } // PD11
                , SercomMux{ id<Port<2>::Pin< 7>>(), Peripheral::C, /*Sercom*/6, /*Pad*/3 } // PC07
                , SercomMux{ id<Port<2>::Pin<11>>(), Peripheral::C, /*Sercom*/6, /*Pad*/3 } // PC11
                , SercomMux{ id<Port<2>::Pin<19>>(), Peripheral::C, /*Sercom*/6, /*Pad*/3 } // PC19
                , SercomMux{ id<Port<1>::Pin<21>>(), Peripheral::D, /*Sercom*/7, /*Pad*/0 } // PB21
                , SercomMux{ id<Port<3>::Pin< 8>>(), Peripheral::C, /*Sercom*/7, /*Pad*/0 } // PD08
                , SercomMux{ id<Port<1>::Pin<30>>(), Peripheral::C, /*Sercom*/7, /*Pad*/0 } // PB30
                , SercomMux{ id<Port<2>::Pin<12>>(), Peripheral::C, /*Sercom*/7, /*Pad*/0 } // PC12
                , SercomMux{ id<Port<1>::Pin<20>>(), Peripheral::D, /*Sercom*/7, /*Pad*/1 } // PB20
                , SercomMux{ id<Port<3>::Pin< 9>>(), Peripheral::C, /*Sercom*/7, /*Pad*/1 } // PD09
                , SercomMux{ id<Port<1>::Pin<31>>(), Peripheral::C, /*Sercom*/7, /*Pad*/1 } // PB31
                , SercomMux{ id<Port<2>::Pin<13>>(), Peripheral::C, /*Sercom*/7, /*Pad*/1 } // PC13
                , SercomMux{ id<Port<1>::Pin<18>>(), Peripheral::D, /*Sercom*/7, /*Pad*/2 } // PB18
                , SercomMux{ id<Port<2>::Pin<10>>(), Peripheral::D, /*Sercom*/7, /*Pad*/2 } // PC10
                , SercomMux{ id<Port<2>::Pin<14>>(), Peripheral::C, /*Sercom*/7, /*Pad*/2 } // PC14
                , SercomMux{ id<Port<3>::Pin<10>>(), Peripheral::C, /*Sercom*/7, /*Pad*/2 } // PD10
                , SercomMux{ id<Port<0>::Pin<30>>(), Peripheral::C, /*Sercom*/7, /*Pad*/2 } // PA30
                , SercomMux{ id<Port<1>::Pin<19>>(), Peripheral::D, /*Sercom*/7, /*Pad*/3 } // PB19
                , SercomMux{ id<Port<2>::Pin<11>>(), Peripheral::D, /*Sercom*/7, /*Pad*/3 } // PC11
                , SercomMux{ id<Port<2>::Pin<15>>(), Peripheral::C, /*Sercom*/7, /*Pad*/3 } // PC15
                , SercomMux{ id<Port<3>::Pin<11>>(), Peripheral::C, /*Sercom*/7, /*Pad*/3 } // PD11
                , SercomMux{ id<Port<0>::Pin<31>>(), Peripheral::C, /*Sercom*/7, /*Pad*/3 } // PA31
            };
        }

        template<typename Pin_t>
        constexpr std::optional<Peripheral> sercomPinPeripheral(const uint8_t sercomIndex, const Pin_t& pin = {})
        {
            for (const auto mux : sercomMux())
            {
                if (mux.sercomIndex == sercomIndex && mux.pinId == id(pin))
                    return mux.peripheral;
            }

            return std::nullopt;
        }
        ///@{ Static tests of common known values
        static_assert(*sercomPinPeripheral<PA06>(0) == Peripheral::D);
        static_assert(*sercomPinPeripheral<PA04>(0) == Peripheral::D);
        static_assert(*sercomPinPeripheral<PA05>(0) == Peripheral::D);
        static_assert(*sercomPinPeripheral<PB22>(1) == Peripheral::C);
        static_assert(*sercomPinPeripheral<PB23>(1) == Peripheral::C);
        static_assert(*sercomPinPeripheral<PA17>(1) == Peripheral::C);
        static_assert(*sercomPinPeripheral<PB08>(4) == Peripheral::D);
        static_assert(*sercomPinPeripheral<PB11>(4) == Peripheral::D);
        static_assert(*sercomPinPeripheral<PB09>(4) == Peripheral::D);
        ///@} Static tests of common known values

        template<typename Pin_t>
        constexpr std::optional<uint8_t> sercomPinPad(const uint8_t sercomIndex, const Pin_t& pin = {})
        {
            for (const auto mux : sercomMux())
            {
                if (mux.sercomIndex == sercomIndex && mux.pinId == id(pin))
                    return mux.sercomPad;
            }

            return std::nullopt;
        }
        ///@{ Static tests - Common known combinations
        static_assert(*sercomPinPad(0, PA04()) == 0);
        static_assert(*sercomPinPad(0, PA06()) == 2);
        static_assert(*sercomPinPad(0, PA05()) == 1);
        static_assert(*sercomPinPad(1, PB23()) == 3);
        static_assert(*sercomPinPad(1, PB22()) == 2);
        static_assert(*sercomPinPad(1, PA17()) == 1);
        static_assert(*sercomPinPad(4, PB11()) == 3);
        static_assert(*sercomPinPad(4, PB08()) == 0);
        static_assert(*sercomPinPad(4, PB09()) == 1);
        ///@} Static tests


    } // END ATsamd5x

    template<typename Pin_t>
    void gpioEnableInput( const Pin_t& pin )
    {
        using ::Port;
        PORT->Group[pin.id().port].PINCFG[pin.pinport].bit.INEN = true;
    }

    template<typename Pin_t>
    void gpioDirectionIn( const Pin_t& pin )
    {
        using ::Port;
        PORT->Group[pin.id().port].DIRCLR.reg = (1 << pin.id().pin); // clear DIR
    }

    template<typename Pin_t>
    void gpioDirectionOut( const Pin_t& pin )
    {
        using ::Port;
        PORT->Group[pin.id().port].DIRSET.reg = (1 << pin.id().pin); // Set DIR
    }

    template<typename Pin_t>
    void gpioPullEnable( const Pin_t& pin )
    {
        using ::Port;
        PORT->Group[pin.id().port].PINCFG[pin.id().pin].bit.PULLEN = true;
    }

    template<typename Pin_t>
    void gpioPullDisable( const Pin_t& pin )
    {
        using ::Port;
        PORT->Group[pin.id().port].PINCFG[pin.id().pin].bit.PULLEN = false;
    }

    template<typename Pin_t>
    void gpioOutHigh( const Pin_t& pin )
    {
        using ::Port;
        PORT->Group[pin.id().port].OUTSET.reg = pin.mask(); // Drive high
    }

    template<typename Pin_t>
    void gpioOutLow( const Pin_t& pin )
    {
        using ::Port;
        PORT->Group[pin.id().port].OUTCLR.reg = pin.mask(); // Drive low
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

    //TODO: DUplication..refartor
    template<typename Pin_t >
    void gpioFunction(const Pin_t& pin, const std::optional<ATsamd5x::Peripheral> peripheral )
    {
        using ::Port; //<Disambiguate Sam.h vs BareCpper::Gpio

        PORT->Group[pin.id().port].PINCFG[pin.id().pin].bit.PMUXEN = peripheral.has_value(); //<0xF is maximum valid function
        if (peripheral.has_value())
        {
            if (pin.id().pin & 0x1) // Odd numbered pin 
            {
                PORT->Group[pin.id().port].PMUX[pin.id().pin / 2].bit.PMUXO = static_cast<uint8_t>(*peripheral);
            }
            else // Even numbered pin
            {
                PORT->Group[pin.id().port].PMUX[pin.id().pin / 2].bit.PMUXE = static_cast<uint8_t>(*peripheral);
            }
        }
    }

} //END: BareCopper

#endif