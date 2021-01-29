#ifndef BARECPPER_SPI_HPP_
#  error "Include <BareCpper/Spi.hpp> instead of this file."
#endif

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <cstdint> 
#include <array> //< std::array

#include "../Common.hpp" //< 'temp?

#include "sam.h"
#include "sercom.h" //< @note From Samd51 CMSIS

#define CONF_SPIRXEN 0x1
#define CONF_SPICHSIZE 0x0
#define CONF_SPIDUMMYBYTE 0x1ff //< TBC!!
#define CONF_SPIIBON 0x0 ///< Immediate Buffer Overflow Notification
#define CONF_SPIRUNSTDBY 0x0
/// @todo Slave SPI only: #define CONF_SPIAMODE_EN 0 //< AddressMode
/// @todo Slave SPI only: #define CONF_SPIAMODE 0
/// @todo Slave SPI only: #define CONF_SPIADDR 0
/// @todo Slave SPI only: #define CONF_SPIADDRMASK 0
#define CONF_SPISSDE 0 ///< Slave Select Low Detect Enable
#define CONF_SPIMSSEN 0x0 ///< Master Slave Select Enable
#define CONF_SPIPLOADEN 0 ///< Data Preload Enable
#define CONF_SPIRXPO 2 ///< Data In Pinout
#define CONF_SPITXPO 3 ///< Data Out Pinout

#ifndef SERCOM_SPI_CTRLA_MODE_SPI_MASTER
#define SERCOM_SPI_CTRLA_MODE_SPI_MASTER SERCOM_SPI_CTRLA_MODE(0x03) //< @todo Is the CTRLA SPI MASTER  defined somewhere?
#endif
namespace BareCpper
{
    template< uint8_t SercomIndex, uint8_t GclkId_CORE_Src, size_t GclkId_CORE_Freq, uint8_t GclkId_SLOW_Src>
    struct SpiPlatformConfig
    {
        static constexpr uint8_t sercomIndex = SercomIndex;

        static constexpr uint8_t gclkId_CORE_Src = GclkId_CORE_Src;// CONF_GCLK_SERCOM2_CORE_SRC;
        static constexpr size_t gclkId_CORE_Freq = GclkId_CORE_Freq;
        static constexpr uint8_t gclkId_SLOW_Src = GclkId_SLOW_Src;// CONF_GCLK_SERCOM2_SLOW_SRC;
    };

    template< typename Pins_t, typename Config_t, typename PlatformConfig_t = std::nullopt_t >
    class SpiImpl
    {
    public:
        using config_t = Config_t;
        using platformConfig_t = PlatformConfig_t;

        bool initialise(const config_t& config, const platformConfig_t& platformConfig)
        {
            return initialiseClock(platformConfig)
                && initialiseDevice(config, platformConfig)
                //&& initialiseAsync(config, platformConfig)
                //&& initialiseGpio(config, platformConfig)
                ;
        }

        bool destroy()
        {
            hw_->CTRLA.bit.ENABLE = false;
            hw_->SYNCBUSY.bit.SWRST = true;
            while (hw_->SYNCBUSY.bit.SWRST | hw_->SYNCBUSY.bit.ENABLE);
            return true;
        }

        bool enable()
        {
            return enableSync()
#if 0  ///@todo Async: 
                   && enableAsync()
#endif
                ;
        }

        bool disable()
        {
            return disableSync()
#if 0  ///@todo Async: 
                && disableAsync()
#endif
                ;
        }

        uint8_t byteTransfer(const uint8_t sendByte)
        {
            hw_->DATA.bit.DATA = sendByte; 

            while (hw_->INTFLAG.bit.RXC == 0); // Waiting Complete Reception

            const uint8_t readByte = hw_->DATA.bit.DATA;  // Reading data
            return readByte;
        }

