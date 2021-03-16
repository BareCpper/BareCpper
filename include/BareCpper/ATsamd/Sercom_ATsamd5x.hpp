#ifndef BARECPPER_SERCOM_ATSAM5X_H_
#define BARECPPER_SERCOM_ATSAM5X_H_

#include <cassert>
#include <array>
#include <optional>

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

    struct SercomPads
    {
        uint8_t mosi;
        uint8_t miso;
        uint8_t sck;
    };

    /** Pins <Mosi, Miso, Sck> */
    template < typename MosiPin_t, typename MisoPin_t, typename SerialClockPin_t >
    constexpr std::optional<uint8_t> sercomForPins(const MosiPin_t mosi, const MisoPin_t miso, const SerialClockPin_t sck)
    {
        if (mosi == PA04() && miso == PA06() && sck == PA05()) return 0;
        if (mosi == PB23() && miso == PB22() && sck == PA17()) return 1;
        if (mosi == PB11() && miso == PB08() && sck == PB09()) return 4;
        return std::nullopt; //< Not a valid/known combination so far...
    }
    ///@{ Static tests - Common known combinations
    static_assert(ATsamd5x::sercomForPins(PA04(), PA06(), PA05()) == 0);
    static_assert(ATsamd5x::sercomForPins(PB23(), PB22(), PA17()) == 1);
    static_assert(ATsamd5x::sercomForPins(PB11(), PB08(), PB09()) == 4);
    ///@} Static tests

    /** Pins <Mosi, Miso, Sck> */
    template < typename MosiPin_t, typename MisoPin_t, typename SerialClockPin_t >
    constexpr std::optional<SercomPads> sercomPadsForPins(const MosiPin_t mosi, const MisoPin_t miso, const SerialClockPin_t sck )
    {
        if (mosi == PA04() && miso == PA06() && sck == PA05() ) return SercomPads{ 0, 2, 1 };
        if (mosi == PB23() && miso == PB22() && sck == PA17() ) return SercomPads{ 3, 2, 1 };
        if (mosi == PB11() && miso == PB08() && sck == PB09() ) return SercomPads{ 3, 0, 1 };
        return std::nullopt; //< Not a valid/known combination so far...
    }
    ///@{ Static tests - Common known combinations
    static_assert(ATsamd5x::sercomPadsForPins(PA04(), PA06(), PA05())->mosi == 0 );
    static_assert(ATsamd5x::sercomPadsForPins(PA04(), PA06(), PA05())->miso == 2);
    static_assert(ATsamd5x::sercomPadsForPins(PA04(), PA06(), PA05())->sck == 1);
    static_assert(ATsamd5x::sercomPadsForPins(PB23(), PB22(), PA17())->mosi == 3);
    static_assert(ATsamd5x::sercomPadsForPins(PB23(), PB22(), PA17())->miso == 2);
    static_assert(ATsamd5x::sercomPadsForPins(PB23(), PB22(), PA17())->sck == 1);
    static_assert(ATsamd5x::sercomPadsForPins(PB11(), PB08(), PB09())->mosi == 3);
    static_assert(ATsamd5x::sercomPadsForPins(PB11(), PB08(), PB09())->miso == 0);
    static_assert(ATsamd5x::sercomPadsForPins(PB11(), PB08(), PB09())->sck == 1);
    ///@} Static tests


