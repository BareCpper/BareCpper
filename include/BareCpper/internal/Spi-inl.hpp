#ifndef BARECPPER_SPI_INL_HPP_
#define BARECPPER_SPI_INL_HPP_

#include "../../BareCpper/gpio.hpp" //< gpioDirectionIn, gpioPullDisable @todo Ardunio can't path this properly!

namespace BareCpper
{
    constexpr SpiClockPolarity spiClockPolarity(const SpiMode mode)
    {
        switch (mode)
        {
        case SpiMode::Mode0: case SpiMode::Mode1: return SpiClockPolarity::Cpol0;
        case SpiMode::Mode2: case SpiMode::Mode3: return SpiClockPolarity::Cpol1;
        default: assert(false); return SpiClockPolarity::Cpol0;
        }
    }
    static_assert(spiClockPolarity(SpiMode::Mode0) == SpiClockPolarity::Cpol0);
    static_assert(spiClockPolarity(SpiMode::Mode1) == SpiClockPolarity::Cpol0);
    static_assert(spiClockPolarity(SpiMode::Mode2) == SpiClockPolarity::Cpol1);
    static_assert(spiClockPolarity(SpiMode::Mode3) == SpiClockPolarity::Cpol1);

    constexpr SpiClockPhase spiClockPhase(const SpiMode mode)
    {
        switch (mode)
        {
        case SpiMode::Mode0: case SpiMode::Mode2: return SpiClockPhase::Cpha0;
        case SpiMode::Mode1: case SpiMode::Mode3: return SpiClockPhase::Cpha1;
        default: assert(false); return SpiClockPhase::Cpha0;
        }
    }
    static_assert(spiClockPhase(SpiMode::Mode0) == SpiClockPhase::Cpha0);
    static_assert(spiClockPhase(SpiMode::Mode1) == SpiClockPhase::Cpha1);
    static_assert(spiClockPhase(SpiMode::Mode2) == SpiClockPhase::Cpha0);
    static_assert(spiClockPhase(SpiMode::Mode3) == SpiClockPhase::Cpha1);

    constexpr SpiMode spiMode(const SpiClockPolarity cpol, const SpiClockPhase cpha)
    {
        if (cpol == spiClockPolarity(SpiMode::Mode0) && cpha == spiClockPhase(SpiMode::Mode0)) return SpiMode::Mode0;
        if (cpol == spiClockPolarity(SpiMode::Mode1) && cpha == spiClockPhase(SpiMode::Mode1)) return SpiMode::Mode1;
        if (cpol == spiClockPolarity(SpiMode::Mode2) && cpha == spiClockPhase(SpiMode::Mode2)) return SpiMode::Mode2;
        if (cpol == spiClockPolarity(SpiMode::Mode3) && cpha == spiClockPhase(SpiMode::Mode3)) return SpiMode::Mode3;
        assert(false);
        return SpiMode::Mode0;
    }
    static_assert(spiMode(SpiClockPolarity::Cpol0, SpiClockPhase::Cpha0) == SpiMode::Mode0);
    static_assert(spiMode(SpiClockPolarity::Cpol0, SpiClockPhase::Cpha1) == SpiMode::Mode1);
    static_assert(spiMode(SpiClockPolarity::Cpol1, SpiClockPhase::Cpha0) == SpiMode::Mode2);
    static_assert(spiMode(SpiClockPolarity::Cpol1, SpiClockPhase::Cpha1) == SpiMode::Mode3);
   
    template< typename Pins_t, typename Config_t, typename PlatformConfig_t >
    Spi<Pins_t,Config_t,PlatformConfig_t>::Spi()
     : IoDescriptor {
          [](IoDescriptor& descriptor, uint8_t* const buffer, const uint16_t bufferLength) ->int32_t
            {
                return reinterpret_cast<Spi&>(descriptor).read(buffer, bufferLength);
            }
        , [](IoDescriptor& descriptor, const uint8_t* const buffer, const uint16_t bufferLength) ->int32_t
            {
                return reinterpret_cast<Spi&>(descriptor).write(buffer, bufferLength);
            } 
    }
    , config_{}
    , impl_{}
    {}

    template< typename Pins_t, typename Config_t, typename PlatformConfig_t>
    bool Spi<Pins_t, Config_t, PlatformConfig_t>::initialise(
          const Pins_t& pins
        , const Config_t& config
        , const PlatformConfig_t& platformConfig)
    {
        pins_ = pins;
        config_ = config;
        return initialiseGpio(pins, platformConfig)
            && impl_.initialise(config_, platformConfig);
    }

    template< typename Pins_t, typename Config_t, typename PlatformConfig_t>
    bool Spi<Pins_t, Config_t, PlatformConfig_t>::initialiseGpio(const Pins_t& pins, const PlatformConfig_t& platformConfig)
    {        
        gpioDirectionIn(pins.miso);// Set pin direction to input
        gpioPullDisable(pins.miso);
        gpioFunction(pins.miso, Function::Spi, platformConfig);

        gpioOutLow(pins.sck); ///< Initial level
        gpioDirectionOut(pins.sck);// Set pin direction to output
        gpioFunction(pins.sck, Function::Spi, platformConfig);

        gpioOutLow(pins.mosi);
        gpioDirectionOut(pins.mosi); // Set pin direction to output
        gpioFunction(pins.mosi, Function::Spi, platformConfig);

        return true;
    }

    template< typename Pins_t, typename Config_t, typename PlatformConfig_t>
    bool Spi<Pins_t, Config_t, PlatformConfig_t>::enable()
    {
        return impl_.enable();
    }

    template< typename Pins_t, typename Config_t, typename PlatformConfig_t>
    bool Spi<Pins_t, Config_t, PlatformConfig_t>::disable()
    {
        return impl_.disable();
    }


    template< typename Pins_t, typename Config_t, typename PlatformConfig_t >
    int32_t Spi<Pins_t, Config_t, PlatformConfig_t>::transfer(const SpiMessage& message)
    {
        return impl_.transfer(message);
    }

    template< typename Pins_t, typename Config_t, typename PlatformConfig_t >
    int32_t Spi<Pins_t, Config_t, PlatformConfig_t>::read(uint8_t* const buffer, const uint16_t bufferLength, const bool doStop)
    {
        /// @todo CS may be hardware controlled!
        BareCpper::gpioOutLow(pins_.cs); //< Start/resume comms

        SpiMessage message = {};
        message.bufferLength = bufferLength;
        message.rxBuffer = buffer;

        const int32_t retVal = transfer(message);

        if (doStop || (retVal == 0))
        {
            BareCpper::gpioOutHigh(pins_.cs); //< Stop comms
        }
        return retVal;
    }

    template< typename Pins_t, typename Config_t, typename PlatformConfig_t >
    int32_t Spi<Pins_t, Config_t, PlatformConfig_t>::write(const uint8_t* const buffer, const uint16_t bufferLength, const bool doStop )
    {
        /// @todo CS may be hardware controlled!
        BareCpper::gpioOutLow(pins_.cs); //< Start/resume comms

        SpiMessage message = {};
        message.bufferLength = bufferLength;
        message.txBuffer = buffer;

        const int32_t retVal = transfer(message);

        if (doStop || (retVal == 0))
        {
            BareCpper::gpioOutHigh(pins_.cs); //< Stop comms
        }
        return retVal;

    }

} //< END: BareCpper

#endif