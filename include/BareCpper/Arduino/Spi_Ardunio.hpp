#ifndef BARECPPER_SPI_H_
#  error "Include <barecpper/spi.h> instead of this file."
#endif

#if !ARDUINO
#  error "ARDUINO library error, please check and update accordingly."
#endif


namespace BareCpper
{

    struct SPI_Arduino;

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Arduino>::initialise(uint8_t kPin_SS, uint8_t kPin_MISO, uint8_t kPin_MOSI, uint8_t kPin_SCLK)
    {
        //TODO; make configurable!
        SPI.setBitOrder(MSBFIRST);
        SPI.setDataMode(SPI_MODE0);
        SPI.begin();
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Arduino>::destroy( /*uint8_t kPin_SS, uint8_t kPin_MISO, uint8_t kPin_MOSI, uint8_t kPin_SCLK*/)
    {
        SPI.end();
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Arduino>::begin(uint8_t kPin_SS)
    {
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Arduino>::end(uint8_t kPin_SS)
    { //TODO:
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Arduino>::write(uint8_t kPin_MOSI, uint8_t kPin_SCLK, uint8_t val)
    {
        SPI.transfer(val);
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Arduino>::setClockDivider(uint8_t rate)
    { //TODO:
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Arduino>::setDataMode(eSPIDataMode mode)
    { //TODO:
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Arduino>::setBitOrder(eSPIBitOrder bitOrder)
    { //TODO:
    }

} //END: BareCpper