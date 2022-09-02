#ifndef DMAC_SAMD51_HPP
#define DMAC_SAMD51_HPP

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <functional>

#include <sam.h>

namespace BareCpper
{
  namespace SAMD51
  {
    static constexpr size_t DmacNumChannels = DMAC_CH_NUM;
    static constexpr uint8_t DmacInvalidChannel = 255;
    /**
     * @brief Dmac driver for SAMD51
     * Example usage:
     * 1. Get the DMAC instance
     * 2. Initialize and enable the DMAC
     * 3. Configure channel and its default transfer descriptor
     *  - Set beat size (byte, halfword or word transfer in each bus access)
     *  - Set the action to take after block is transferred
     *  - Set start address for source of the transfer and the increment
     *    for the source address
     *  - Set start address for destination of the transfer and the increment
     *    for the desination address
     *  - Set the number of beats to transfer (number of bytes, halfwords or words,
     *    depending on beat size)
     *  - Set channel priority
     *  - Set the trigger source and the action to take when the trigger is detected
     * 4. (Optional) Configure channel interrupts and set callback functions
     * 5. Enable the channel
     */
    class DmaController
    {
      public:
        /**
         * @brief Enum for size of one data
         * transfer bus access
         */
        enum class BeatSize : uint8_t
        {
          Byte = 0x0, // 8-bit
          Hword = 0x1, // 16-bit
          Word = 0x2 //32-bit
        };

        /**
         * @brief Enum for holding possible actions
         * to take when block transfer has completed
         * 
         */
        enum class BlockAction : uint8_t
        {
          NoAction = 0x0, // disable the channel
          Interrupt = 0x1, // fire interrupt after block transfer is completed
          Suspend = 0x2, // suspend the channel after block transfer is completed
          SuspendAndInterrupt = 0x3 // suspend the channel and fire interrupt after block transfer is completed
        };

        /**
         * @brief Enum for different increment steps
         * supported for source and destination
         * addresses of DMA transfers.
         * @note: 2 to the power of this number will be multiplied with
         * beat size to determine address increment
         */
        enum class AddressIncrementSize : uint8_t
        {
          Disabled = 255,  // special case, address is incrementead only by beat size
          x1 = 0,
          x2 = 1,
          x4 = 2,
          x8 = 3,
          x16 = 4,
          x32 = 5,
          x64 = 6,
          x128 = 7
        };

        /**
         * @brief Enum for selecting when to generate event
         * for a given descriptor
         * 
         */
        enum class EventOutput : uint8_t
        {
          Disable = 0x0, // don't generate event
          Block = 0x1, // generate event when block transfer is completed
          Beat = 0x3 // generate event after beat transfer
        };

        /**
         * @brief Enum for different priority levels
         * that can be enabled/disabled for handling in
         * the DMA and set to a DMA channel
         */
        enum class Priority : uint8_t
        {
          Level0 = 0x0,
          Level1 = 0x1,
          Level2 = 0x2,
          Level3 = 0x3,
          AllLevels = 0xF
        };

