#ifndef BARECPPER_TWIASYNC_HPP_
#  error "Include <BareCpper/TwiAsync.hpp> instead of this file."
#endif

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <algorithm> //< std::find
#include <cstdint>
#include <iterator> //< std::begin, std::end, std::distance
#include "../Common.hpp" //< BareCpper::pollForCondition
#include "Gpio_ATsam5x.hpp" //< BareCpper::pollForCondition

#include "sam.h"
#include "sercom.h"

#if 0 ///ASF4 example

static uint8_t I2C_0_example_str[12] = "Hello World!";

void I2C_0_tx_complete( struct i2c_m_async_desc* const i2c )
{
}

void I2C_0_example( void )
{
    struct io_descriptor* I2C_0_io;

    i2c_m_async_get_io_descriptor( &I2C_0, &I2C_0_io );
    i2c_m_async_enable( &I2C_0 );
    i2c_m_async_register_callback( &I2C_0, I2C_M_ASYNC_TX_COMPLETE, (FUNC_PTR)I2C_0_tx_complete );
    i2c_m_async_set_slaveaddr( &I2C_0, 0x12, I2C_M_SEVEN );
    i2c_m_async_register_callback
    io_write( I2C_0_io, I2C_0_example_str, 12 );
}

#endif

#define TWI_ENABLE 1
#define TWI_BAUD 400000
#define TWI_ADVANCED_CONFIG 0
#define TWI_TRISE 215
#define TWI_MEXTTOEN 0
#define TWI_SEXTTOEN 0
#define TWI_LOWTOUTEN 0
#define TWI_INACTOUT 0x0 //< @todo 0x03 to enable inactive timeout
#define TWI_SDAHOLD 0x2 //< SDA hold time with respect to the negative edge of SCL: 0x2=300-600ns hold time
#define TWI_RUNSTDBY 0
#define TWI_DEBUG_STOP_MODE 0
#define TWI_SPEED 0x00 // Speed: Standard/Fast mode

/**#######################################
 @todo Determine optimal fast and slow clocks at 48MHz!?!?
  From cores/arduino/startup.c:
     GCLK0 = F_CPU
     GCLK1 = 48 MHz
     GCLK2 = 100 MHz
     GCLK3 = XOSC32K
     GCLK4 = 12 MHz
#######################################*/
#ifndef CONF_GCLK_SERCOM2_CORE_SRC
#define TWI_SERCOM_FREQ_REF 48000000ul ///< uses the 48 MHz clock
#define CONF_GCLK_SERCOM2_CORE_SRC GCLK_PCHCTRL_GEN_GCLK1
#endif
#ifndef CONF_GCLK_SERCOM2_SLOW_SRC
#define CONF_GCLK_SERCOM2_SLOW_SRC GCLK_PCHCTRL_GEN_GCLK3 ///< "36.6.3.1 SMBus" GCLK_SERCOM_SLOW clock is used to accurately time the time-out and must be configured to use a 32 KHz oscillator
#endif

#ifndef SERCOM_I2CM_CTRLB_CMD_STOP
#define SERCOM_I2CM_CTRLB_CMD_STOP 0x3  //< @todo Is the CTRLB CMD defined somewhere?
#endif
#ifndef SERCOM_I2CM_STATUS_BUSSTATE_IDLE
#define SERCOM_I2CM_STATUS_BUSSTATE_IDLE 0x1  //< @todo Is the CTRLB BUSSTATE defined somewhere?
#endif

#ifndef SERCOM_I2CM_CTRLA_MODE_I2C_MASTER
#define SERCOM_I2CM_CTRLA_MODE_I2C_MASTER SERCOM_I2CM_CTRLA_MODE(0x5) //< @todo Is the CTRLA I2C MASTER  defined somewhere?
#endif


namespace BareCpper
{

    /** Two-Wire-Interface (TWI) Physical Layer (PHY) driver aka I2C
       */
    class TwiAsync : public BareCpper::IoDescriptor
    {
    public:

        /** Error codes for physical hardware dependent module
        */
        enum class Status : int8_t
        {
            Success = 0 ///< Communication with device succeeded.
            , Ack = -1 ///< Received ACK from device on I2C bus 
            , NAck = -2 ///< Received NACK from device on I2C bus
            , ArbitrationLost = -3 ///< Arbitration lost
            , BadAddress = -4 ///< Bad address
            , BusError = -5 ///< Bus error
            , DeviceBusy = -6 ///< Device busy
            , PackageCollision = -7 ///< Package collision
        };

