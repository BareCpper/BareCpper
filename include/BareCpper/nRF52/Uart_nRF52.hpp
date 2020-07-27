#pragma once


namespace BareCpper {
namespace Serial
{

    class Uart
    {

        void initialise()
        {
        }

        void destroy()
        {
         
        }

        void begin()
        {
        /*
#define UARTE_BAUDRATE_BAUDRATE_Baud1200 (0x0004F000UL) /*!< 1200 baud (actual rate: 1205) */
#define UARTE_BAUDRATE_BAUDRATE_Baud2400 (0x0009D000UL) /*!< 2400 baud (actual rate: 2396) */
#define UARTE_BAUDRATE_BAUDRATE_Baud4800 (0x0013B000UL) /*!< 4800 baud (actual rate: 4808) */
#define UARTE_BAUDRATE_BAUDRATE_Baud9600 (0x00275000UL) /*!< 9600 baud (actual rate: 9598) */
#define UARTE_BAUDRATE_BAUDRATE_Baud14400 (0x003AF000UL) /*!< 14400 baud (actual rate: 14401) */
#define UARTE_BAUDRATE_BAUDRATE_Baud19200 (0x004EA000UL) /*!< 19200 baud (actual rate: 19208) */
#define UARTE_BAUDRATE_BAUDRATE_Baud28800 (0x0075C000UL) /*!< 28800 baud (actual rate: 28777) */
#define UARTE_BAUDRATE_BAUDRATE_Baud31250 (0x00800000UL) /*!< 31250 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud38400 (0x009D0000UL) /*!< 38400 baud (actual rate: 38369) */
#define UARTE_BAUDRATE_BAUDRATE_Baud56000 (0x00E50000UL) /*!< 56000 baud (actual rate: 55944) */
#define UARTE_BAUDRATE_BAUDRATE_Baud57600 (0x00EB0000UL) /*!< 57600 baud (actual rate: 57554) */
#define UARTE_BAUDRATE_BAUDRATE_Baud76800 (0x013A9000UL) /*!< 76800 baud (actual rate: 76923) */
#define UARTE_BAUDRATE_BAUDRATE_Baud115200 (0x01D60000UL) /*!< 115200 baud (actual rate: 115108) */
#define UARTE_BAUDRATE_BAUDRATE_Baud230400 (0x03B00000UL) /*!< 230400 baud (actual rate: 231884) */
#define UARTE_BAUDRATE_BAUDRATE_Baud250000 (0x04000000UL) /*!< 250000 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud460800 (0x07400000UL) /*!< 460800 baud (actual rate: 457143) */
#define UARTE_BAUDRATE_BAUDRATE_Baud921600 (0x0F000000UL) /*!< 921600 baud (actual rate: 941176) */
#define UARTE_BAUDRATE_BAUDRATE_Baud1M (0x10000000UL) /*!< 1Mega baud */
            * /

             //   NRF_UART0, UARTE0_UART0_IRQn, PIN_SERIAL_RX, PIN_SERIAL_TX
                NRF_UARTE_Type* nrfUart = NRF_UARTE0;
            IRQn_Type IRQn = UARTE0_UART0_IRQn;
            uc_pinTX = PIN_SERIAL_TX;
            uc_pinRX = PIN_SERIAL_RX;

            nrfUart->PSELTXD = uc_pinTX;
            nrfUart->PSELRXD = uc_pinRX;

            if (uc_hwFlow == 1) {
                nrfUart->PSELCTS = uc_pinCTS;
                nrfUart->PSELRTS = uc_pinRTS;
                nrfUart->CONFIG = (UART_CONFIG_PARITY_Excluded << UART_CONFIG_PARITY_Pos) | UART_CONFIG_HWFC_Enabled;
            }
            else {
                nrfUart->CONFIG = (UART_CONFIG_PARITY_Excluded << UART_CONFIG_PARITY_Pos) | UART_CONFIG_HWFC_Disabled;
            }

            nrfUart->BAUDRATE = nrfBaudRate;

            nrfUart->ENABLE = UART_ENABLE_ENABLE_Enabled;

            nrfUart->EVENTS_RXDRDY = 0x0UL;
            nrfUart->EVENTS_TXDRDY = 0x0UL;

            nrfUart->TASKS_STARTRX = 0x1UL;
            nrfUart->TASKS_STARTTX = 0x1UL;

            nrfUart->INTENSET = UART_INTENSET_RXDRDY_Msk;

            NVIC_ClearPendingIRQ(IRQn);
            NVIC_SetPriority(IRQn, 3);
            NVIC_EnableIRQ(IRQn);
        }

        void end()
        {
            NVIC_DisableIRQ(IRQn);

            nrfUart->INTENCLR = UART_INTENCLR_RXDRDY_Msk;

            nrfUart->TASKS_STOPRX = 0x1UL;
            nrfUart->TASKS_STOPTX = 0x1UL;

            nrfUart->ENABLE = UART_ENABLE_ENABLE_Disabled;

            nrfUart->PSELTXD = 0xFFFFFFFF;
            nrfUart->PSELRXD = 0xFFFFFFFF;

            nrfUart->PSELRTS = 0xFFFFFFFF;
            nrfUart->PSELCTS = 0xFFFFFFFF;

            rxBuffer.clear();

        }

        /*
        
extern "C"
{
  void UARTE0_UART0_IRQHandler()
  {
    SERIAL_PORT_HARDWARE.IrqHandler();
  }
}
void Uart::IrqHandler()
{
  if (nrfUart->EVENTS_RXDRDY)
  {
    nrfUart->EVENTS_RXDRDY = 0x0UL;
    rxBuffer.store_char(nrfUart->RXD);
  }
}
*/

        static Uart& write(uint8_t byte)
        {
            //Wait for previous operation to complete
            //@todo DMA version!
            while (!nrfUart->EVENTS_TXDRDY);
            nrfUart->EVENTS_TXDRDY = 0x0UL;

            nrfUart->TXD = data;
            return *this;
        }

        static Uart& write(const uint8_t* bytes, const uint_fast16_t byteCount )
        {
            for (size_t iByte = 0; iByte < byteCount; ++iByte)
                write(bytes[i]);

            return *this;
        }

        template<size_t byteCount>
        static Uart& write(const uint8_t bytes[byteCount])
        {
            for (const uint8_t byte : bytes) 
                write(byte);

            return *this
        }

        template<typename _T>
        Uart& operator << (const _T& rhs)
        {
            return write(rhs);
        }
    };


} //END: Uart
} //END: BareCpper