#if 0// @NOTE WIP

    template<size_t SercomIndex>
    struct Sercom
    {

        struct IoSets; //< @ std::tuple< Pins<...>, Pins<...> >

        static constexpr size_t ioSetPinMatch(const std::array<PinId, 4>& ioSetPins, const PinId& a, const PinId& b, const PinId& c)
        {
            return ioSetPins
        }

        static constexpr uint8_t ioSetForPins(const PinId& a, const PinId& b, const PinId& c)
        {
            { ((std::cout << args << '\n'), ...); }

            std::apply([](auto&&... ioSets)
                { ((std::cout << args << '\n'), ...); }
            , IoSets());
        }

        static constexpr uint8_t padForPin(const PinId& pinId)
        {
            std::apply([](auto&&... ioSets)
                { ((std::cout << args << '\n'), ...); }
            , IoSets());
        }
};


    /** @see "6.2.8.1 SERCOM IOSET Configurations"
    */
    constexpr uint8_t sercomPadForPin(const uint8_t sercomIndex, const PinId& pinId)
    {
#if 0 // @todo Expansion from make_index_sequence instead of Switch case?
        constexpr uint8_t sercomCount = SERCOM_INST_NUM; //< TODO: put somewhere useful!
        std::apply([]() {
            (())
            }, std::make_index_sequence<sercomCount>());
#else
        switch (sercomIndex)
        {
        case 0: return Sercom<0>::padForPin(pinId);
        case 1: return Sercom<1>::padForPin(pinId);
        case 2: return Sercom<2>::padForPin(pinId);
        case 3: return Sercom<3>::padForPin(pinId);
        case 4: return Sercom<4>::padForPin(pinId);
        case 5: return Sercom<5>::padForPin(pinId);
        case 6: return Sercom<6>::padForPin(pinId);
        case 7: return Sercom<7>::padForPin(pinId);
        default:
            assert(false);
            return 0;
        }
#endif
    }

    /// @{ "6.2.8.1 SERCOM IOSET Configurations"
    template<size_t SercomIndex>
    struct Sercom<SercomIndex>::IoSets : std::tuple<>{};

    template<> struct Sercom<0>::IoSets : std::tuple< Pins<SAM_G(PA08), SAM_G(PA09), SAM_G(PA10), SAM_G(PA11)>       //SAM_G
                                                    , Pins<SAM_N(PB24), SAM_N(PB25), SAM_N(PC24), SAM_N(PC25)>       //SAM_N
                                                    , Pins<SAM_G(PA04), SAM_G(PA05), SAM_G(PA06), SAM_G(PA07)>       //SAM_G
                                                    , Pins<SAM_N(PC17), SAM_N(PC16), SAM_N(PC18), SAM_N(PC19)> > {}; //SAM_N
                         
    template<> struct Sercom<1>::IoSets : std::tuple< Pins<SAM_G(PA16), SAM_G(PA17), SAM_G(PA18), SAM_G(PA19)>       //SAM_G
                                                    , Pins<SAM_P(PC22), SAM_P(PC23), SAM_P(PD20), SAM_P(PD21)>       //SAM_P
                                                    , Pins<SAM_N(PC27), SAM_N(PC28), SAM_G(PB22), SAM_G(PB23)>       //SAM_N
                                                    , Pins<SAM_G(PA00), SAM_G(PA01), SAM_G(PA30), SAM_G(PA31)> > {}; //SAM_G
                         
    template<> struct Sercom<2>::IoSets : std::tuple< Pins<SAM_G(PA12), SAM_G(PA13), SAM_G(PA14), SAM_G(PA15)>       //SAM_G
                                                    , Pins<SAM_P(PB26), SAM_P(PB27), SAM_P(PB28), SAM_P(PB29)>       //SAM_P
                                                    , Pins<SAM_G(PA09), SAM_G(PA08), SAM_G(PA10), SAM_G(PA11)>       //SAM_G
                                                    , Pins<SAM_N(PB25), SAM_N(PB24), SAM_N(PC24), SAM_N(PC25)> > {}; //SAM_N
                         
    template<> struct Sercom<3>::IoSets : std::tuple< Pins<SAM_G(PA22), SAM_G(PA23), SAM_G(PA24), SAM_G(PA25)>       //SAM_G
                                                    , Pins<SAM_N(PB20), SAM_N(PB21), SAM_G(PA20), SAM_G(PA21)>       //SAM_N
                                                    , Pins<SAM_G(PA17), SAM_G(PA16), SAM_G(PA18), SAM_G(PA19)>       //SAM_G
                                                    , Pins<SAM_P(PC23), SAM_P(PC22), SAM_P(PD20), SAM_P(PD21)> > {}; //SAM_P
                         
    template<> struct Sercom<4>::IoSets : std::tuple< Pins<SAM_J(PB12), SAM_J(PB13), SAM_J(PB14), SAM_J(PB15)>       //SAM_J
                                                    , Pins<SAM_G(PB08), SAM_G(PB09), SAM_G(PB10), SAM_G(PB11)>       //SAM_G
                                                    , Pins<SAM_G(PA13), SAM_G(PA12), SAM_G(PA14), SAM_G(PA15)>       //SAM_G
                                                    , Pins<SAM_P(PB27), SAM_P(PB26), SAM_P(PB28), SAM_P(PB29)> > {}; //SAM_P
                         
    template<> struct Sercom<5>::IoSets : std::tuple< Pins<SAM_J(PB16), SAM_J(PB17), SAM_N(PB18), SAM_N(PB19)>       //SAM_N
                                                    , Pins<SAM_G(PA23), SAM_G(PA22), SAM_G(PA20), SAM_G(PA21)>       //SAM_G
                                                    , Pins<SAM_G(PA23), SAM_G(PA22), SAM_G(PA24), SAM_G(PA25)>       //SAM_G
                                                    , Pins<SAM_G(PA23), SAM_G(PA22), SAM_G(PB22), SAM_G(PB23)>       //SAM_G
                                                    , Pins<SAM_J(PB31), SAM_J(PB30), SAM_J(PB00), SAM_J(PB01)>       //SAM_J
                                                    , Pins<SAM_G(PB02), SAM_G(PB03), SAM_J(PB00), SAM_J(PB01)> > {}; //SAM_J
                         
    template<> struct Sercom<6>::IoSets : std::tuple< Pins<SAM_N(PC16), SAM_N(PC17), SAM_N(PC18), SAM_N(PC19)>       //SAM_N
                                                    , Pins<SAM_P(PC04), SAM_N(PC05), SAM_N(PC06), SAM_N(PC07)>       //SAM_P
                                                    , Pins<SAM_P(PD09), SAM_P(PD08), SAM_P(PD10), SAM_P(PD11)>       //SAM_P
                                                    , Pins<SAM_N(PC13), SAM_N(PC12), SAM_N(PC14), SAM_N(PC15)>       //SAM_N
                                                    , Pins<SAM_N(PC13), SAM_N(PC12), SAM_N(PC10), SAM_N(PC11)> > {}; //SAM_N
                         
    template<> struct Sercom<7>::IoSets : std::tuple< Pins<SAM_N(PC12), SAM_N(PC13), SAM_N(PC14), SAM_N(PC15)>       //SAM_N
                                                    , Pins<SAM_P(PD08), SAM_P(PD09), SAM_P(PD10), SAM_P(PD11)>       //SAM_P
                                                    , Pins<SAM_N(PC12), SAM_N(PC13), SAM_N(PC10), SAM_N(PC11)>       //SAM_N
                                                    , Pins<SAM_N(PB21), SAM_N(PB20), SAM_N(PB18), SAM_N(PB19)>       //SAM_N
                                                    , Pins<SAM_N(PB30), SAM_N(PB31), SAM_N(PA30), SAM_N(PA31)> > {}; //SAM_N @TODO: SAM_J only has 6 Sercom instances so IOset-4 cannot be used on that device albeit the pins are valid
    /// @} "6.2.8.1 SERCOM IOSET Configurations"

#endif

} //END: ATsamd5x
} //END: BareCopper

#endif