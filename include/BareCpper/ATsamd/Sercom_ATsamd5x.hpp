#ifndef BARECPPER_SERCOM_ATSAM5X_H_
#define BARECPPER_SERCOM_ATSAM5X_H_

#include <cassert>

#include "Common_ATsamd5x.hpp"
#include "sercom.h" //< @note From Samd51 CMSIS

namespace BareCpper {
namespace ATsamd5x {
    inline ::Sercom* sercom(const uint8_t sercomIndex)
    {
        using ::Sercom;
        std::array devices = SERCOM_INSTS;
        return devices[sercomIndex];
    }

    inline void sercomApbEnable(const uint8_t sercomIndex)
    {
        switch (sercomIndex)
        {
        case 0: MCLK->APBAMASK.reg |= MCLK_APBAMASK_SERCOM0; break; //< APBA
        case 1: MCLK->APBAMASK.reg |= MCLK_APBAMASK_SERCOM1; break; //< APBA
        case 2: MCLK->APBBMASK.reg |= MCLK_APBBMASK_SERCOM2; break; //< APBB
        case 3: MCLK->APBBMASK.reg |= MCLK_APBBMASK_SERCOM3; break; //< APBB
        case 4: MCLK->APBDMASK.reg |= MCLK_APBDMASK_SERCOM4; break; //< APBC
        case 5: MCLK->APBDMASK.reg |= MCLK_APBDMASK_SERCOM5; break; //< APBC
        }
    }

    struct SercomParameters
    {
        uint8_t gclkId_CORE; ///< SERCOMn_GCLK_ID_CORE
        uint8_t gclkId_SLOW; ///< SERCOM2_GCLK_ID_SLOW
    };

    constexpr SercomParameters sercomParameters(const uint8_t sercomIndex)
    {
        constexpr std::array gclkId_COREs = { SERCOM0_GCLK_ID_CORE, SERCOM1_GCLK_ID_CORE, SERCOM2_GCLK_ID_CORE, SERCOM3_GCLK_ID_CORE, SERCOM4_GCLK_ID_CORE, SERCOM5_GCLK_ID_CORE };
        constexpr std::array gclkId_SLOWs = { SERCOM0_GCLK_ID_SLOW, SERCOM1_GCLK_ID_SLOW, SERCOM2_GCLK_ID_SLOW, SERCOM3_GCLK_ID_SLOW, SERCOM4_GCLK_ID_SLOW, SERCOM5_GCLK_ID_SLOW };

        return { (uint8_t)gclkId_COREs[sercomIndex], (uint8_t)gclkId_SLOWs[sercomIndex] };
    }

    template<size_t SercomIndex>
    struct Sercom
    {
        template<size_t IoSetIndex> ///< @todo limit Valid port indices to supported range in GCC 'compatible' manner /*, typename = Valid<true>*/
        struct IoSet
        {
            struct PadPins;

            constexpr static uint8_t sercom = SercomIndex;
            constexpr static uint8_t ioSet = IoSetIndex;
        };
    };

