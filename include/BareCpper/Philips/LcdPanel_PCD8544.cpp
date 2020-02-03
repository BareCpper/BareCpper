#include "LCDPanel_PCD8544.h"
#include "CharacterSet_8x5.h"

namespace BareCpper
{
    LCDPanel_PCD8544::LCDPanel_PCD8544()
    {
        m_cursor.column =
            m_cursor.line = 0;
    }

    LCDPanel_PCD8544::~LCDPanel_PCD8544()
    {
        setPower(false);
    }

    bool LCDPanel_PCD8544::initialise()
    {
        pinMode(Pin_SCE, OUTPUT);
        pinMode(Pin_RESET, OUTPUT);
        pinMode(Pin_DC, OUTPUT);
        pinMode(Pin_SDIN, OUTPUT);
        pinMode(Pin_SCLK, OUTPUT);

        digitalWriteFast(Pin_RESET, LOW);
        digitalWriteFast(Pin_RESET, HIGH);

        beginCommand();

        sendData(0x21);  // LCD Extended Commands.
        sendData(0xBf);  // Set LCD Vop (Contrast). //B1
        sendData(0x04);  // Set Temp coefficent. //0x04
        sendData(0x13);  // LCD bias mode 1:48. //0x13
        sendData(Cmd_DisplayInvert); //< if we remove this 'Fill' tests are 4micros slower!?
        sendData(Cmd_PowerOn);
        sendData(Cmd_DisplayInvert);  // LCD in normal mode. 0x0d for inverse

        end();

        return true;
    }

    void LCDPanel_PCD8544::begin(eMode dcMode)
    {
        digitalWriteFast(Pin_DC, dcMode);//Set us to data mode
        begin();
    }

    void LCDPanel_PCD8544::begin()
    {
        digitalWriteFast(Pin_SCE, LOW); //, select serial device for communication
    }

    void LCDPanel_PCD8544::end()
    {
        digitalWriteFast(Pin_SCE, HIGH); //, disable device for communication
    }

    void LCDPanel_PCD8544::setCursor(UInt8 column, UInt8 line)
    {
        setColumn(column);
        setLine(line);
    }

    void LCDPanel_PCD8544::setColumn(UInt8 column)
    {
        m_cursor.column = column;
        sendData(Cmd_SetColumn | column);  // Column.
    }
    void LCDPanel_PCD8544::setLine(UInt8 line)
    {
        m_cursor.line = line;
        sendData(Cmd_SetLine | line);  // Column.
    }

    void LCDPanel_PCD8544::setPower(bool on)
    {
        sendData(on ? Cmd_PowerOn : Cmd_PowerOff);
    }

    void LCDPanel_PCD8544::setInverse(bool inverse)
    {
        sendData(inverse ? 0x0d : 0x0c);
    }

} // END: BareCpper