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

    struct SercomClocks
    {
        uint8_t gclkId_CORE; ///< SERCOMn_GCLK_ID_CORE
        uint8_t gclkId_SLOW; ///< SERCOM2_GCLK_ID_SLOW
    };

    constexpr SercomClocks sercomClocks(const uint8_t sercomIndex)
    {
        constexpr std::array gclkId_COREs = { SERCOM0_GCLK_ID_CORE, SERCOM1_GCLK_ID_CORE, SERCOM2_GCLK_ID_CORE, SERCOM3_GCLK_ID_CORE, SERCOM4_GCLK_ID_CORE, SERCOM5_GCLK_ID_CORE };
        constexpr std::array gclkId_SLOWs = { SERCOM0_GCLK_ID_SLOW, SERCOM1_GCLK_ID_SLOW, SERCOM2_GCLK_ID_SLOW, SERCOM3_GCLK_ID_SLOW, SERCOM4_GCLK_ID_SLOW, SERCOM5_GCLK_ID_SLOW };

        return { (uint8_t)gclkId_COREs[sercomIndex], (uint8_t)gclkId_SLOWs[sercomIndex] };
    }

    /** Finds the first SercomIndex that is common to all provided Pins (Should only ever be one match!)
    * @note Approach is to find the First SercomIndex candidate and then find subsequenct 2 matches
    * @warning This approach does not succeed if sercomMux() has dusplication which must eb avoided! */
    template< size_t pinCount>
    constexpr std::optional<uint8_t> sercomForPins(const PinId(&ids)[pinCount] )
    {
        constexpr auto muxes = sercomMux();
        for ( auto iMux = std::begin(muxes); iMux != std::end(muxes); ++iMux )
        {
            size_t i = 0;
            for (; i < pinCount && iMux->pinId != ids[i]; ++i) {/*nop*/ }
            if (i == pinCount) continue;//< No match

            uint8_t matchCount = 1;
            for ( auto iMuxB = iMux+1; iMuxB != std::end(muxes); ++iMuxB)
            {
                if (iMuxB->sercomIndex == iMux->sercomIndex)
                {
                    size_t j = 0;
                    for (j = 0; i < pinCount && iMux->pinId != ids[j]; ++i) { /*nop*/}
                   // assert(j != i); //< Does not suppoert duplicates in sercomMu()!!
                    if ((j < pinCount) && (++matchCount == pinCount))
                    {
                        return iMux->sercomIndex;
                    }
                }
            }
        }
        return std::nullopt; //< Not a valid/known combination so far...
    }

    ///@{ Static tests - Common known combinations
    static_assert(*ATsamd5x::sercomForPins({ id<PA04>(), id<PA06>(),  id<PA05>() } ) == 0);
    static_assert(*ATsamd5x::sercomForPins({ id<PB23>(), id<PB22>(),  id<PA17>() } ) == 1);
    static_assert(*ATsamd5x::sercomForPins({ id<PB11>(), id<PB08>(),  id<PB09>() } ) == 4);
    ///@} Static tests

#if 0// @NOTE WIP - Superceeded by sercomForPins() ???
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