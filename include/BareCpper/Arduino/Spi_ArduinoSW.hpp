#ifndef BARECPPER_SPI_H_
#  error "Include <barecpper/spi.h> instead of this file."
#endif

#if !ARDUINO
#  error "ARDUINO library error, please check and update accordingly."
#endif


namespace BareCpper
{

    struct SPI_ArduinoSW;

    template<>
    inline void SerialImpl<Serial_SPI, SPI_ArduinoSW>::initialise(uint8_t kPin_SS, uint8_t kPin_MISO, uint8_t kPin_MOSI, uint8_t kPin_SCLK)
    { //TODO:
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_ArduinoSW>::destroy( /*uint8_t kPin_SS, uint8_t kPin_MISO, uint8_t kPin_MOSI, uint8_t kPin_SCLK*/)
    { //TODO:
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_ArduinoSW>::begin(uint8_t kPin_SS)
    { //TODO:
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_ArduinoSW>::end(uint8_t kPin_SS)
    { //TODO:
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_ArduinoSW>::write(uint8_t kPin_MOSI, uint8_t kPin_SCLK, uint8_t val)
    {
        ::shiftOut(kPin_MOSI, kPin_SCLK, MSBFIRST, val);
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_ArduinoSW>::setClockDivider(uint8_t rate)
    { //TODO:
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_ArduinoSW>::setDataMode(eSPIDataMode mode)
    { //TODO:
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_ArduinoSW>::setBitOrder(eSPIBitOrder bitOrder)
    { //TODO:	
    }

} //END: BareCpper