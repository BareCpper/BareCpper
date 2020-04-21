#ifndef BARECPPER_SWI_HPP_
#define BARECPPER_SWI_HPP_

#include <cstdint>
#include "Io.hpp"

namespace BareCpper
{
    /** Single-Wire-Interface (SWI) Physical Layer (PHY) driver
    */
    class Swi
    {
    public:

        /** Error codes for physical hardware dependent module
        */
        enum class Status : uint8_t
        {
              Success = ((uint8_t)0x00) ///< Communication with device succeeded.
          //, SWI_FUNCTION_RETCODE_TIMEOUT = ((uint8_t)0xF1)///< Communication timed out.
          //, SWI_FUNCTION_RETCODE_RX_FAIL = ((uint8_t)0xF9)///< Communication failed after at least one byte was received.
            , AddressAckFail = ((uint8_t)0xF2)///< NAK during address.
            , DataAckFail = ((uint8_t)0xF3)///< Communication failed after at least one byte was received.
        };

        /** Single Wire Interface communication structure for the packet
        @note 'packed' attribute ensures that swi_package_t aligns to one-byte boundaries
        */
        struct __attribute__((packed)) Package_t
        {
            uint8_t dev_addr;																//SWI chip address to communicate with.
            uint8_t opcode;																	//op code
            uint8_t mem_addr;																//SWI address/commands to issue to the other chip (node).
            uint8_t mem_addr_length;														//as
            void* buffer;																	//where to find the data.		
            uint16_t wlen;																	//how many bytes do we want to write.
            uint16_t rlen;																	//how many bytes do we want to read.
            uint8_t chk_ack_only_flag;														//this flag tells the low level drive to check for ack only
        };


    public:

#if 0 ///< @todo Standardise IO interfaces
        IoDescriptor io()
        {
            return {
                [](IoDescriptor& descriptor, const uint8_t* const buffer, const uint16_t bufferLength) ->int32_t
                {
                    return 0; //< Read result
                },
                [](IoDescriptor& descriptor, uint8_t* const buffer, const uint16_t bufferLength) ->int32_t
                {
                    return 0; //< Write result
                }
            };
        }
#endif

#if 0 ///< @todo Remove: Missing function form Atmel reference code!?
        void enable(void);
        void set_device_id(uint8_t id);
        void set_signal_pin(uint8_t end);
#endif

        /** Send ACK 
        */
        void sendAck();

        /** Send NACK
        */
        void sendNack(void);
        
        /** Send start and stop condition
        */
        void start_stop_cond(void);

        /** Transmit data bytes
        */
        uint8_t send_bytes(uint8_t count, uint8_t* buffer);
        
        inline uint8_t send_byte(uint8_t value)
        {
            return send_bytes(1, &value);
        }

        /** Receive bytes
        */
        uint8_t receive_bytes(uint8_t count, uint8_t* buffer); 
        
        /** Perform device discovery response
        */
        uint8_t device_discovery(void);

        /** Read operation
        */
        uint8_t read(const Package_t* pkg);

        /** Write operation
        */
        uint8_t write(const Package_t* pkg);

        /** Set device to operate in standard speed mode
        */
        uint8_t write_stdspeed_cmd(const Package_t* packet);

    };

} //END: BareCpper

#if !DOXYGEN ///< Platform specific interface definitions
    #if __SAMD51__
        #include "ATsamd/Swi_SoftDevice.hpp"
    #endif
#endif

#endif