        /**
         * @brief Enum for different trigger sources
         * that can fire DMA transfer
         */
        enum class TriggerSource : uint8_t
        {
          SoftwareOrEvent = 0x00,
          Rtc             = 0x01,
          Dsu0            = 0x02,
          Dsu1            = 0x03,
          Sercom0Rx       = 0x04,
          Sercom0Tx       = 0x05,
          Sercom1Rx       = 0x06,
          Sercom1Tx       = 0x07,
          Sercom2Rx       = 0x08,
          Sercom2Tx       = 0x09,
          Sercom3Rx       = 0x0A,
          Sercom3Tx       = 0x0B,
          Sercom4Rx       = 0x0C,
          Sercom4Tx       = 0x0D,
          Sercom5Rx       = 0x0E,
          Sercom5Tx       = 0x0F,
          Sercom6Rx       = 0x10,
          Sercom6Tx       = 0x11,
          Sercom7Rx       = 0x12,
          Sercom7Tx       = 0x13,
          Can0            = 0x14,
          Can1            = 0x15,
          Tcc0Ovf         = 0x16,
          Tcc0MC0         = 0x17,
          Tcc0MC1         = 0x18,
          Tcc0MC2         = 0x19,
          Tcc0MC3         = 0x1A,
          Tcc0MC4         = 0x1B,
          Tcc0MC5         = 0x1C,
          Tcc1Ovf         = 0x1D,
          Tcc1MC0         = 0x1E,
          Tcc1MC1         = 0x1F,
          Tcc1MC2         = 0x20,
          Tcc1MC3         = 0x21,
          Tcc2Ovf         = 0x22,
          Tcc2MC0         = 0x23,
          Tcc2MC1         = 0x24,
          Tcc2MC2         = 0x25,
          Tcc3Ovf         = 0x26,
          Tcc3MC0         = 0x27,
          Tcc3MC1         = 0x28,
          Tcc4Ovf         = 0x29,
          Tcc4MC0         = 0x2A,
          Tcc4MC1         = 0x2B,
          Tc0Ovf          = 0x2C,
          Tc0MC0          = 0x2D,
          Tc0MC1          = 0x2E,
          Tc1Ovf          = 0x2F,
          Tc1MC0          = 0x30,
          Tc1MC1          = 0x31,
          Tc2Ovf          = 0x32,
          Tc2MC0          = 0x33,
          Tc2MC1          = 0x34,
          Tc3Ovf          = 0x35,
          Tc3MC0          = 0x36,
          Tc3MC1          = 0x37,
          Tc4Ovf          = 0x38,
          Tc4MC0          = 0x39,
          Tc4MC1          = 0x3A,
          Tc5Ovf          = 0x3B,
          Tc5MC0          = 0x3C,
          Tc5MC1          = 0x3D,
          Tc6Ovf          = 0x3E,
          Tc6MC0          = 0x3F,
          Tc6MC1          = 0x40,
          Tc7Ovf          = 0x41,
          Tc7MC0          = 0x42,
          Tc7MC1          = 0x43,
          Adc0Resrdy      = 0x44,
          Adc0Seq         = 0x45,
          Adc1Resrdy      = 0x46,
          Adc1Seq         = 0x47,
          DacEmpty0       = 0x48,
          DacEmpty1       = 0x49,
          DacResrdy0      = 0x4A,
          DacResrdy1      = 0x4B,
          I2SRx0          = 0x4C,
          I2STx0          = 0x4D,
          I2SRx1          = 0x4E,
          I2STx1          = 0x4F,
          PccRx           = 0x50,
          AesWr           = 0x51,
          AesRd           = 0x52,
          QspiRx          = 0x53,
          QspiTx          = 0x54
        };

        /**
         * @brief Enum for possible actions 
         * applying during DMA transfer
         * 
         */
        enum class TriggerAction : uint8_t
        {
          Block = 0x0, // one trigger required for each block transfer
          Burst = 0x2, // one trigger for burst transfer
          Transaction = 0x3 // one trigger for whole transaction
        };

        /**
         * @brief Enum for possible levels
         * of quality of service for a given
         * priority level
         * 
         */
        enum class QualityOfService : uint8_t
        {
          Disable = 0x0, // no sensitive operation
          Low = 0x1, // sensitive to bandwidth
          Medium = 0x2, // sensitive to latency
          Critical = 0x3 // critical latency
        };

        /**
         * @brief Enum for possible event
         * output modes
         * 
         */
        enum class EventOutputMode : uint8_t
        {
          Default = 0x0, // block event output selection (selected in descriptor)
          TrigAct = 0x1 // ongoing trigger action
        };

        /**
         * @brief Select which action to take
         * when event is recieved
         * 
         */
        enum class EventInputAction : uint8_t
        {
          NoAction = 0x0,
          Trigger = 0x1,
          ConditionalTrigger = 0x2,
          ConditionalBlockTransfer = 0x3,
          Suspend = 0x4,
          Resume = 0x5,
          SkipNextSuspend = 0x6,
          IncreasePriority = 0x7
        };