        int32_t transfer(const SpiMessage& message)
        {
            const uint8_t* txBuffer = message.txBuffer;
            uint8_t* rxBuffer = message.rxBuffer;

            size_t iRxBuffer = 0;
            size_t iTxBuffer = 0;

            int32_t retVal = 0;

            SERCOM_SPI_INTFLAG_Type iFlag; //, Store volatile per iteration to save re-reading for each flag
            for (iFlag.reg = hw_->INTFLAG.reg; !iFlag.bit.ERROR && (iRxBuffer < message.bufferLength); iFlag.reg = hw_->INTFLAG.reg )
            {
                // Receive Complete Interrupt
                if (iFlag.bit.RXC )
                {
                    const uint8_t readByte = static_cast<uint8_t>(hw_->DATA.bit.DATA); // @note Clears hw_->INTFLAG.bit.RXC
                    if (rxBuffer)
                        rxBuffer[iRxBuffer] = readByte;
                    ++iRxBuffer;
                }

                //  Data Register Empty
                if (iFlag.bit.DRE 
                    && (iTxBuffer < message.bufferLength)
#if 0 /// @todo TBC: ASF _spi_m_sync_trans: In master mode, do not start next byte before previous byte received as to make better output waveform
                    && (iRxBuffer >= iTxBuffer)
#endif
                    )
                {
                    const uint8_t writeByte = txBuffer ? txBuffer[iTxBuffer] : uint8_t(CONF_SPIDUMMYBYTE);
                    //SERCOM_CRITICAL_SECTION_ENTER
                    hw_->DATA.bit.DATA = writeByte; //< @note Clears hw_->INTFLAG.bit.DRE
                    //SERCOM_CRITICAL_SECTION_LEAVE
                    ++iTxBuffer;
                }

            }

            /* Wait until SPI bus idle */
            while(!(hw_->INTFLAG.reg & (SERCOM_SPI_INTFLAG_TXC | SERCOM_SPI_INTFLAG_DRE)));
            hw_->INTFLAG.reg = (SERCOM_SPI_INTFLAG_TXC | SERCOM_SPI_INTFLAG_DRE); //< Clear flag

            if (iFlag.bit.ERROR)
            {
                //SERCOM_CRITICAL_SECTION_ENTER ???
                hw_->STATUS.reg = ~0;
                hw_->INTFLAG.reg = SERCOM_SPI_INTFLAG_ERROR;
                return -1;
            }
            else
            {
                return iTxBuffer;
            }
        }


     protected:

        bool enableSync()
        {
            // SERCOM_CRITICAL_SECTION_ENTER();
            hw_->CTRLA.bit.ENABLE = true;
            while (hw_->SYNCBUSY.bit.SWRST | hw_->SYNCBUSY.bit.ENABLE);
            // SERCOM_CRITICAL_SECTION_LEAVE();

            return true;
        }

        bool disableSync()
        {
            hw_->CTRLA.bit.ENABLE = false;
            while (hw_->SYNCBUSY.bit.SWRST | hw_->SYNCBUSY.bit.ENABLE);

            return true;
        }

        bool initialiseClock( const platformConfig_t& platformConfig)
        {
            /// @TODO Power-Saving: TBC if Slow-Clock even necessary

            constexpr ATsamd5x::SercomParameters params = ATsamd5x::sercomParameters(platformConfig.sercomIndex);

            GCLK->PCHCTRL[params.gclkId_CORE].bit.CHEN = 0; // Disable timer
            GCLK->PCHCTRL[params.gclkId_SLOW].bit.CHEN = 0; // Disable timer

            while (GCLK->PCHCTRL[params.gclkId_SLOW].bit.CHEN
                || GCLK->PCHCTRL[params.gclkId_CORE].bit.CHEN);  // Wait for disable

            // GCLK_CRITICAL_SECTION_ENTER();
            GCLK->PCHCTRL[params.gclkId_CORE].reg = platformConfig.gclkId_CORE_Src | GCLK_PCHCTRL_CHEN;
            GCLK->PCHCTRL[params.gclkId_SLOW].reg = platformConfig.gclkId_SLOW_Src | GCLK_PCHCTRL_CHEN;
            // GCLK_CRITICAL_SECTION_LEAVE();

             //MCLK_CRITICAL_SECTION_ENTER();
            ATsamd5x::sercomApbEnable(platformConfig.sercomIndex);
            //MCLK_CRITICAL_SECTION_LEAVE();

            while (!GCLK->PCHCTRL[params.gclkId_CORE].bit.CHEN
                && !GCLK->PCHCTRL[params.gclkId_SLOW].bit.CHEN ); // Wait for clock enable

            return true;
        }

