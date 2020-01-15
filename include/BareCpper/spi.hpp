/*
 * Serial_SPI.hpp
 *
 * Created: 10/02/2013 15:12:58
 *  Author: CraigH
 */
#ifndef BARECPPER_SPI_H_
#define BARECPPER_SPI_H_

#include <cstdint> //< uint8_t

namespace BareCpper
{

    //We allow disabling implementation if they are currently not compatible
    // with a platform by defining these constants as 0 i.e. '#define IMPL_SPI_Software 0'. 
    // May also aid compile times and code size

#if !defined(IMPL_SPI_Software)
#	define IMPL_SPI_Software 1 //< Enable/disable implementation support
#endif

#if !defined(SPI_IMPL_HARDWARE)
// @todo SPIE is Atmel specific
#	define SPI_IMPL_HARDWARE (defined(SPIE))//< Enable/disable implementation support
#endif

#if !defined(IMPL_SPI_ArduinoSw)
#	define IMPL_SPI_ArduinoSw (defined(ARDUINO)) //< Enable/disable implementation support
#endif

#ifndef IMPL_SPI_Arduino
// @todo SPIE is Atmel specific
#	define IMPL_SPI_Arduino (defined(ARDUINO) && defined(SPIE)) //< Enable/disable implementation support
#endif

#if !defined(SPIE) && (IMPL_SPI_Arduino || SPI_IMPL_HARDWARE)
#error "Hardware SPI is not supported on the current device, "
#endif

#if !defined(ARDUINO) && (IMPL_SPI_Arduino || IMPL_SPI_ArduinoSw)
#error "Arduino SPI implementation is not available, requires Arduino Library and ARDUINO define"
#endif

/** @{ Each implementation is tagged with a Serial_SPI sub-type implementation
    \remarks Not all processors have Hardware SPI/UART and a software implementation may be used in its place
*/
#if IMPL_SPI_Software
/** Fast software SPI via direct port manipulation and MSBFIRST only support at present
*/
    struct SPI_Software;
#endif

#if IMPL_SPI_ArduinoSw

    /** Arduino implementation of software SPI via the extremely slow Arduino IO library
    */
    struct SPI_ArduinoSW;
#endif

#if SPI_IMPL_HARDWARE
    /** Hardware SPI implementation
    */
    struct SPI_Hardware;
#endif

#if IMPL_SPI_Arduino
    /** Arduino hardware implementation using the slow Arduino library
    */
    struct SPI_HardwareSlow;
#endif

    /// @note We always default to hardware support where available
#if SPI_IMPL_HARDWARE
    typedef  SPI_Hardware SPI_Default;
#elif SPI_IMPL_SOFTWARE
    typedef SPI_Software SPI_Default;
#endif

    /// }@


     /** Defines the bit-order for serial communication
         \remarks SPIBitOrder_MSBFirst MSB appears to be more common but check device documentation etc.
     */
    enum eSPIBitOrder : uint8_t
    {
        SPIBitOrder_LSBFirst = 0, //< Bits are output from right to left (binary 1 '00000001' will output bit value 1 first)
        SPIBitOrder_MSBFirst = 1, //< Bits are output from left to right (binary 1 '00000001' will output bit value 0 first)
    };

    /* Register (SPCR).:
        CPOL The clock polarity is specified by the CPOL control bit
        CPHA clock phase control bit selects one of the two fundamentally different transfer formats
    */
    enum eSPIPolarity : uint8_t
    {
        SPIPolarity_0 = 0, //idle low, active high
        SPIPolarity_1 = 1, //<idle high, active low		
    };

    enum eSPIPhase : uint8_t
    {
        SPIPhase_0 = 0, //< http://www.atmel.com/Images/doc2585.pdf Chapter 2.4
        SPIPhase_1 = 1, //<  http://www.atmel.com/Images/doc2585.pdf Chapter 2.4
    };

    /** SPI clocking mode which is a combination of Polarity and Phase
        \remarks http://www.atmel.com/Images/doc2585.pdf Chapter 2.4
    */
    enum eSPIDataMode : uint8_t
    {
        SPIDataMode_0 = SPIPolarity_0 | SPIPhase_0, //< Shift SCK: Falling, Capture SCK: Rising
        SPIDataMode_1 = SPIPolarity_0 | SPIPhase_1, //< Shift SCK: Rising, Capture SCK: Falling
        SPIDataMode_2 = SPIPolarity_1 | SPIPhase_0, //< Shift SCK: Rising, Capture SCK: Falling
        SPIDataMode_3 = SPIPolarity_1 | SPIPhase_1, //< Shift SCK: Falling, Capture SCK: Rising