        /**
         * \brief The I2C master callback function type for completion of RX or TX
         */
        typedef void (*FnComplete_t)(TwiAsync& twi);

        /**
         * \brief The I2C master callback function type for error
         */
        typedef void (*FnError_t)( TwiAsync& twi, const Status errorCode );

        struct Message
        {
            union Flags
            {
                struct
                {
                    ///@{ Setting
                    uint8_t READ : 1; ///< Read data, from slave to master
                    uint8_t STOP : 1; ///< Stop bus after transmission
                    uint8_t SILENT : 1; ///< Stop Callback being triggered on transfer completion e.g. On writting address register
                    ///@}
                    ///@{ State
                    uint8_t BUSY : 1; ///< Message is busy being processed
                    uint8_t FAIL : 1; ///< Message processing failed
                    ///@}
                } bit; ///< Structure used for bit access
                uint8_t reg;
            };

            uint8_t address; //< @todo Support 10bit address?
            Flags flags;
            int16_t bufferLength;
            uint8_t* buffer;
        };

        /** Async Service state
        * @note We only handle one message transfer at a time
        * @todo Add a message queue?
        */
        struct Service
        {
            Message message;
           // uint16_t mode;
           // uint16_t trise;
        };

    public:

        /// @todo Use GPIO as pins b=must currently be on the same Port!
        // iPinScl == PA13
        // iPinSda == PA12
        TwiAsync( const PinId pinScl, const PinId pinSda )
            : IoDescriptor{
              []( IoDescriptor& descriptor, uint8_t* const buffer, const uint16_t bufferLength ) ->int32_t
                {
                    TwiAsync& twi = reinterpret_cast<TwiAsync&>(descriptor);                    
                    return twi.readAsync( buffer, bufferLength );
                }
            , []( IoDescriptor& descriptor, const uint8_t* const buffer, const uint16_t bufferLength ) ->int32_t
                {
                    TwiAsync& twi = reinterpret_cast<TwiAsync&>(descriptor);
                    return twi.writeAsync( buffer, bufferLength );
                } }
            , hw_()
            , pinScl_( pinScl )
            , pinSda_( pinSda )
            , fnError_()
            , fnTxComplete_()
            , fnRxComplete_()
            , slaveAddress_()
            , service_()
        {
        };

        bool initialise()
        {
            const auto sercomIndex = ATsamd5x::sercomForPins({ pinScl_, pinSda_ });
            if (!sercomIndex)
                return false;

            ::Sercom* sercom = ATsamd5x::sercom(*sercomIndex);
            if (!sercom)
                return false;

            hw_ = &sercom->I2CM;

            return initialiseClock()
                && initialiseDevice()
                && initialiseAsync()
                && initialiseGpio();
        }

        /** Retrieve ordinal number of the given sercom hardware instance
        * @return Index to Sercom instance, or SERCOM_INST_NUM if Sercom is not valid
        */
        uint8_t hardwareIndex() const
        {
            Sercom* const sercomHwModules[] = SERCOM_INSTS;
            return std::distance( std::begin(sercomHwModules)
                                , std::find( std::begin(sercomHwModules), std::end( sercomHwModules ), reinterpret_cast<Sercom* const>(hw_) ) );
        }

        void setSlaveAddress( const uint16_t address )
        {
            slaveAddress_ = address;
        }

        void setRxCompleteCallback( const FnComplete_t fnRxComplete )
        {
            fnRxComplete_ = fnRxComplete;
        }
        void setTxCompleteCallback( const FnComplete_t fnTxComplete )
        {
            fnTxComplete_ = fnTxComplete;
        }
        void setErrorCallback( const FnError_t fnError )
        {
            fnError_ = fnError;
        }

