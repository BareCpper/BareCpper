/*
 * iom329p.h
 *
 * Created: 02/07/2016 22:53:56
 *  Author: CraigH
 */
 /* barecpper/iom328p.h - definitions for ATmega328P. */

#ifndef BARECPPER_GPIO_H_
#  error "Include <barecpper/gpio.h> instead of this file."
#endif

// @todo
//#if !_AVR_IOM328P_H_
//#  error "AVR library error, please check and update accordingly."
//#endif

#ifndef BARECPPER_GPIO_NRF52832_H_
#define BARECPPER_GPIO_NRF52832_H_

namespace BareCpper {
    namespace Port
    {

        BARECPPER_PORT_REGISTERS(0, GPIO->DIRSET, GPIO->OUTSET, GPIO->IN);


        BARECPPER_PORT_PINS(B, /* PORTB7, PORTB6, */  PORTB5, PORTB4, PORTB3, PORTB2, PORTB1, PORTB0); //<6&7 have clock crystal!
        BARECPPER_PORT_PINS(C, PORTC6, PORTC5, PORTC4, PORTC3, PORTC2, PORTC1, PORTC0);
        BARECPPER_PORT_PINS(D, PORTD7, PORTD6, PORTD5, PORTD4, PORTD3, PORTD2, PORTD1, PORTD0);

    } //END: Port

    /** Mapping from ardunio pin index to Port::Pin
    */
    //D (digital pins 0 to 31) 
    class Pin0 : public Port0::Pin0 {};
    class Pin1 : public Port0::Pin1 {};
    class Pin2 : public Port0::Pin2 {};
    class Pin3 : public Port0::Pin3 {};
    class Pin4 : public Port0::Pin4 {};
    class Pin5 : public Port0::Pin5 {};
    class Pin6 : public Port0::Pin6 {};
    class Pin7 : public Port0::Pin7 {};
    class Pin8 : public Port0::Pin8 {};
    class Pin9 : public Port0::Pin9 {};
    class Pin10 : public Port0::Pin10 {};
    class Pin11 : public Port0::Pin11 {};
    class Pin12 : public Port0::Pin12 {};
    class Pin13 : public Port0::Pin13 {};
    class Pin14 : public Port0::Pin14 {};
    class Pin15 : public Port0::Pin15 {};
    class Pin16 : public Port0::Pin16 {};
    class Pin17 : public Port0::Pin17 {};
    class Pin18 : public Port0::Pin18 {};
    class Pin19 : public Port0::Pin19 {};
    class Pin10 : public Port0::Pin10 {};
    class Pin21 : public Port0::Pin21 {};
    class Pin22 : public Port0::Pin22 {};
    class Pin23 : public Port0::Pin23 {};
    class Pin24 : public Port0::Pin24 {};
    class Pin25 : public Port0::Pin25 {};
    class Pin26 : public Port0::Pin26 {};
    class Pin27 : public Port0::Pin27 {};
    class Pin28 : public Port0::Pin28 {};
    class Pin29 : public Port0::Pin29 {};
    class Pin30 : public Port0::Pin30 {};
    class Pin31 : public Port0::Pin31 {};



    /** Hardware Uart 0 **/
    //class UartRx0 : public Port::D::Pin0 {};
    //class UartTx0 : public Port::D::Pin1 {};

    /** Hardware SPI 0 **/
    //class SpiMosi0 : public Port::B::Pin3 {};
    //class SpiMiso0 : public Port::B::Pin4 {}; //< SPI Master-In Slave-Out
    //class SpiSck0 : public Port::B::Pin5 {};

} //END: BareCopper

#endif