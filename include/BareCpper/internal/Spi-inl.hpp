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
   
    template< typename Pins_t, typename PlatformConfig_t >
    Spi<Pins_t,PlatformConfig_t>::Spi()
     : /* IoDescriptor {
          [](IoDescriptor& descriptor, uint8_t* const buffer, const uint16_t bufferLength) ->int32_t
            {
                return reinterpret_cast<Spi&>(descriptor).read(buffer, bufferLength);
            }
        , [](IoDescriptor& descriptor, const uint8_t* const buffer, const uint16_t bufferLength) ->int32_t
            {
                return reinterpret_cast<Spi&>(descriptor).write(buffer, bufferLength);
            } 
    }
    ,*/
     impl_{}
    {}

    template< typename Pins_t, typename PlatformConfig_t>
    bool Spi<Pins_t, PlatformConfig_t>::initialise(
          const Pins_t& pins
        , const PlatformConfig_t& platformConfig)
    {
        return initialiseGpio(pins, platformConfig)
            && impl_.initialise(platformConfig);
    }

    template< typename Pins_t, typename PlatformConfig_t>
    bool Spi<Pins_t, PlatformConfig_t>::initialiseGpio(const Pins_t& pins, const PlatformConfig_t& platformConfig)
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

    template< typename Pins_t, typename PlatformConfig_t >
    template< typename Config_t >
    Spi<Pins_t, PlatformConfig_t>::Instance<Config_t>::Instance(spi_t* device)
        : device_(device)
        , config_{}
    {}

    template< typename Pins_t, typename PlatformConfig_t >
    template< typename Config_t >
    bool Spi<Pins_t, PlatformConfig_t>::Instance<Config_t>::initialise()
    {
        // @todo Should only be done once but occurs per instance!?!?
        if (!device_->initialise())
            return false;

        return true;
    }

    template< typename Pins_t, typename PlatformConfig_t >
    template< typename Config_t >
    bool Spi<Pins_t, PlatformConfig_t>::Instance<Config_t>::configure(const Config_t& config)
    {
        config_ = config;
        gpioDirectionOut(config_.chipSelect);// Set pin direction to out
        gpioOutHigh(config_.chipSelect); //< Innactive slave select

        return true;
    }

    template< typename Pins_t, typename PlatformConfig_t >
    template< typename Config_t >
    int32_t Spi<Pins_t, PlatformConfig_t>::Instance<Config_t>::transfer(const SpiMessage& message)
    {
        return device_->impl_.transfer(message);
    }

    template< typename Pins_t, typename PlatformConfig_t >
    template< typename Config_t >
    bool Spi<Pins_t, PlatformConfig_t>::Instance<Config_t>::enable()
    {
        const bool retVal = device_->impl_.configure(config_) && device_->impl_.enable();
        BareCpper::gpioOutLow(config_.chipSelect); //< Start/resume comms
        return retVal;
    }

    template< typename Pins_t, typename PlatformConfig_t >
    template< typename Config_t >
    bool Spi<Pins_t, PlatformConfig_t>::Instance<Config_t>::disable()
    {
        BareCpper::gpioOutHigh(config_.chipSelect); //< Stop/Cancel comms
        return device_->impl_.disable();
    }

    template< typename Pins_t, typename PlatformConfig_t >
    template< typename Config_t >
    int32_t Spi<Pins_t, PlatformConfig_t>::Instance<Config_t>::read(uint8_t* const buffer, const uint16_t bufferLength, const bool doStop)
    {
        if (!enable())
            return -1;

        SpiMessage message = {};
        message.bufferLength = bufferLength;
        message.rxBuffer = buffer;

        const int32_t retVal = transfer(message);

        if (doStop || (retVal == 0))
        {
            disable(); //< Stop comms
        }
        return retVal;
    }

    template< typename Pins_t, typename PlatformConfig_t >
    template< typename Config_t >
    int32_t Spi<Pins_t, PlatformConfig_t>::Instance<Config_t>::write(const uint8_t* const buffer, const uint16_t bufferLength, const bool doStop )
    {
        if (!enable())
            return -1;

        SpiMessage message = {};
        message.bufferLength = bufferLength;
        message.txBuffer = buffer;

        const int32_t retVal = transfer(message);

        if (doStop || (retVal <= 0))
        {
            disable(); //< Stop comms
        }
        return retVal;

    }

} //< END: BareCpper

#endif