        bool enable()
        {
            // SERCOM_CRITICAL_SECTION_ENTER();
            hw_->CTRLA.bit.ENABLE = true;
            while( hw_->SYNCBUSY.bit.SWRST | hw_->SYNCBUSY.bit.ENABLE );
            // SERCOM_CRITICAL_SECTION_LEAVE();
            
            hw_->STATUS.bit.BUSSTATE = SERCOM_I2CM_STATUS_BUSSTATE_IDLE;
            while( hw_->SYNCBUSY.bit.SYSOP ); //< Wait for SYSOP

            auto busIsIdle = [hw = hw_]() -> bool { return (hw->STATUS.bit.BUSSTATE == SERCOM_I2CM_STATUS_BUSSTATE_IDLE);  };
            if( !busIsIdle() )
            {
#if 0 //< Can we reset the BNO055 via the bus?
                stop();
#endif

                /// @todo BNO055 incomplete stops this so Timeouts per _i2c_m_enable_implementation? 
                bool okBusIdle = pollForCondition( busIsIdle, 100, CycleCounter(), CycleCounter::nsToTicks( 1000 ) );
                if( !okBusIdle )
                {
                    return false;
                }
            }

            return enableAsync();
        }

        int32_t readAsync( uint8_t* const buffer, const uint16_t bufferLength, const bool doStop = true )
        {
            Message message = {};
            message.address = slaveAddress_;
            message.flags.bit.READ = true;
            message.flags.bit.STOP = doStop;
            message.bufferLength = bufferLength;
            message.buffer = buffer;
            const Status transferStatus = asyncTransfer( message );

            return (transferStatus == Status::Success) ? static_cast<int32_t>(bufferLength) : static_cast<int32_t>(transferStatus);
        }

        int32_t writeAsync( const uint8_t* const buffer, const uint16_t bufferLength, const bool doStop = true )
        {
            Message message = {};
            message.address = slaveAddress_;
            message.flags.bit.STOP = doStop;
            message.bufferLength = bufferLength;
            message.buffer = const_cast<uint8_t*>(buffer); ///< @todo Use union for read/write message types to remove cast
            const Status transferStatus = asyncTransfer( message );

            return (transferStatus == Status::Success) ? static_cast<int32_t>(bufferLength) : static_cast<int32_t>(transferStatus);
        }

        /* Release/Stop the I2C bus
        */
        void stop()
        {
            while( hw_->SYNCBUSY.bit.SYSOP ); //< Wait for SYSOP

            //SERCOM_CRITICAL_SECTION_ENTER();
            hw_->CTRLB.bit.CMD = SERCOM_I2CM_CTRLB_CMD_STOP;
            while( hw_->SYNCBUSY.bit.SYSOP ); //< Wait for SYSOP
            //SERCOM_CRITICAL_SECTION_LEAVE();
        }

    private:

        /** Enables interrupts for async mode
        */
        bool enableAsync()
        {
            hw_->INTENSET.bit.SB = true;
            hw_->INTENSET.bit.MB = true;
            hw_->INTENSET.bit.ERROR = true;
            return true;
        }

        bool initialiseClock()
        {
            GCLK->PCHCTRL[SERCOM2_GCLK_ID_CORE].bit.CHEN = 0;     // Disable timer
            GCLK->PCHCTRL[SERCOM2_GCLK_ID_SLOW].bit.CHEN = 0;     // Disable timer

            while( GCLK->PCHCTRL[SERCOM2_GCLK_ID_SLOW].bit.CHEN
                   || GCLK->PCHCTRL[SERCOM2_GCLK_ID_CORE].bit.CHEN );  // Wait for disable

           // GCLK_CRITICAL_SECTION_ENTER();
            GCLK->PCHCTRL[SERCOM2_GCLK_ID_CORE].reg = CONF_GCLK_SERCOM2_CORE_SRC | GCLK_PCHCTRL_CHEN;
            GCLK->PCHCTRL[SERCOM2_GCLK_ID_SLOW].reg = CONF_GCLK_SERCOM2_SLOW_SRC | GCLK_PCHCTRL_CHEN;
           // GCLK_CRITICAL_SECTION_LEAVE();

            //MCLK_CRITICAL_SECTION_ENTER();
            MCLK->APBBMASK.reg |= MCLK_APBBMASK_SERCOM2;
            //MCLK_CRITICAL_SECTION_LEAVE();

            while( !GCLK->PCHCTRL[SERCOM2_GCLK_ID_SLOW].bit.CHEN
                   && !GCLK->PCHCTRL[SERCOM2_GCLK_ID_CORE].bit.CHEN ); // Wait for clock enable

            return true;
        }