        SPIDataMode_Mask = SPIPolarity_1 | SPIPhase_1 | SPIPolarity_0 | SPIPhase_0
    };

    template<
        uint8_t TPin_SS, //, Device select pin (Arduino Playground = 7)
        uint8_t TPin_MOSI, //, Config_t::Pin_MOSI/DN pin (Arduino Playground = 4)
        uint8_t TPin_MISO, //< MISO output pin
        uint8_t TPin_SCLK,  //< Clock pin (Arduino Playground = 3)
        eSPIDataMode TDataMode,
        eSPIBitOrder TBitOrder >
    struct SPIConfig
    {
        enum ePin : uint8_t
        {
            Pin_SS = TPin_SS,
            Pin_MISO = TPin_MISO,
            Pin_MOSI = TPin_MOSI,
            Pin_SCLK = TPin_SCLK
        };

        enum eDataMode
        {
            DataMode = TDataMode,
        };
        enum eBitOrder
        {
            BitOrder = TBitOrder
        };
    };

    //@todo This fitted into a idea/design somehow...
    struct Serial_SPI;
    template <  typename Transport_t, typename Impl_t > class SerialImpl;
    template <  typename Transport_t, typename Impl_t, typename Config_t > class SerialInterface;
    
    template < typename Impl_t >
    class SerialImpl< Serial_SPI, Impl_t >
    {
    public:
        inline void initialise(uint8_t kPin_SS, uint8_t kPin_MISO, uint8_t kPin_MOSI, uint8_t kPin_SCLK);

        inline void destroy( /*uint8_t kPin_SS, uint8_t kPin_MISO, uint8_t kPin_MOSI, uint8_t kPin_SCLK*/);

        inline void begin(uint8_t kPin_SS);

        inline void end(uint8_t kPin_SS);

        inline void write(uint8_t kPin_MOSI, uint8_t kPin_SCLK, uint8_t val);

        /** SPI Special functions
        */
        inline void setClockDivider(uint8_t rate); //< Valid only on hardware!?

        inline void setDataMode(eSPIDataMode mode);

        inline void setBitOrder(eSPIBitOrder bitOrder);
    };


    template < typename Impl_t, typename Config_t >
    class SerialInterface<Serial_SPI, Impl_t, Config_t> : public SerialImpl<Serial_SPI, Impl_t>
    {
        typedef SerialImpl<Serial_SPI, Impl_t> Impl;

    public:

        SerialInterface() {}
        ~SerialInterface() { destroy(); }

        void initialise()
        {
            Impl::initialise(Config_t::Pin_SS, Config_t::Pin_MISO, Config_t::Pin_MOSI, Config_t::Pin_SCLK);
        }

        void destroy()
        {
            end();
            Impl::destroy( /*Config_t::Pin_SS, Config_t::Pin_MISO, Config_t::Pin_MOSI, Config_t::Pin_SCLK*/);
        }

        void begin()
        {
            Impl::begin(Config_t::Pin_SS);
        }

        void end()
        {
            Impl::end(Config_t::Pin_SS);
        }

        void write(uint8_t val)
        {
            Impl::write(Config_t::Pin_MOSI, Config_t::Pin_SCLK, val);
        }

        /** SPI Special functions
        */
        void setClockDivider(uint8_t rate) //< Valid only on hardware!?
        {
            Impl::setClockDivider(rate);
        }

        void setDataMode(eSPIDataMode mode)
        {
            Impl::setDataMode(mode);
        }

        void setBitOrder(eSPIBitOrder bitOrder)
        {
            Impl::setBitOrder(bitOrder);
        }
    };


    template < typename Impl_t, typename Config_t >
    using SPI = SerialInterface<Serial_SPI, Impl_t, Config_t>;


} //END: BareCpper

#if IMPL_SPI_Software
#include "Spi_Software.hpp"
#endif

#if IMPL_SPI_ArduinoSw
#include "Arduino/Spi_ArduinoSw.hpp"
#endif

#if IMPL_SPI_Arduino
#include "Arduino/Spi_Arduino.hpp"
#endif

#if SPI_IMPL_HARDWARE
#if _AVR_IOM328P_H_
#include "ATmega/Spi_ATmega328P.h"
#else
#  error "AVR library error, please check and update accordingly."
#endif
#endif

#endif //BARECPPER_SPI_H_