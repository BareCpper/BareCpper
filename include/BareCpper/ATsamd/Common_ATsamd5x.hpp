
#ifndef BARECPPER_COMMON_HPP_
#  error "Include <BareCpper/Common.hpp> instead of this file."
#endif

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include "sam.h"
#include "sercom.h" //< @note From Samd51 CMSIS

namespace BareCpper {

    namespace ATsamd5x
    {
        /* @param[in] function  The pin function is given by a 32 - bit wide bitfield found in the header files for the device
                                e.g. PINMUX_PA12C_SERCOM2_PAD0
        */
        static void setPinFunction(const uint8_t iPort, const uint8_t iPin, const uint32_t function)
        {
            using ::Port; //< Disambiguate Sam.h vs BareCpper::Gpio

            PORT->Group[iPort].PINCFG[iPin].bit.PMUXEN = (function <= PORT_PMUX_PMUXE_Msk); //< 0xF is maximum valid function

            if (iPin & 0x1) // Odd numbered pin 
            {
                PORT->Group[iPort].PMUX[iPin / 2].bit.PMUXO = function;
            }
            else // Even numbered pin
            {
                PORT->Group[iPort].PMUX[iPin / 2].bit.PMUXE = function;
            }
        }

        constexpr ::Sercom* sercom(const uint8_t sercomIndex)
        {
            using ::Sercom;
            std::array devices = SERCOM_INSTS;
            return devices[sercomIndex];
        }
        constexpr void sercomApbEnable(const uint8_t sercomIndex)
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

    }
} //END: BareCpper