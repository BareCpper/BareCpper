#ifndef BARECPPER_SPI_H_
#  error "Include <barecpper/spi.h> instead of this file."
#endif


namespace BareCpper
{

    struct SPI_Software;

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Software>::initialise(uint8_t kPin_SS, uint8_t kPin_MISO, uint8_t kPin_MOSI, uint8_t kPin_SCLK)
    {
        // Set direction register for kPin_SCLK and kPin_MOSI pin.
        setPinAsOutput(kPin_SCLK);
        setPinAsOutput(kPin_MOSI);
        setPinAsOutput(kPin_SS);

        digitalBitClear(kPin_SCLK);
        digitalBitClear(kPin_MOSI);
        digitalBitSet(kPin_SS);

        setPinAsInput(kPin_MISO); // SPI DI
        digitalBitSet(kPin_MISO); // SPI DI
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Software>::destroy( /*uint8_t kPin_SS, uint8_t kPin_MISO, uint8_t kPin_MOSI, uint8_t kPin_SCLK*/)
    { //TODO:
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Software>::begin(uint8_t kPin_SS)
    {
        digitalBitClear(kPin_SS); //, select serial device for communication
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Software>::end(uint8_t kPin_SS)
    {
        digitalBitSet(kPin_SS); //, disable device for communication
    }

    //__attribute__((always_inline))
    template<>
    inline void SerialImpl<Serial_SPI, SPI_Software>::write(uint8_t kPin_MOSI, uint8_t kPin_SCLK, uint8_t val)
    {
        //Send each bit of the val byte MSBFIRST
        // @todo Configurable bit-order
        for (uint8_t mask = 1 << 7u; mask; mask >>= 1u)
        {
            digitalBitClear(kPin_SCLK);
#if 0 //< Saves 32 cycles on ATmega328!?
            digitalBitWriteAndSet(kPin_MOSI, (val & mask), kPin_SCLK);
#else
            digitalBitWrite(kPin_MOSI, (val & mask));	//Set data pin state based on input values bit
            digitalBitSet(kPin_SCLK);//Bit is registered on upstroke of clock
#endif
        }

        // @TODO: COuld save power?
        //digitalBitClear(kPin_MOSI);
        //digitalBitClear(kPin_SCLK);
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Software>::setClockDivider(uint8_t rate)
    { //TODO:
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Software>::setDataMode(eSPIDataMode mode)
    { //TODO:
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Software>::setBitOrder(eSPIBitOrder bitOrder)
    { //TODO:	
    }

} //END: BareCpper