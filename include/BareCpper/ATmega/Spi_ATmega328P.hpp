/*
 * Created: 02/07/2016 22:53:56
 *  Author: CraigH
 */
 /* barecpper/iom328p.h - definitions for ATmega328P. */

#ifndef BARECPPER_SPI_H_
#  error "Include <BareCpper/Spi.h> instead of this file."
#endif

#if !_AVR_IOM328P_H_
#  error "AVR library error, please check and update accordingly."
#endif


namespace BareCpper
{

    typedef SPIConfig<8, 11, 12/*TODO: check*/, 13, SPIDataMode_0, SPIBitOrder_MSBFirst> SPIConfig_HardwareAtmega328;
    typedef SPIConfig<7, 4, 255, 3, SPIDataMode_0, SPIBitOrder_MSBFirst> SPIConfig_ArduinoPlaygroundAtmega328; //< Settings for ArduinoPlayground example for Nokia 3310 PCD8544 display
    typedef SPIConfig<PORTB3, PORTB0, PORTB1, PORTB2, SPIDataMode_0, SPIBitOrder_MSBFirst> SPIConfig_SoftwareATtiny13a;

    struct SPI_Hardware;

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::initialise(uint8_t kPin_SS, uint8_t kPin_MISO, uint8_t kPin_MOSI, uint8_t kPin_SCLK)
    {
        //setBitOrder( SPIBitOrder_MSBFirst );
        //setDataMode( SPIDataMode_0 );
        //setClockDivider( 4 );

        // Set direction register for kPin_SCLK and kPin_MOSI pin.  MISO pin automatically overrides to INPUT.
        // When the SS pin is set as OUTPUT, it can be used as a general purpose output port (it doesn't influence SPI operations).
        setPinAsOutput(kPin_SCLK);
        setPinAsOutput(kPin_MOSI);
        setPinAsOutput(kPin_SS);

        digitalBitClear(kPin_SCLK);
        digitalBitClear(kPin_MOSI);
        //digitalBitSet(kPin_SS);

        //TODO: Polarity << CPOL
        //TODO: Phase << CPHA


        // Enable SPI, Master, set clock rate fck/2 (maximum)
        SPCR |= _BV(MSTR);
        SPCR |= _BV(SPE);
        //SPCR |= (1<<SPE/*ENable SPI*/)|(1<<MSTR/*Master Device*/);
        //SPSR = (1<<SPI2X);
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::destroy( /*uint8_t kPin_SS, uint8_t kPin_MISO, uint8_t kPin_MOSI, uint8_t kPin_SCLK*/)
    {
        SPCR &= ~(1 << SPE);
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::begin(uint8_t kPin_SS)
    {
        digitalBitWrite(kPin_SS, LOW); //, select serial device for communication
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::end(uint8_t kPin_SS)
    {
        loop_until_bit_is_set(SPSR, SPIF); //< Make sure any pending write is finished
        digitalBitWrite(kPin_SS, HIGH); //, disable device for communication
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::write(uint8_t kPin_MOSI, uint8_t kPin_SCLK, uint8_t val)
    {
        loop_until_bit_is_set(SPSR, SPIF); //< Make sure any pending write is finished
        SPDR = val;
        // while(!(SPSR & (1<<SPIF))); //< Better!?
        //while (!(SPSR & _BV(SPIF)));

        // Get return Value;
        //uint8_t datain = SPDR;
        //return datain;
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::setClockDivider(uint8_t rate)
    {
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

        SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
        SPSR = (SPSR & ~SPI2X) | ((rate >> 2)& SPI2X);
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::setDataMode(eSPIDataMode mode)
    {
        SPCR = (SPCR & ~SPIDataMode_Mask) | mode;
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::setBitOrder(eSPIBitOrder bitOrder)
    {
        if (bitOrder == SPIBitOrder_LSBFirst) {
            SPCR |= 1 << DORD;
        }
        else {
            SPCR &= ~(1 << DORD);
        }
    }

} //END: BareCpper