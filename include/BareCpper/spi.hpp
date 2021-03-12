#ifndef BARECPPER_SPI_HPP_
#define BARECPPER_SPI_HPP_

#include "Io.hpp"
#include "Gpio.hpp"

namespace BareCpper
{
    /** Defines the bit-order for serial communication
        \remarks SpiBitOrder_MSBFirst MSB appears to be more common but check device documentation etc.
    */
    enum class SpiBitOrder : uint8_t
    {
          LSBFirst = 0 ///< Bits are output from right to left (binary 1 '00000001' will output bit value 1 first)
        , MSBFirst = 1 ///< Bits are output from left to right (binary 1 '00000001' will output bit value 0 first)
    };

    /* Determines the polarity of the clock (CPOL)
    */
    enum class SpiClockPolarity : uint8_t
    {
        Cpol0 = 0, ///< idle low, active high
        Cpol1 = 1, ///< idle high, active low		
    };

    /* Determines the timing (i.e. phase) of the data bits relative to the clock pulses (CPHA)
    */
    enum class SpiClockPhase : uint8_t
    {
        Cpha0 = 0, ///< http://www.atmel.com/Images/doc2585.pdf Chapter 2.4
        Cpha1 = 1, ///<  http://www.atmel.com/Images/doc2585.pdf Chapter 2.4
    };

    /** Spi clocking mode which is a combination of Polarity, Phase, and in some platforms Clock-edge
        @remarks https://en.wikipedia.org/wiki/Serial_Peripheral_Interface#Mode_numbers
        @remarks http://www.atmel.com/Images/doc2585.pdf Chapter 2.4
    */
    enum class SpiMode : uint8_t
    {
          Mode0 ///< SpiClockPolarity::Cpol0 | SpiClockPhase::Cpha0 : Shift SCK: Falling, Capture SCK: Rising
        , Mode1 ///< SpiClockPolarity::Cpol0 | SpiClockPhase::Cpha1 : Shift SCK: Rising, Capture SCK: Falling
        , Mode2 ///< SpiClockPolarity::Cpol1 | SpiClockPhase::Cpha0 : Shift SCK: Rising, Capture SCK: Falling
        , Mode3 ///< SpiClockPolarity::Cpol1 | SpiClockPhase::Cpha1 : Shift SCK: Falling, Capture SCK: Rising
    };

    /** Get the polarity of the SPI clock for the specified mode
    */
    constexpr SpiClockPolarity spiClockPolarity(const SpiMode mode);

    /** Get the Phase of the SPI clock for the specified mode
    */
    constexpr SpiClockPhase spiClockPhase(const SpiMode mode);

    /** Get the Mode of the SPI clock for the specified Polarity+Phase pair
    */
    constexpr SpiMode spiMode(const SpiClockPolarity cpol, const SpiClockPhase cpha);

    /** SPI pin-selection
    */
    template< 
          typename MosiPin_t //, Config_t::Pin_MOSI/DN pin
        , typename MisoPin_t //< MISO output pin
        , typename SerialClockPin_t  //< Clock pin
    >
    struct SpiPins
    {
        static constexpr MosiPin_t mosi = {};
        static constexpr MisoPin_t miso = {};
        static constexpr SerialClockPin_t sck = {};
    };

    constexpr size_t SpiDefaultBaudRate = 6000000; //< 6MHz

    /** Specify a Compile-time configuration for SPI
    * @see SpiRuntimeConfig to set a flexible mode at runtime i.e. multiple slave devices
    * @note For Soft-Device implementations this can create more optimal (smaller+faster) code than using a runtime configuration
    */
    template<
        typename ChipSelectPin_t //< Device select pin(s)
        , SpiMode Mode
        , SpiBitOrder BitOrder
        , size_t BaudRate = SpiDefaultBaudRate  >
    struct SpiConfig
    {
        static constexpr ChipSelectPin_t chipSelect = {};
        static constexpr size_t baudRate = BaudRate;
        static constexpr SpiMode mode = Mode;
        static constexpr SpiBitOrder bitOrder = BitOrder;
    };

    /** Specify a Runtime-time configuration for SPI
    */
    struct SpiRuntimeConfig
    {
        int/** TODO: Runtime pin addressing */ chipSelect;
        const size_t baudRate = SpiDefaultBaudRate;
        const SpiMode mode;
        const SpiBitOrder bitOrder;
    };

    struct SpiMessage
    {
        const uint8_t* txBuffer; ///<  Pointer to the output data buffer
        uint8_t* rxBuffer; ///< Pointer to the input data buffer 
        int16_t bufferLength; ///< Size of the message data in SPI characters
    };

    /** CRTP Platform specific base
    * @note May specialise based on Config_t is SpiRuntimeConfig or SpiConfig etc
    * @todo Could specialise for Async, Soft, HW device options alsos
    */
    template< typename Pins_t, typename PlatformConfig_t >
    class SpiImpl;

    template< typename Pins_t, typename PlatformConfig_t = std::nullopt_t >
    class Spi
    {
    public:
        using spi_t = Spi< Pins_t, PlatformConfig_t>;
        using impl_t = SpiImpl<Pins_t, PlatformConfig_t>;
        using pins_t = Pins_t;
        using platformConfig_t = PlatformConfig_t; ///< Optional per-platform configuration i.e. Multiple device selection and extended configuration etc

        template< typename Config_t >
        class Instance : public IoDescriptor
        {
        public:
            Instance(spi_t* device);

            bool initialise();

            /** Setup SPI peripheral including setting ChipSelect Pin IO states
             * @param config  Device specific config
             * @return True on success, false on failure
            */
            bool configure(const Config_t& config = {});

            /** Start SPI clock and select ChipSelect Pin
            */
            bool enable();

            /** Stop SPI clock and deselect ChipSelect Pin
            */
            bool disable();

            int32_t read(uint8_t* const buffer, const uint16_t bufferLength, const bool doStop = true);

            int32_t write(const uint8_t* const buffer, const uint16_t bufferLength, const bool doStop = true);

            int32_t transfer(const SpiMessage& message);

        protected:
            spi_t* device_;
            Config_t config_;
        };


    public:
        Spi();

        bool initialise(
              const Pins_t& pins = {}
            , const PlatformConfig_t& platformConfig = {} );

        template< typename Config_t >
        Instance<Config_t> makeInstance()
        {
            return Instance<Config_t>(this);
        }

    private:

        bool initialiseGpio(const Pins_t& pins, const PlatformConfig_t& platformConfig);

    protected:
        impl_t impl_ = {};
    };

} //END: BareCpper

#include "internal/Spi-inl.hpp"

#if !DOXYGEN ///< Platform specific interface definitions
#if __SAMD51__
#include "ATsamd/Spi_Atsamd5x.hpp"
#endif
#endif

#endif