      public:
        // remove copy and move constructors and assignment operators
        // for singleton class
        DmaController(const DmaController &) = delete;
        DmaController(DmaController &&) = delete;
        DmaController& operator=(const DmaController &) = delete;
        DmaController&& operator=(DmaController &&) = delete;
        /**
         * @brief Get reference to the DmaController instance
         * 
         * @return DmaController& Reference to the DmaController instance
         */
        static DmaController &getInstance();
        
        /***** Basic DMAC control *****/
        
        /**
         * @brief Initialize the DMA controller
         * for operation
         * @param priority Which transfer priority levels should be handled by the DMA
         */
        void init(const Priority &priority = Priority::AllLevels);

        /**
         * @brief Reset the registers of the DMA controller
         * 
         */
        void reset();

        /**
         * @brief Enable the DMAC
         * 
         */
        void enable();

        /**
         * @brief Disable the DMAC, aborting any ongoing transfers
         * 
         */
        void disable();

        /**
         * @brief Change which priority levels will be handled
         * by the DMAC
         * @param priority The priority level
         * @param enable true if the priority level should be handled, false if not
         */
        void changePriorityLevel(const Priority &priority, bool enable);

        /**
         * @brief Change the scheduling scheme for a given
         * priority level, including the 
         * @param priority The priority level
         * @param qos Level of quality of service
         * @param roundRobin If true, use round-robin arbitration scheme.
         * If false, use static arbitration scheme.
         * Refer to datasheet, section 22.6.2.4 Arbitration, p.346
         */
        void changePriorityLevelScheduling(const Priority &priority, const QualityOfService &qos, bool roundRobin);

        /***** Descriptors related *****/
        // This functions can be moved into separate Descriptor class, they are all static now

        /**
         * @brief Set the source address of the transfer for the
         * specified transfer descriptor
         * 
         * @param descriptor Reference to DMAC descriptor
         * @param addr The source address
         */
        static void setSourceAddress(DmacDescriptor &descriptor, const std::uintptr_t addr);

        /**
         * @brief Set the destination address of the transfer for the
         * specified transfer descriptor
         * 
         * @param descriptor Reference to DMAC descriptor
         * @param addr The destination address
         */
        static void setDestinationAddress(DmacDescriptor &descriptor, const std::uintptr_t addr);

        /**
         * @brief Set the size of one bus transfer for the specified transfer descriptor, 
         * called beat, i.e. 8-bits, 16-bits or 32-bits in one bus access
         * 
         * @param descriptor Reference to DMAC descriptor
         * @param beatSize The size of one bus transfer
         */
        static void setBeatSize(DmacDescriptor &descriptor, const BeatSize &beatSize);

        /**
         * @brief Set the size of the transfer in number
         * of beats
         * 
         * @param descriptor Reference to DMAC descriptor
         * @param size The number of beats for transfer
         */
        static void setTransferSize(DmacDescriptor &descriptor, const uint16_t size);

        /**
         * @brief Change the source address increment, to allow
         * incrementing of the source address after each beat transfer,
         * or it is also possible to disable the increment
         * 
         * @param descriptor Reference to DMAC descriptor
         * @param stepSize The step size
         * If StepSize::Disabled, the address is not incremented
         * If StepSize::x1, the address is incremented by 1
         * Else, the address increment is flexible, but
         * destination address increment CAN'T BE USED
         */
        static void setSourceAddressIncrement(DmacDescriptor &descriptor, const AddressIncrementSize &stepSize = AddressIncrementSize::x1);

        /**
         * @brief Change the destination address increment, to allow
         * incrementing of the destination address after each beat transfer,
         * or it is also possible to disable the increment
         * 
         * @param descriptor Reference to DMAC descriptor
         * @param stepSize The step size
         * If StepSize::Disabled, the address is not incremented
         * If StepSize::x1, the address is incremented by 1
         * Else, the address increment is flexible, but
         * source address increment CAN'T BE USED
         */
        static void setDestinationAddressIncrement(DmacDescriptor &descriptor, const AddressIncrementSize &stepSize = AddressIncrementSize::x1);

