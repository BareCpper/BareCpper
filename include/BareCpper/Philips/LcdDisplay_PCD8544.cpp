#include "LCDDisplay.h"
#include "CharacterSet_8x5.h"

#include "LCDPanel.h"
#include "PCD8544.h"

/** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
*/
template <typename _TPanel, typename _TInterface>
void LCDDisplay<_TPanel, _TInterface>::fill(UInt8 data)
{
    //Clear the screen
    // - It doesn't matter about current cursor position as the cursor will wrap
    //NOTE: Due to Uint8 overflow the fill operation is done in multiple loops
    //so we can maintain 8-bit native types for faster performance

    m_interf.beginData();

    //< 5340 microns!
    UInt16 rem = Panel::Dim_X * Panel::Dim_Lines;
    while (rem)
    {
        UInt8 cnt = (rem < 255u ? rem : 255u);
        for (UInt8 i = 0; i < cnt; ++i)
            m_interf.sendData(data);
        rem -= cnt;
    }

    m_interf.end();
}


/** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
*/
template <typename _TPanel, typename _TInterface>
void LCDDisplay<_TPanel, _TInterface>::fillLine(UInt8 data)
{
    //Clear the current line
    m_interf.beginCommand();
    m_interf.setColumn(0); //, first return to the start...
    m_interf.beginData();

    for (UInt8 i = 0; i < Panel::Dim_X; ++i)
        m_interf.sendData(data);

    m_interf.end();
}


/** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
*/
template <typename _TPanel, typename _TInterface>
void LCDDisplay<_TPanel, _TInterface>::write(Char chr)
{
    CharGlyph glyph;
    if (!getCharGlyph(chr, glyph))
        return;

    m_interf.beginData();//Set us to data mode

    for (UInt8 col = 0; col < 5; ++col)
        m_interf.sendData(glyph[col]);

    //Add empty column to seperate characters
    m_interf.sendData(0x00);

    m_interf.end();
}


/** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
*/
static PROGMEM const UInt8 kDoubled[16] =
{
    B00000000,
    B00000011,
    B00001100,
    B00001111,
    B00110000,
    B00110011,
    B00111100,
    B00111111,
    B11000000,
    B11000011,
    B11001100,
    B11001111,
    B11110000,
    B11110011,
    B11111100,
    B11111111,
};


/** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
*/
UInt8 doubleBits(UInt8 val)
{
#if 0
    val |= (val & B00001100) << 2; //< move upper two bits
    val |= (val & B00100010) << 1;
    return (val & B00010001) | ((val & B01110111) << 1);
#else

    return pgm_read_byte(kDoubled + val);
#endif
}


/** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
*/
inline void doubleBits(UInt8 val, UInt8& top, UInt8& bottom)
{
    top = pgm_read_byte(kDoubled + (val & 0xF));
    bottom = pgm_read_byte(kDoubled + (val >> 4));
}


/** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
*/
template <typename _TPanel, typename _TInterface>
void LCDDisplay<_TPanel, _TInterface>::writeBig(Char chr)
{
    CharGlyph glyph;
    if (!getCharGlyph(chr, glyph))
        return;

    //setCursor( 5, 0 );

    m_interf.beginData();//Set us to data mode

    for (UInt8 col = 0; col < 5; ++col)
    {
        UInt8 top;
        doubleBits(glyph[col], top, glyph[col]); //< top half
        m_interf.sendData(top);
        m_interf.sendData(top);
    }
    //m_interf.end();

    m_interf.beginCommand();
    m_interf.setCursor(6, 1);

    m_interf.beginData();//Set us to data mode

    for (UInt8 col = 0; col < 5; ++col)
    {
        m_interf.sendData(glyph[col]);
        m_interf.sendData(glyph[col]);
    }

    m_interf.beginCommand();

    m_interf.setLine(0);
    m_interf.end();
}


/** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
*/
template <typename _TPanel, typename _TInterface>
void LCDDisplay<_TPanel, _TInterface>::write(const __FlashStringHelper* progmemStr)
{
    m_interf.beginData();

    const char PROGMEM* str = (const char PROGMEM*)progmemStr;
    while (unsigned char chr = pgm_read_byte(str++))
    {
        CharGlyph glyph;
        if (getCharGlyph(chr, glyph))
        {
            //Write out the character
            for (UInt8 col = 0; col < 5; ++col)
                m_interf.sendData(glyph[col]);

            //Add empty column to seperate characters
            m_interf.sendData(0x00);
        }
    }
    m_interf.end();
}


/** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
*/
template <typename _TPanel, typename _TInterface>
void LCDDisplay<_TPanel, _TInterface>::write(const Char* str)
{
    m_interf.beginData();

    while (*str)
    {
        CharGlyph glyph;
        if (getCharGlyph(*str++, glyph))
        {
            //Write out the character
            for (UInt8 col = 0; col < 5; ++col)
                m_interf.sendData(glyph[col]);

            //Add empty column to seperate characters
            m_interf.sendData(0x00);
        }
    }
    m_interf.end();
}

/** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
*/
#include "LCDPanel_PCD8544.h"
template class LCDDisplay<LCDPanel, LCDPanel& >;
template class LCDDisplay<LCDPanel, LCDPanelBufferedInterface<LCDPanel> >;