    /// @{ "6.2.8.1 SERCOM IOSET Configurations"
    template<>template<>template<> struct Sercom<0>::IoSet<0>::PadPins : Pins<SAM_G(PA08), SAM_G(PA09), SAM_G(PA10), SAM_G(PA11)> {};//SAM_G
    template<>template<>template<> struct Sercom<0>::IoSet<1>::PadPins : Pins<SAM_N(PB24), SAM_N(PB25), SAM_N(PC24), SAM_N(PC25)> {};//SAM_N
    template<>template<>template<> struct Sercom<0>::IoSet<2>::PadPins : Pins<SAM_G(PA04), SAM_G(PA05), SAM_G(PA06), SAM_G(PA07)> {};//SAM_G
    template<>template<>template<> struct Sercom<0>::IoSet<3>::PadPins : Pins<SAM_N(PC17), SAM_N(PC16), SAM_N(PC18), SAM_N(PC19)> {};//SAM_N
    template<>template<>template<> struct Sercom<1>::IoSet<0>::PadPins : Pins<SAM_G(PA16), SAM_G(PA17), SAM_G(PA18), SAM_G(PA19)> {};//SAM_G
    template<>template<>template<> struct Sercom<1>::IoSet<1>::PadPins : Pins<SAM_P(PC22), SAM_P(PC23), SAM_P(PD20), SAM_P(PD21)> {};//SAM_P
    template<>template<>template<> struct Sercom<1>::IoSet<2>::PadPins : Pins<SAM_N(PC27), SAM_N(PC28), SAM_G(PB22), SAM_G(PB23)> {};//SAM_N
    template<>template<>template<> struct Sercom<1>::IoSet<3>::PadPins : Pins<SAM_G(PA00), SAM_G(PA01), SAM_G(PA30), SAM_G(PA31)> {};//SAM_G
    template<>template<>template<> struct Sercom<2>::IoSet<0>::PadPins : Pins<SAM_G(PA12), SAM_G(PA13), SAM_G(PA14), SAM_G(PA15)> {};//SAM_G
    template<>template<>template<> struct Sercom<2>::IoSet<1>::PadPins : Pins<SAM_P(PB26), SAM_P(PB27), SAM_P(PB28), SAM_P(PB29)> {};//SAM_P
    template<>template<>template<> struct Sercom<2>::IoSet<2>::PadPins : Pins<SAM_G(PA09), SAM_G(PA08), SAM_G(PA10), SAM_G(PA11)> {};//SAM_G
    template<>template<>template<> struct Sercom<2>::IoSet<3>::PadPins : Pins<SAM_N(PB25), SAM_N(PB24), SAM_N(PC24), SAM_N(PC25)> {};//SAM_N
    template<>template<>template<> struct Sercom<3>::IoSet<0>::PadPins : Pins<SAM_G(PA22), SAM_G(PA23), SAM_G(PA24), SAM_G(PA25)> {};//SAM_G
    template<>template<>template<> struct Sercom<3>::IoSet<1>::PadPins : Pins<SAM_N(PB20), SAM_N(PB21), SAM_G(PA20), SAM_G(PA21)> {};//SAM_N
    template<>template<>template<> struct Sercom<3>::IoSet<2>::PadPins : Pins<SAM_G(PA17), SAM_G(PA16), SAM_G(PA18), SAM_G(PA19)> {};//SAM_G
    template<>template<>template<> struct Sercom<3>::IoSet<3>::PadPins : Pins<SAM_P(PC23), SAM_P(PC22), SAM_P(PD20), SAM_P(PD21)> {};//SAM_P
    template<>template<>template<> struct Sercom<4>::IoSet<0>::PadPins : Pins<SAM_J(PB12), SAM_J(PB13), SAM_J(PB14), SAM_J(PB15)> {};//SAM_J
    template<>template<>template<> struct Sercom<4>::IoSet<1>::PadPins : Pins<SAM_G(PB08), SAM_G(PB09), SAM_G(PB10), SAM_G(PB11)> {};//SAM_G
    template<>template<>template<> struct Sercom<4>::IoSet<2>::PadPins : Pins<SAM_G(PA13), SAM_G(PA12), SAM_G(PA14), SAM_G(PA15)> {};//SAM_G
    template<>template<>template<> struct Sercom<4>::IoSet<3>::PadPins : Pins<SAM_P(PB27), SAM_P(PB26), SAM_P(PB28), SAM_P(PB29)> {};//SAM_P
    template<>template<>template<> struct Sercom<5>::IoSet<0>::PadPins : Pins<SAM_J(PB16), SAM_J(PB17), SAM_N(PB18), SAM_N(PB19)> {};//SAM_N
    template<>template<>template<> struct Sercom<5>::IoSet<1>::PadPins : Pins<SAM_G(PA23), SAM_G(PA22), SAM_G(PA20), SAM_G(PA21)> {};//SAM_G
    template<>template<>template<> struct Sercom<5>::IoSet<2>::PadPins : Pins<SAM_G(PA23), SAM_G(PA22), SAM_G(PA24), SAM_G(PA25)> {};//SAM_G
    template<>template<>template<> struct Sercom<5>::IoSet<3>::PadPins : Pins<SAM_G(PA23), SAM_G(PA22), SAM_G(PB22), SAM_G(PB23)> {};//SAM_G
    template<>template<>template<> struct Sercom<5>::IoSet<4>::PadPins : Pins<SAM_J(PB31), SAM_J(PB30), SAM_J(PB00), SAM_J(PB01)> {};//SAM_J
    template<>template<>template<> struct Sercom<5>::IoSet<5>::PadPins : Pins<SAM_G(PB02), SAM_G(PB03), SAM_J(PB00), SAM_J(PB01)> {};//SAM_J

    template<>template<>template<> struct Sercom<6>::IoSet<0>::PadPins : Pins<SAM_N(PC16), SAM_N(PC17), SAM_N(PC18), SAM_N(PC19)> {}; //SAM_N
    template<>template<>template<> struct Sercom<6>::IoSet<1>::PadPins : Pins<SAM_P(PC04), SAM_N(PC05), SAM_N(PC06), SAM_N(PC07)> {}; //SAM_P
    template<>template<>template<> struct Sercom<6>::IoSet<2>::PadPins : Pins<SAM_P(PD09), SAM_P(PD08), SAM_P(PD10), SAM_P(PD11)> {}; //SAM_P
    template<>template<>template<> struct Sercom<6>::IoSet<3>::PadPins : Pins<SAM_N(PC13), SAM_N(PC12), SAM_N(PC14), SAM_N(PC15)> {}; //SAM_N
    template<>template<>template<> struct Sercom<6>::IoSet<5>::PadPins : Pins<SAM_N(PC13), SAM_N(PC12), SAM_N(PC10), SAM_N(PC11)> {}; //SAM_N
    /// @} "6.2.8.1 SERCOM IOSET Configurations"


} //END: ATsamd5x
} //END: BareCopper

#endif