        /**
         * @brief Set which action to take when block transfer is completed
         * 
         * @param descriptor Reference to DMAC descriptor
         * @param blockAction The action after block transfer is completed
         */
        static void setBlockAction(DmacDescriptor &descriptor, const BlockAction &blockAction);

        /**
         * @brief Select which action generates event
         * 
         * @param descriptor Reference to DMAC descriptor
         * @param eventSelection The action which generates event
         */
        static void setEventOutputSelection(DmacDescriptor &descriptor, const EventOutput &eventSelection);

        // @todo: Allow creation of additional descriptors
        /**
         * @brief Setup the specified DMAC transfer descriptor.
         * 
         * @param descriptor The DMAC transfer descriptor
         * @param beatSize The size of one bus access (i.e. byte, half word or word)
         * @param blockAction What to do after block transfer is completed
         * @param sourceAddress The start address for the source of the transfer
         * @param sourceIncrement The increment for the source address
         * @param destinationAddress The start address for the destination of the transfer
         * @param destinationIncrement The increment for the destination address
         * @param transferSize The number of beats to transfer
         */
        static void setupDescriptor(DmacDescriptor &descriptor, const BeatSize &beatSize, const BlockAction &blockAction,
                          std::uintptr_t sourceAddress, const AddressIncrementSize &sourceIncrement,
                          std::uintptr_t destinationAddress, const AddressIncrementSize &destinationIncrement,
                          const uint16_t transferSize);

        /**
         * @brief Link the second descriptor to the first descriptor.
         * Can be used to link the descriptor to itself, making the transfer circular.
         * 
         * @param first Reference to the first descriptor
         * @param second Reference to the second descriptor
         */
        static void linkDescriptors(DmacDescriptor *first, const DmacDescriptor *second);
        
        /**
         * @brief Link the specified descriptor address to the default descriptor
         * for the specified channel
         * 
         * @param chNum The DMAC channel number
         * @param secondDescriptorAddress The address of the descriptor to be linked with 
         * the channel descriptor 
         */
        static void linkDescriptors(uint8_t chNum, const std::uintptr_t secondDescriptorAddress);

        /**
         * @brief Link the descriptor to the default descriptor
         * for the specified channel
         * 
         * @param chNum The DMAC channel number
         * @param descriptor The descriptor to be linked with the channel descriptor 
         */
        static void linkDescriptors(uint8_t chNum, const DmacDescriptor *descriptor);
        
        /**
         * @brief Get the address of the default descriptor linked to
         * the specified DMAC channel.
         * Useful for linking descriptors.
         * 
         * @param chNum The DMAC channel number
         * @return std::uintptr_t The address of the descriptor, as unsigned integer
         */
        static std::uintptr_t getDescriptorAddress(uint8_t chNum);

        static DmacDescriptor& getDescriptor(uint8_t chNum)
        {
          return descriptorSection[chNum];
        }