        bool initialiseDevice()
        {
            if( !hw_->SYNCBUSY.bit.SWRST )
            {                
                if( hw_->CTRLA.bit.ENABLE ) // Disable
                {
                    hw_->CTRLA.bit.ENABLE = false;
                    while( hw_->SYNCBUSY.bit.ENABLE );
                }
                
                hw_->CTRLA.reg = SERCOM_I2CM_CTRLA_SWRST | SERCOM_I2CM_CTRLA_MODE_I2C_MASTER; // Master reset
            }

            while( hw_->SYNCBUSY.bit.SWRST ); // Wait for reset

            /**                 gclk_freq - (i2c_scl_freq * 10) - (gclk_freq * i2c_scl_freq * Trise)
             * BAUD + BAUDLOW = --------------------------------------------------------------------
             *                                                i2c_scl_freq
             * @note BAUDLOW only used for odd BAUD + BAUDLOW
             */
#if 0
            const uint8_t baudBit = TWI_SERCOM_FREQ_REF / (2 * TWI_BAUD) - 1;
#else
            const uint32_t clkrate = TWI_SERCOM_FREQ_REF;
            const uint32_t baudrate = TWI_BAUD;
            const uint32_t trise = TWI_TRISE;
            const uint8_t baudBit = static_cast<uint32_t>((clkrate - 10 * baudrate - baudrate * clkrate * (trise * 0.000000001)) / (2 * baudrate));
#endif

            //SERCOM_CRITICAL_SECTION_ENTER();
            hw_->CTRLA.reg = SERCOM_I2CM_CTRLA_MODE_I2C_MASTER
                | (TWI_RUNSTDBY ? SERCOM_I2CM_CTRLA_RUNSTDBY : 0)
                | SERCOM_I2CM_CTRLA_SPEED( TWI_SPEED )
                | (TWI_MEXTTOEN ? SERCOM_I2CM_CTRLA_MEXTTOEN : 0)
                | (TWI_SEXTTOEN ? SERCOM_I2CM_CTRLA_SEXTTOEN : 0)
                | SERCOM_I2CM_CTRLA_INACTOUT( TWI_INACTOUT )
                | (TWI_LOWTOUTEN ? SERCOM_I2CM_CTRLA_LOWTOUTEN : 0)
                | SERCOM_I2CM_CTRLA_SDAHOLD( TWI_SDAHOLD );
            hw_->CTRLB.reg = SERCOM_I2CM_CTRLB_SMEN;
            hw_->DBGCTRL.bit.DBGSTOP = true;
            hw_->BAUD.bit.BAUD = baudBit;
            //SERCOM_CRITICAL_SECTION_LEAVE();

#if 0 /// @todo Only supports standard/fast modes
            service->mode = (_i2cms[i].ctrl_a & SERCOM_I2CM_CTRLA_SPEED_Msk) >> SERCOM_I2CM_CTRLA_SPEED_Pos;
            hri_sercomi2cm_write_ADDR_HS_bit( hw, service->mode < I2C_HS ? 0 : 1 );
#endif

            //service->trise = _i2cms[i].trise;
            return true;
        }

        bool initialiseAsync()
        {
            const uint8_t iHardware = hardwareIndex();

            /// Register to service interupts SERCOM2_0_Handler etc..
            sercom_s[iHardware] = this; /// _sercom_init_irq_param

            const IRQn_Type beginIrq = static_cast<IRQn_Type>(SERCOM0_0_IRQn + (iHardware * 4U));
            const IRQn_Type endIrq = static_cast<IRQn_Type>(beginIrq + 4);
            for( IRQn_Type irq = beginIrq; irq < endIrq; irq = static_cast<IRQn_Type>(irq+1) )
            {
                NVIC_DisableIRQ( irq );
                NVIC_ClearPendingIRQ( irq );
                NVIC_EnableIRQ( irq );
            }

            return true;
        }

        bool initialiseGpio()
        {
            //PORT_CRITICAL_SECTION_ENTER();
            using ::Port; //< Disambiguate Sam.h vs BareCpper::Gpio
            PORT->Group[pinSda_.port].PINCFG[pinSda_.pin].bit.PULLEN = false;//< SDA, GPIO_PULL_OFF
            PORT->Group[pinScl_.port].PINCFG[pinScl_.pin].bit.PULLEN = false;//< SCL, GPIO_PULL_OFF
            //PORT_CRITICAL_SECTION_LEAVE();

            ATsamd5x::setPinFunction( pinSda_, MUX_PA12C_SERCOM2_PAD0 );
            ATsamd5x::setPinFunction( pinScl_, MUX_PA13C_SERCOM2_PAD1 );

            return true;
        }