        bool initialiseDevice(const config_t& config, const platformConfig_t& platformConfig)
        {
            ::Sercom* sercom = ATsamd5x::sercom(platformConfig.sercomIndex);
            if (!sercom)
                return false;
            hw_ = &sercom->SPI;

            if (!hw_->SYNCBUSY.bit.SWRST)
            {
                if (hw_->CTRLA.bit.ENABLE) // Disable
                {
                    hw_->CTRLA.bit.ENABLE = false;
                    while (hw_->SYNCBUSY.bit.ENABLE);
                }

                hw_->CTRLA.reg = SERCOM_SPI_CTRLA_SWRST | SERCOM_SPI_CTRLA_MODE_SPI_MASTER; // Master reset
            }

            while (hw_->SYNCBUSY.bit.SWRST); // Wait for reset

            using BaudBit_t = decltype(hw_->BAUD.reg);
            const BaudBit_t baudBit = static_cast<BaudBit_t>((static_cast<float>(platformConfig.gclkId_CORE_Freq) / static_cast<float>(2 * config.baudRate)) - 1);
                            
            //SERCOM_CRITICAL_SECTION_ENTER();
            const uint32_t ctrlA = SERCOM_SPI_CTRLA_MODE_SPI_MASTER
                | (config.bitOrder == SpiBitOrder::LSBFirst ? SERCOM_SPI_CTRLA_DORD : 0)  ///@todo Support selecting slave modes?
                | (spiClockPolarity(config.mode) == SpiClockPolarity::Cpol1 ? SERCOM_SPI_CTRLA_CPOL : 0)
                | (spiClockPhase(config.mode) == SpiClockPhase::Cpha1 ? SERCOM_SPI_CTRLA_CPHA : 0)
                /// @todo Slave SPI only:  | (CONF_SPIAMODE_EN ? SERCOM_SPI_CTRLA_FORM(2) : SERCOM_SPI_CTRLA_FORM(0))
                | SERCOM_SPI_CTRLA_DOPO(CONF_SPITXPO)
                | SERCOM_SPI_CTRLA_DIPO(CONF_SPIRXPO)
                | (CONF_SPIIBON ? SERCOM_SPI_CTRLA_IBON : 0)
                | (CONF_SPIRUNSTDBY ? SERCOM_SPI_CTRLA_RUNSTDBY : 0);
            const uint32_t ctrlB = ((CONF_SPIRXEN ? SERCOM_SPI_CTRLB_RXEN : 0)
                | (CONF_SPIMSSEN ? SERCOM_SPI_CTRLB_MSSEN : 0)
                | (CONF_SPISSDE ? SERCOM_SPI_CTRLB_SSDE : 0)
                | (CONF_SPIPLOADEN ? SERCOM_SPI_CTRLB_PLOADEN : 0)
                /// @todo Slave SPI only: | SERCOM_SPI_CTRLB_AMODE(CONF_SPIAMODE) 
                | SERCOM_SPI_CTRLB_CHSIZE(CONF_SPICHSIZE));

            hw_->CTRLA.reg = ctrlA;
            hw_->CTRLB.reg = ctrlB;

            /// @todo Slave SPI only:  |addr = (SERCOM_SPI_ADDR_ADDR(CONF_SPIADDR) | SERCOM_SPI_ADDR_ADDRMASK(CONF_SPIADDRMASK));
            hw_->DBGCTRL.bit.DBGSTOP = true;
            hw_->BAUD.bit.BAUD = baudBit;
            //SERCOM_CRITICAL_SECTION_LEAVE();

            return true;
        }

    private:
        SercomSpi* hw_ = nullptr;
    };


#if 0
    inline IoDescriptor SpiDma::io()
    {
        return {
            [](IoDescriptor& descriptor, uint8_t* const buffer, const uint16_t bufferLength) ->int32_t
            {
                return 0; //< Read result
            },
            [](IoDescriptor& descriptor, const uint8_t* const buffer, const uint16_t bufferLength) ->int32_t
            {
                return 0; //< Write result
            }
        };
    }
#endif

} //END: BareCpper