        // same functions as above, for default descriptors
        static void setSourceAddress(uint8_t chNum, const std::uintptr_t addr)
        {
          if(chNum > DmacNumChannels) return;
          setSourceAddress(descriptorSection[chNum], addr);
        }
        static void setDestinationAddress(uint8_t chNum, const std::uintptr_t addr)
        {
          if(chNum > DmacNumChannels) return;
          setDestinationAddress(descriptorSection[chNum], addr);
        }
        static void setBeatSize(uint8_t chNum, const BeatSize &beatSize)
        {
          if(chNum > DmacNumChannels) return;
          setBeatSize(descriptorSection[chNum], beatSize);
        }
        static void setTransferSize(uint8_t chNum, const uint16_t size)
        {
          if(chNum > DmacNumChannels) return;
          setTransferSize(descriptorSection[chNum], size);
        }
        static void setSourceAddressIncrement(uint8_t chNum, const AddressIncrementSize &stepSize = AddressIncrementSize::x1)
        {
          if(chNum > DmacNumChannels) return;
          setSourceAddressIncrement(descriptorSection[chNum], stepSize);
        }
        static void setDestinationAddressIncrement(uint8_t chNum, const AddressIncrementSize &stepSize = AddressIncrementSize::x1)
        {
          if(chNum > DmacNumChannels) return;
          setDestinationAddressIncrement(descriptorSection[chNum], stepSize);
        }
        static void setBlockAction(uint8_t chNum, const BlockAction &blockAction)
        {
          if(chNum > DmacNumChannels) return;
          setBlockAction(descriptorSection[chNum], blockAction);
        }
        static void setEventOutputSelection(uint8_t chNum, const EventOutput &eventSelection)
        {
          if(chNum > DmacNumChannels) return;
          setEventOutputSelection(descriptorSection[chNum], eventSelection);
        }
        /***** Channels related *****/
        /**
         * @brief Select the source for triggering DMA transfer and the action
         * that is started when the source triggers the transfer
         * 
         * @param chNum The DMAC channel number
         * @param triggerSource The trigger source
         * @param triggerAction Which action should be started when the trigger happens
         * (i.e. transfer one beat at each trigger, transfer one block or transfer whole transaction )
         * @param burstLen The number of beats in a burst transfer (1 to 16 beats).
         * Used only when triggerAction is TriggerAction::Burst.
         */
        void setTrigger(uint8_t chNum, const TriggerSource &triggerSource, const TriggerAction &triggerAction, const uint8_t burstLen=1);

        /**
         * @brief Set the priority level of the specified DMAC channel
         * 
         * @param chNum The DMAC channel number
         * @param priority The priority level
         */
        void setPriority(uint8_t chNum, const Priority &priority);

        /**
         * @brief Enable event output for the specified DMAC channel
         * and change the output mode.
         * ENSURE THAT THE EVENT GENERATOR IS CONFIGURED IN THE
         * EVENT SYSTEM PERIPHERAL.
         * 
         * @param chNum The DMAC channel number
         * @param enable true to enable the event output, false to disable it
         * @param mode The output event mode
         */
        void enableEventOutput(uint8_t chNum, bool enable = true, const EventOutputMode &mode = EventOutputMode::Default);

        /**
         * @brief Enable event input for the specified DMAC channel
         * and set the action to take.
         * ENSURE THAT THE EVENT IS MAPPED IN THE EVENT SYSTEM PERIPHERAL.
         * 
         * @param chNum The DMAC channel number
         * @param action The action of the DMA transfer
         * @param enable true to enable event input, false to disable it
         */
        void enableEventInput(uint8_t chNum, const EventInputAction &action, bool enable = true);

        /**
         * @brief Configure specified DMAC channel and its corresponding
         * transfer descriptor. By default, the transfer descriptor is not
         * linked to other transfer descriptor.
         * 
         * @param chNum The DMAC channel number
         * @param beatSize The size of one bus access (i.e. byte, half word or word)
         * @param blockAction What to do after block transfer is completed
         * @param sourceAddress The start address for the source of the transfer
         * @param sourceIncrement The increment for the source address
         * @param destinationAddress The start address for the destination of the transfer
         * @param destinationIncrement The increment for the destination address
         * @param priority The priority level for the channel
         * @param transferSize The number of beats to transfer
         * @param triggerSource The source that triggers the transfer
         * @param triggerAction What to transfer at each trigger
         * @param burstLen The number of beats in a burst transfer (1 to 16).
         * Only used when triggerAction is TriggerAction::Burst.
         */
        void setupChannel(uint8_t chNum, const BeatSize &beatSize, const BlockAction &blockAction,
                          std::uintptr_t sourceAddress, const AddressIncrementSize &sourceIncrement,
                          std::uintptr_t destinationAddress, const AddressIncrementSize &destinationIncrement,
                          const uint16_t transferSize,
                          const Priority &priority,
                          const TriggerSource &triggerSource, const TriggerAction &triggerAction, const uint8_t burstLen=1);