        Status asyncTransfer( const Message& message )
        {
            if( message.bufferLength == 0 )
            {
                return Status::Success;
            }

            if( service_.message.flags.bit.BUSY )
            {
                return Status::DeviceBusy;
            }

            //Make copy of the message
            service_.message = message;
            service_.message.flags.bit.BUSY = true;
            
            //SERCOM_CRITICAL_SECTION_ENTER();
            hw_->CTRLB.bit.SMEN = true; //< Enable smart mode
            while( hw_->SYNCBUSY.bit.SYSOP ); //< Wait for SYSOP
            //SERCOM_CRITICAL_SECTION_LEAVE();

            Status returnStatus = sendAddress();

            if( returnStatus != Status::Success )
            {
                service_.message.flags.bit.BUSY = false;
            }

            return returnStatus;
        }

        Status sendAddress()
        {
            //SERCOM_CRITICAL_SECTION_ENTER();
            hw_->CTRLB.bit.ACKACT = (service_.message.bufferLength == 1 && hw_->CTRLA.bit.SCLSM); //< @note SCLSM is Clock-stretching
            while( hw_->SYNCBUSY.bit.SYSOP ); //< Wait for SYSOP

            /// @todo Can we do the ACKACT + ADDR in a single wait-operation?

            hw_->ADDR.bit.ADDR = (service_.message.address << 1) | service_.message.flags.bit.READ; //< 7-bits address + 1-bits R/W @todo Support ten bit addresses!
            while( hw_->SYNCBUSY.bit.SYSOP ); //< Wait for SYSOP
            // SERCOM_CRITICAL_SECTION_LEAVE();

            return Status::Success;
        }

        Status onInterruptMasterOnBus()
        {
            while( hw_->SYNCBUSY.bit.SYSOP ); //< Wait for SYSOP

            if( hw_->STATUS.bit.ARBLOST ) // tx error
            {
                service_.message.flags.bit.FAIL = true;
                service_.message.flags.bit.BUSY = false;
                hw_->INTFLAG.reg = SERCOM_I2CM_INTFLAG_MB; ///< Clear flag
                return hw_->STATUS.bit.BUSERR ? Status::BusError : Status::BadAddress;
            }

            if( hw_->STATUS.bit.RXNACK )
            {
                // Slave rejects to receive more data
                if( service_.message.bufferLength > 0 )
                {
                    service_.message.flags.bit.FAIL = true;
                }

                if( service_.message.flags.bit.STOP )
                {
                    stop();
                }
                else // @todo Not in Atmel Start, Necessary?
                {
                    hw_->INTFLAG.reg = SERCOM_I2CM_INTFLAG_MB; ///< Clear flag
                }

                service_.message.flags.bit.BUSY = false;
                return Status::NAck;
            }

#if 0 //< @todo Support 10bit address and use Should use the 32-bit mode to simplify transmit for performance etc!
            if( service_.message.flags & I2C_M_TEN )
            {
                hw_->ADDR.reg = ((((service_.message.address & TEN_ADDR_MASK) >> 8) | TEN_ADDR_FRAME) << 1) | I2C_M_RD
                                                | (hri_sercomi2cm_read_ADDR_reg( hw ) & SERCOM_I2CM_ADDR_HS) );
                service_.message.flags &= ~I2C_M_TEN;
                return Status::Success;
            }
#endif

            if( service_.message.bufferLength == 0 )
            {
                if( service_.message.flags.bit.STOP )
                {
                    stop();
                }
                else // @todo Not in Atmel Start, Necessary?
                {
                    hw_->INTFLAG.reg = SERCOM_I2CM_INTFLAG_MB; ///< Clear flag
                }

                service_.message.flags.bit.BUSY = false;
            }
            else
            {
                hw_->DATA.reg = *service_.message.buffer; ///< Write byte
                while( hw_->SYNCBUSY.bit.SYSOP ); //< Wait for SYSOP
                service_.message.buffer++;
                service_.message.bufferLength--;
            }

            return Status::Success;
        }

