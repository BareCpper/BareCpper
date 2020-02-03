#ifndef BARECPPER_LCD_PCD8544_H
#define BARECPPER_LCD_PCD8544_H


namespace BareCpper {

    /** @todo REFACTOR: Only tested in early Atmega platform wihtout newer interfaces etc
    */
    class PCD8544
    {
    public:

        /** The pins to use on the arduino for the pins on the LCD
            \remarks Defined as constant to save program/memory space
        */
        enum ePin
        {
#if 1	//< Hardware SPI compatible pins
            Pin_SCE = 8, //, Device select pin (Arduino Playground = 7)
            Pin_RESET = 9, //< Device rest pin (Arduino Playground = 6)
            Pin_DC = 10, //< Data/Command state pin (Arduino Playground = 5)
            Pin_SDIN = 11, //, MOSI/DN pin (Arduino Playground = 4)
            Pin_SCLK = 13,  //< Clock pin (Arduino Playground = 3)
#else	//< Arduino playground defaults	
            Pin_SCE = 7,
            Pin_RESET = 6,
            Pin_DC = 5,
            Pin_SDIN = 4,
            Pin_SCLK = 3,
#endif
        };

        /** LCD dimension attributes
        */
        enum eDim
        {
            Dim_X = 84, //<Nokia 5110 has 83 columns
            Dim_Y = 48,	//< Nokia 5110 has 5

            Dim_Columns = Dim_X,
            Dim_Lines = Dim_Y / 8,
        };

        /** LCD Message modes
        */
        enum eMode
        {
            Mode_C = LOW,
            Mode_D = HIGH,
        };

        /** LCD Commands
        */
        enum eCmd
        {
            Cmd_PowerOn = 0x20,
            Cmd_PowerOff = 0x24,
            Cmd_DisplayInvert = 0x0D,
            Cmd_DisplayNormal = 0x0C,
            Cmd_SetColumn = 0x80,
            Cmd_SetLine = 0x40,
        };

    };

} // END: BareCpper

#endif