        /**
         * @brief Enable or disable a given DMAC channel
         * 
         * @param chNum The DMAC channel number
         * @param enable true to enable, false to disable
         */
        void enableChannel(uint8_t chNum, bool enable=true);

        /**
         * @brief Start DMA transfer for the given channel
         * in software
         * 
         * @param chNum The DMAC channel number
         */
        void startTransfer(uint8_t chNum);

        /**
         * @brief Abort the DMA transfer for the specified channel
         * 
         * @param chNum The DMAC channel number
         */
        void abortTransfer(uint8_t chNum);

        /**
         * @brief Suspend (pause) the transfer for the specified DMAC channel
         * 
         * @param chNum The DMAC channel number
         */
        void suspendChannel(uint8_t chNum);

        /**
         * @brief Resume the transfer for the specified DMAC channel
         * 
         * @param chNum The DMAC channel number
         */
        void resumeChannel(uint8_t chNum);

        /**
         * @brief Reset the registers for the specified DMAC channel
         * 
         * @param chNum The DMAC channel number
         */
        void resetChannel(uint8_t chNum);

        /***** Interrupts related *****/
        
        /**
         * @brief Enable DMAC channel interrupt when 
         * transfer is completed
         * 
         * @param chNum The DMAC channel number
         */
        void enableTransferCompletedInterrupt(uint8_t chNum);

        /**
         * @brief Enable DMAC channel interrupt when 
         * error during transfer is encountered
         * 
         * @param chNum The DMAC channel number
         */
        void enableTransferErrorInterrupt(uint8_t chNum);

        /**
         * @brief Enable DMAC channel interrupt when 
         * the channel is suspended
         * 
         * @param chNum The DMAC channel number
         */
        void enableChannelSuspendedInterrupt(uint8_t chNum);

        /**
         * @brief Disable DMAC channel interrupt when 
         * transfer is completed
         * 
         * @param chNum The DMAC channel number
         */
        void disableTransferCompletedInterrupt(uint8_t chNum);

        /**
         * @brief Disable DMAC channel interrupt when 
         * error during transfer is encountered
         * 
         * @param chNum The DMAC channel number
         */
        void disableTransferErrorInterrupt(uint8_t chNum);

        /**
         * @brief Disable DMAC channel interrupt when 
         * the channel is suspended
         * 
         * @param chNum The DMAC channel number
         */
        void disableChannelSuspendedInterrupt(uint8_t chNum);
        
        /**
         * @brief Set callback function to be called
         * when transfer compete interrupt fires
         * 
         * @param chNum The DMAC channel number
         * @param callback The callback function
         */
        void setTransferCompletedCallback(uint8_t chNum, const std::function<void()> &callback);

        /**
         * @brief Set callback function to be called
         * when transfer error interrupt fires
         * 
         * @param chNum The DMAC channel number
         * @param callback The callback function
         */
        void setTransferErrorCallback(uint8_t chNum, const std::function<void()> &callback);

        /**
         * @brief Set callback function to be called
         * when channel suspend interrupt fires
         * 
         * @param chNum The DMAC channel number
         * @param callback The callback function
         */
        void setChannelSuspendedCallback(uint8_t chNum, const std::function<void()> &callback);

      private:
        DmaController() {}  //singleton class, private constructor
        void enableInterrupts();  // enable all DMAC interrupts in the NVIC

      private:
        /***** Transfer descriptor blocks *****/
        // Must reside in sram, on 128-bit aligned addresses
        static DmacDescriptor descriptorSection[DmacNumChannels] __attribute__((__aligned__(16)));
        static DmacDescriptor writebackSection[DmacNumChannels] __attribute__((__aligned__(16)));
    };
  }
}

#endif //DMAC_SAMD51_HPP