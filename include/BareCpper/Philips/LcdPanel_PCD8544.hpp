#ifndef BARECPPER_LCDPANEL_PCD8544_H
#define BARECPPER_LCDPANEL_PCD8544_H

#include <Arduino.h>
#include "Types.h"
#include "SerialInterface.h"

//TODO: we could ahve a define to select different panel types etc
#include "PCD8544.h"

namespace BareCpper 
{

    /** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
    */
    class LCDPanel_PCD8544 :
        public PCD8544,
        public SerialInterface<Serial_SPI, SPI_SoftwareFast>
    {
    public:

        typedef PCD8544 Settings;

    public:
        LCDPanel_PCD8544();
        ~LCDPanel_PCD8544();

        bool initialise();

        /** \note Must be terminated with a subsequent call to end()
        */
        void beginData()
        {
            begin(Mode_D);
        }

        /** \note Must be terminated with a subsequent call to end()
        */
        void beginCommand()
        {
            begin(Mode_C);
        }

        /** \note Must be terminated with a subsequent call to end()
        */
        void begin();

        /** Send data/command data value
            \remarks Data is interpreted differently depending on beginData()/beginCommand() calls
        */
        void sendData(Byte data)
        {
            serialOut(Pin_SDIN, Pin_SCLK, data);
        }

        void sync()
        { /* Do nothign as we are an immediate device */
        }

        void end();

        /** \note Must be in command mode
        */
        void setCursor(UInt8 column, UInt8 line);

        /** \note Must be in command mode
        */
        void setColumn(UInt8 column);

        /** \note Must be in command mode
        */
        void setLine(UInt8 line);

        void setPower(bool on);
        void setInverse(bool inverse);

    protected:


        void begin(eMode dcMode);

    private:
        struct Cursor
        {
            UInt8 column; //< max = Dim_X
            UInt8 line; //< max = Dim_Lines
        };

        Cursor m_cursor;
    };

} // END: BareCpper
#endif

