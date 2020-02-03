#ifndef BARECPPER_LCDDISPLAY_PCD8544_H
#define BARECPPER_LCDDISPLAY_PCD8544_H

#include "Types.h"
//#include "LCDPanel_PCD8544.h"
#include <arduino.h>


/** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
*/
template < typename _TPanel >
class LCDPanelBufferedInterface
{
public:
    typedef LCDPanel Panel;
public:

    LCDPanelBufferedInterface(LCDPanel& panel)
        : m_cursor(0),
        m_panel(panel)
    {}

    void sync()
    {
        m_panel.beginData();
        for (UInt8* buffer = m_buffer; buffer != m_buffer + _TPanel::Dim_Columns * _TPanel::Dim_Lines; ++buffer)
            m_panel.sendData(*buffer);
        m_panel.end();
    }

    /** \note Must be terminated with a subsequent call to end()
    */
    void beginData()
    {
    }

    /** \note Must be terminated with a subsequent call to end()
    */
    void beginCommand()
    {
        //m_mode = Mode_Command; 
        m_panel.beginCommand();
    }

    /** \note Must be terminated with a subsequent call to end()
    */
    void begin()
    {
        m_panel.begin();
    }

    /** Send data/command data value
        \remarks Data is interpreted differently depending on beginData()/beginCommand() calls
    */
    void sendData(Byte data)
    {
        m_buffer[m_cursor++] = data;
        if (m_cursor == _TPanel::Dim_Columns * _TPanel::Dim_Lines)
            m_cursor = 0;
    }


    void end()
    {
        m_panel.end();
    }

    /** \note Must be in command mode
    */
    void setCursor(UInt8 column, UInt8 line)
    {
        m_cursor = column + line * _TPanel::Dim_Columns;
    }

    /** \note Must be in command mode
    */
    void setColumn(UInt8 column)
    {
        m_cursor = column;
    }

    /** \note Must be in command mode
    */
    void setLine(UInt8 line)
    {
        m_cursor = line;
    }

protected:
    UInt8 m_buffer[_TPanel::Dim_Lines * _TPanel::Dim_Columns];
    UInt16 m_cursor;
    _TPanel& m_panel;
};


/** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
*/
/** LCDDisplay buffer/interface
    \param _TInterface Type of interface:
        _TPanel& for direct immediate usage
        LCDPanelBufferedInterface<_TPanel> for buffered usage. Requires call to sync() for display
*/
template < typename _TPanel, typename _TInterface = _TPanel& >
class LCDDisplay
{
public:
    typedef LCDPanel Panel;
public:

    LCDDisplay(LCDPanel& panel)
        : m_interf(panel)
    {}

    void clear()
    {
        fill(0x00);
    }

    void clearLine()
    {
        fillLine(0x00);
    }

    void fill(UInt8 val);

    void fillLine(UInt8 val);

    void write(Char chr);
    void writeBig(Char chr);
    void write(const Char* str);
    void write(const __FlashStringHelper* progmemStr);

    void sync()
    {
        m_interf.sync();
    }

protected:
    _TInterface m_interf;
};

#endif