        Status onInterruptSlaveOnBus()
        {
            while( hw_->SYNCBUSY.bit.SYSOP ); //< Wait for SYSOP

            if( !(service_.message.bufferLength && !hw_->STATUS.bit.RXNACK) )
            {
                return Status::NAck;
            }

            --service_.message.bufferLength;

            // last byte, send nack @note SCLSM is Clock-stretching
            if( (service_.message.bufferLength == 0 && !hw_->CTRLA.bit.SCLSM)
                || (service_.message.bufferLength == 1 && hw_->CTRLA.bit.SCLSM) )
            {
                //SERCOM_CRITICAL_SECTION_ENTER();
                hw_->CTRLB.bit.ACKACT = true;
                while( hw_->SYNCBUSY.bit.SYSOP ); //< Wait for SYSOP
                //SERCOM_CRITICAL_SECTION_LEAVE();
            }

            if( service_.message.bufferLength == 0 )
            {
                if( service_.message.flags.bit.STOP )
                {
                    //SERCOM_CRITICAL_SECTION_ENTER();
                    hw_->CTRLB.bit.SMEN = false;
                    while( hw_->SYNCBUSY.bit.SYSOP ); //< Wait for SYSOP
                    //SERCOM_CRITICAL_SECTION_LEAVE();

                    /// @todo Do we need to do SMEN + CMD operations separately or not?
                    stop();
                }

                service_.message.flags.bit.BUSY = false;
            }

            // @note Accessing DATA.DATA auto-triggers I2C bus operations The operation performed depends on the state of CTRLB.ACKACT, CTRLB.SMEN
            // Send last byte
            while( hw_->SYNCBUSY.bit.SYSOP ); //< Wait for SYSOP
            *service_.message.buffer++ = hw_->DATA.reg;
            return Status::Success;
        }

        void onServiceError( const Status status )
        {
            service_.message.flags.bit.BUSY = false;
            if( fnError_ ) 
            {
                fnError_( *this, status );
            }
        }

        /** @internal Sercom i2c master interrupt handler
         */
        void onService()
        {
            if( hw_->INTFLAG.bit.ERROR )
            {
                onServiceError( Status::BusError );
                hw_->INTFLAG.reg = SERCOM_I2CM_INTFLAG_ERROR; //< Clear flag
                return;
            }

            FnComplete_t fnComplete = nullptr;
            Status retVal = Status::Success;

            if( hw_->INTFLAG.bit.MB ) //< Master On Bus Interrupt
            {
                retVal = onInterruptMasterOnBus();
                fnComplete = fnTxComplete_;

            }
            else
            if( hw_->INTFLAG.bit.SB ) //< Slave On Bus Interrupt 
            {
                retVal = onInterruptSlaveOnBus();
                hw_->INTFLAG.reg = SERCOM_I2CM_INTFLAG_SB;  ///< Clear flag
                fnComplete = fnRxComplete_;
            }

            if(retVal != Status::Success)
            {
                onServiceError( retVal );
                return;
            }

            if( fnComplete
                && !service_.message.flags.bit.SILENT
                && !service_.message.flags.bit.BUSY )
            {
                fnComplete( *this );
            }
        }

        static TwiAsync* sercom_s[SERCOM_INST_NUM];

    public:
        static void SERCOM2_Handler( void )
        {
            if( sercom_s[2] )
            {
                sercom_s[2]->onService(); /// @todo Negative cost embedded!
            }
        }

    private:

        SercomI2cm* hw_;
        const PinId pinScl_;
        const PinId pinSda_;

        FnError_t fnError_;
        FnComplete_t fnTxComplete_;
        FnComplete_t fnRxComplete_;

        uint16_t slaveAddress_;

        Service service_; ///< Service state
    };

    TwiAsync* TwiAsync::sercom_s[SERCOM_INST_NUM] = {};
} //END: BareCpper


extern "C" void SERCOM2_0_Handler( void )
{
    BareCpper::TwiAsync::SERCOM2_Handler();
}
extern "C" void SERCOM2_1_Handler( void )
{
    BareCpper::TwiAsync::SERCOM2_Handler();
}
extern "C" void SERCOM2_2_Handler( void )
{
    BareCpper::TwiAsync::SERCOM2_Handler();
}
extern "C" void SERCOM2_3_Handler( void )
{
    BareCpper::TwiAsync::SERCOM2_Handler();
}