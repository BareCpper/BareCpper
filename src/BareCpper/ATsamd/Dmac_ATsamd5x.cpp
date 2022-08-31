#include "BareCpper/ATsamd/Dmac_ATsamd5x.hpp"

namespace BareCpper
{
  namespace SAMD51
  {
    // descriptor sections
    DmacDescriptor DmaController::descriptorSection[DmacNumChannels] = {};
    DmacDescriptor DmaController::writebackSection[DmacNumChannels] = {};
    // Hold functions to be called when the corresponding interrupt is fired
    std::array<std::function<void()>, DmacNumChannels> dmacTransferCompletedCallbacks;
		std::array<std::function<void()>, DmacNumChannels> dmacTransferErrorCallbacks;
    std::array<std::function<void()>, DmacNumChannels> dmacChannelSuspendedCallbacks;

    DmaController& DmaController::getInstance()
    {
      static DmaController dmaController;
      return dmaController;
    }

    void DmaController::init(const DmaController::Priority &priority)
    {
      // enable DMAC clock for accessing its registers
      MCLK->AHBMASK.bit.DMAC_ = 1;
      disable();  // disable the DMAC if it was running previously
      reset();    // Reset all DMAC registers to reset values
      // enable the requests only with the requested priority levels
      changePriorityLevel(priority, true);
      // set transfer and writeback descriptors base addresses
      DMAC->BASEADDR.reg = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(descriptorSection));
      DMAC->WRBADDR.reg = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(writebackSection));
      // enable interrupts in NVIC
      enableInterrupts();
    }

    void DmaController::reset()
    {
      DMAC->CTRL.bit.SWRST = 1;
      while(DMAC->CTRL.bit.SWRST);
    }

    void DmaController::enable()
    {
      DMAC->CTRL.bit.DMAENABLE = 1;
      DMAC->DBGCTRL.bit.DBGRUN = 1;
    }

    void DmaController::disable()
    {
      // abort all transfers
      for(size_t iChannel = 0; iChannel < DMAC_CH_NUM; ++iChannel)
      {
        abortTransfer(iChannel);
      }
      DMAC->CTRL.bit.DMAENABLE = 0;
      // wait for the DMAC to disable
      // i.e. when ongoing transfer is in progress
      while(DMAC->CTRL.bit.DMAENABLE);
    }

    void DmaController::changePriorityLevel(const DmaController::Priority &priority, bool enable)
    {
      // set which transfers will be handled base on priority levels
      switch(priority)
      {
        case Priority::AllLevels:
          if(enable) DMAC->CTRL.reg |= static_cast<uint8_t>(priority) << DMAC_CTRL_LVLEN0_Pos;
          else DMAC->CTRL.reg |= static_cast<uint8_t>(priority) << DMAC_CTRL_LVLEN0_Pos;
        break;
        case Priority::Level0:
          DMAC->CTRL.bit.LVLEN0 = enable;
        break;
        case Priority::Level1:
          DMAC->CTRL.bit.LVLEN1 = enable;
        break;
        case Priority::Level2:
          DMAC->CTRL.bit.LVLEN2 = enable;
        break;
        case Priority::Level3:
          DMAC->CTRL.bit.LVLEN3 = enable;
        break;
        default:
        break;
      }
    }

    void DmaController::changePriorityLevelScheduling(const DmaController::Priority &priority, 
                          const DmaController::QualityOfService &qos, 
                          bool roundRobin)
    {
      switch(priority)
      {
        case Priority::AllLevels:
          DMAC->PRICTRL0.bit.QOS0 = static_cast<uint8_t>(qos);
          DMAC->PRICTRL0.bit.RRLVLEN0 = roundRobin;
          DMAC->PRICTRL0.bit.QOS1 = static_cast<uint8_t>(qos);
          DMAC->PRICTRL0.bit.RRLVLEN1 = roundRobin;
          DMAC->PRICTRL0.bit.QOS2 = static_cast<uint8_t>(qos);
          DMAC->PRICTRL0.bit.RRLVLEN2 = roundRobin;
          DMAC->PRICTRL0.bit.QOS3 = static_cast<uint8_t>(qos);
          DMAC->PRICTRL0.bit.RRLVLEN2 = roundRobin;
        break;
        case Priority::Level0:
          DMAC->PRICTRL0.bit.QOS0 = static_cast<uint8_t>(qos);
          DMAC->PRICTRL0.bit.RRLVLEN0 = roundRobin;
        break;
        case Priority::Level1:
          DMAC->PRICTRL0.bit.QOS1 = static_cast<uint8_t>(qos);
          DMAC->PRICTRL0.bit.RRLVLEN1 = roundRobin;
        break;
        case Priority::Level2:
          DMAC->PRICTRL0.bit.QOS2 = static_cast<uint8_t>(qos);
          DMAC->PRICTRL0.bit.RRLVLEN2 = roundRobin;
        break;
        case Priority::Level3:
          DMAC->PRICTRL0.bit.QOS3 = static_cast<uint8_t>(qos);
          DMAC->PRICTRL0.bit.RRLVLEN2 = roundRobin;
        break;
        default:
        break;
      }
    }

    void DmaController::setSourceAddress(DmacDescriptor &descriptor, const std::uintptr_t addr)
    {
      descriptor.SRCADDR.reg = static_cast<uint32_t>(addr);
    }
    
    void DmaController::setDestinationAddress(DmacDescriptor &descriptor, const std::uintptr_t addr)
    {
      descriptor.DSTADDR.reg = static_cast<uint32_t>(addr);
    }

    void DmaController::setBeatSize(DmacDescriptor &descriptor, const DmaController::BeatSize &beatSize)
    {
      descriptor.BTCTRL.bit.BEATSIZE = static_cast<uint8_t>(beatSize);
    }

    void DmaController::setTransferSize(DmacDescriptor &descriptor, const uint16_t size)
    {
      descriptor.BTCNT.reg = size;
      // adjust source and destination addresses, if increment is used
      // based on the datasheet, section 22.6.2.7 Addressing, p.350-351
      if(descriptor.BTCTRL.bit.SRCINC)
      {
        uint32_t addrShift = size * (1 << descriptor.BTCTRL.bit.BEATSIZE);
        if(descriptor.BTCTRL.bit.STEPSEL)
          addrShift *= (1 << descriptor.BTCTRL.bit.STEPSIZE);
        descriptor.SRCADDR.reg += addrShift;  
      }

      if(descriptor.BTCTRL.bit.DSTINC)
      {
        uint32_t addrShift = size * (1 << descriptor.BTCTRL.bit.BEATSIZE);
        if(!descriptor.BTCTRL.bit.STEPSEL)
          addrShift *= (1 << descriptor.BTCTRL.bit.STEPSIZE);
        descriptor.DSTADDR.reg += addrShift;
      }
    }

    void DmaController::setSourceAddressIncrement(DmacDescriptor &descriptor, const DmaController::AddressIncrementSize &stepSize)
    {
      if(stepSize == AddressIncrementSize::Disabled)
      {
        // disable source increment
        descriptor.BTCTRL.bit.SRCINC = 0;
      }
      else
      {
        // enable source increment
        descriptor.BTCTRL.bit.SRCINC = 1;        
        // if step size is different from x1,
        // enable flexible source increment,
        // but this will STOP destination flexible increment
        // @todo: handle this case better
        // @todo: reset STEPSIZE somehow, may hold invalid value
        // because of increment changes
        if(stepSize != AddressIncrementSize::x1)
        {
          descriptor.BTCTRL.bit.STEPSIZE = static_cast<uint8_t>(stepSize);
          descriptor.BTCTRL.bit.STEPSEL = 0;
        }
      }
    }

    void DmaController::setDestinationAddressIncrement(DmacDescriptor &descriptor, const DmaController::AddressIncrementSize &stepSize)
    {
      if(stepSize == AddressIncrementSize::Disabled)
      {
        // disable destination increment
        descriptor.BTCTRL.bit.DSTINC = 0;
      }
      else
      {
        // enable destination increment
        descriptor.BTCTRL.bit.DSTINC = 1;        
        // if step size is different from x1,
        // enable flexible destination increment,
        // but this will STOP source flexible increment
        // @todo: handle this case better
        // @todo: reset STEPSIZE somehow, may hold invalid value
        // because of increment changes
        if(stepSize != AddressIncrementSize::x1)
        {
          descriptor.BTCTRL.bit.STEPSIZE = static_cast<uint8_t>(stepSize);
          descriptor.BTCTRL.bit.STEPSEL = 0;
        }
      }
    }

    void DmaController::setBlockAction(DmacDescriptor &descriptor, const DmaController::BlockAction &blockAction)
    {
      descriptor.BTCTRL.bit.BLOCKACT = static_cast<uint8_t>(blockAction);
    }

    void DmaController::setEventOutputSelection(DmacDescriptor &descriptor, const DmaController::EventOutput &eventSelection)
    {
      descriptor.BTCTRL.bit.EVOSEL = static_cast<uint8_t>(eventSelection);
    }

    void DmaController::setupDescriptor(DmacDescriptor &descriptor, const DmaController::BeatSize &beatSize, const DmaController::BlockAction &blockAction,
                          std::uintptr_t sourceAddress, const DmaController::AddressIncrementSize &sourceIncrement,
                          std::uintptr_t destinationAddress, const DmaController::AddressIncrementSize &destinationIncrement,
                          const uint16_t transferSize)
    {
      setBeatSize(descriptor, beatSize);
      setBlockAction(descriptor, blockAction);
      setSourceAddress(descriptor, sourceAddress);
      setSourceAddressIncrement(descriptor, sourceIncrement);
      setDestinationAddress(descriptor, destinationAddress);
      setDestinationAddressIncrement(descriptor, destinationIncrement);
      setTransferSize(descriptor, transferSize);

      linkDescriptors(&descriptor, nullptr);
    }                          

    void DmaController::setTrigger(uint8_t chNum
                    , const DmaController::TriggerSource &triggerSource
                    , const DmaController::TriggerAction &triggerAction
                    , const uint8_t burstLen)
    {
      if(chNum > DmacNumChannels) return;
      DMAC->Channel[chNum].CHCTRLA.bit.TRIGSRC = static_cast<uint8_t>(triggerSource);
      DMAC->Channel[chNum].CHCTRLA.bit.TRIGACT = static_cast<uint8_t>(triggerAction);
      
      if(triggerAction == TriggerAction::Burst && burstLen > 0 && burstLen < 17)
      {
        DMAC->Channel[chNum].CHCTRLA.bit.BURSTLEN = burstLen - 1;
      }
    }

    void DmaController::setPriority(uint8_t chNum, const Priority &priority)
    {
      if(chNum > DmacNumChannels || priority == Priority::AllLevels) return;
      DMAC->Channel[chNum].CHPRILVL.bit.PRILVL = static_cast<uint8_t>(priority);
    }

    void DmaController::linkDescriptors(DmacDescriptor *first, const DmacDescriptor *second)
    {
      first->DESCADDR.reg = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(second));
    }

    void DmaController::linkDescriptors(uint8_t chNum, const std::uintptr_t secondDescriptorAddress)
    {
      if (chNum > DmacNumChannels) return;
      descriptorSection[chNum].DESCADDR.reg = static_cast<uint32_t>(secondDescriptorAddress);
    }

    void DmaController::linkDescriptors(uint8_t chNum, const DmacDescriptor *descriptor)
    {
      if (chNum > DmacNumChannels) return;
      descriptorSection[chNum].DESCADDR.reg = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(descriptor));
    }

    std::uintptr_t DmaController::getDescriptorAddress(uint8_t chNum)
    {
      if (chNum > DmacNumChannels) return 0;
      return reinterpret_cast<std::uintptr_t>(&DmaController::descriptorSection[chNum]);
    }

    void DmaController::enableEventOutput(uint8_t chNum, bool enable, const DmaController::EventOutputMode &mode)
    {
      if (chNum > DmacNumChannels) return;
      // if event output was not enabled, enable it
      // otherwise, do nothing
      if(enable && !DMAC->Channel[chNum].CHEVCTRL.bit.EVOE)
      {
        DMAC->Channel[chNum].CHEVCTRL.bit.EVOE = 1;
        DMAC->Channel[chNum].CHEVCTRL.bit.EVOMODE = static_cast<uint8_t>(mode);
      }
      // if event output was enabled, disable it
      // otherwise, do nothing
      else if(!enable && DMAC->Channel[chNum].CHEVCTRL.bit.EVOE)
      {
        DMAC->Channel[chNum].CHEVCTRL.bit.EVOE = 0;
      }
    }

    void DmaController::enableEventInput(uint8_t chNum, const DmaController::EventInputAction &action, bool enable)
    {
      if (chNum > DmacNumChannels) return;
      // if event input was not enabled, enable it
      // otherwise, do nothing
      if(enable && !DMAC->Channel[chNum].CHEVCTRL.bit.EVIE)
      {
        DMAC->Channel[chNum].CHEVCTRL.bit.EVIE = 1;
        DMAC->Channel[chNum].CHEVCTRL.bit.EVACT = static_cast<uint8_t>(action);
      }
      // if event input was enabled, disable it
      // otherwise, do nothing
      else if(!enable && DMAC->Channel[chNum].CHEVCTRL.bit.EVIE)
      {
        DMAC->Channel[chNum].CHEVCTRL.bit.EVIE = 0;
      }
    }

    void DmaController::setupChannel(uint8_t chNum, const DmaController::BeatSize &beatSize, const DmaController::BlockAction &blockAction,
                    std::uintptr_t sourceAddress, const DmaController::AddressIncrementSize &sourceIncrement,
                    std::uintptr_t destinationAddress, const DmaController::AddressIncrementSize &destinationIncrement,
                    const uint16_t transferSize,
                    const DmaController::Priority &priority,
                    const DmaController::TriggerSource &triggerSource, const DmaController::TriggerAction &triggerAction, const uint8_t burstLen)
    {
      if (chNum > DmacNumChannels) return;

      setBeatSize(descriptorSection[chNum], beatSize);
      setBlockAction(descriptorSection[chNum], blockAction);
      setSourceAddress(descriptorSection[chNum], sourceAddress);
      setSourceAddressIncrement(descriptorSection[chNum], sourceIncrement);
      setDestinationAddress(descriptorSection[chNum], destinationAddress);
      setDestinationAddressIncrement(descriptorSection[chNum], destinationIncrement);
      setTransferSize(descriptorSection[chNum], transferSize);
      
      setPriority(chNum, priority);
      setTrigger(chNum, triggerSource, triggerAction, burstLen);

      // link the default channel descriptor to nullptr
      linkDescriptors(&descriptorSection[chNum], nullptr);
    }

    void DmaController::enableChannel(uint8_t chNum, bool enable)
    {
      if(chNum > DmacNumChannels) return;
      // if channel is not enabled and needs to be enabled, enable it
      // if channel is enabled and needs to be disabled, disable it
      // otherwise do nothing
      if(enable && !DMAC->Channel[chNum].CHCTRLA.bit.ENABLE)
      {
        // set the Valid bit in the descriptor section
        descriptorSection[chNum].BTCTRL.bit.VALID = 1;
        DMAC->Channel[chNum].CHCTRLA.bit.ENABLE = 1;
      }
      else if(!enable && DMAC->Channel[chNum].CHCTRLA.bit.ENABLE)
      {
        DMAC->Channel[chNum].CHCTRLA.bit.ENABLE = 0;
        // wait for the DMAC channel to disable
        // i.e. when ongoing transfer is in progress
        while(DMAC->Channel[chNum].CHCTRLA.bit.ENABLE);
      }
    }

    void DmaController::startTransfer(uint8_t chNum)
    {
      if(chNum > DmacNumChannels) return;
      // software trigger DMA transfer
      DMAC->SWTRIGCTRL.reg |= (1 << chNum);
    }

    void DmaController::abortTransfer(uint8_t chNum)
    {
      if(chNum > DmacNumChannels) return;
      DMAC->Channel[chNum].CHCTRLA.bit.ENABLE = 0;
      // wait for the DMAC channel to disable
      // i.e. when ongoing transfer is in progress
      while(DMAC->Channel[chNum].CHCTRLA.bit.ENABLE);
    }

    void DmaController::suspendChannel(uint8_t chNum)
    {
      if(chNum > DmacNumChannels) return;
      DMAC->Channel[chNum].CHCTRLB.bit.CMD = DMAC_CHCTRLB_CMD_SUSPEND_Val;
    }

    void DmaController::resumeChannel(uint8_t chNum)
    {
      if(chNum > DmacNumChannels) return;
      DMAC->Channel[chNum].CHCTRLB.bit.CMD = DMAC_CHCTRLB_CMD_RESUME_Val;
    }

    void DmaController::resetChannel(uint8_t chNum)
    {
      if(chNum > DmacNumChannels) return;
      // if channel is enabled, disable and reset
      enableChannel(chNum, false);
      DMAC->Channel[chNum].CHCTRLA.bit.SWRST = 1;
      while(DMAC->Channel[chNum].CHCTRLA.bit.SWRST);
    }

    void DmaController::enableTransferCompletedInterrupt(uint8_t chNum)
    {
      if(chNum > DmacNumChannels) return;
      DMAC->Channel[chNum].CHINTENSET.bit.TCMPL = 1;
    }

    void DmaController::enableTransferErrorInterrupt(uint8_t chNum)
    {
      if(chNum > DmacNumChannels) return;
      DMAC->Channel[chNum].CHINTENSET.bit.TERR = 1;
    }

    void DmaController::enableChannelSuspendedInterrupt(uint8_t chNum)
    {
      if(chNum > DmacNumChannels) return;
      DMAC->Channel[chNum].CHINTENSET.bit.SUSP = 1;
    }
    
    void DmaController::disableTransferCompletedInterrupt(uint8_t chNum)
    {
      if(chNum > DmacNumChannels) return;
      DMAC->Channel[chNum].CHINTENCLR.bit.TCMPL = 1;
    }
    
    void DmaController::disableTransferErrorInterrupt(uint8_t chNum)
    {
      if(chNum > DmacNumChannels) return;
      DMAC->Channel[chNum].CHINTENCLR.bit.TERR = 1;
    }

    void DmaController::disableChannelSuspendedInterrupt(uint8_t chNum)
    {
      if(chNum > DmacNumChannels) return;
      DMAC->Channel[chNum].CHINTENCLR.bit.SUSP = 1;
    }
    
    void DmaController::setTransferCompletedCallback(uint8_t chNum, const std::function<void()> &callback)
    {
      if(chNum > DmacNumChannels) return;
      dmacTransferCompletedCallbacks[chNum] = callback;

    }

    void DmaController::setTransferErrorCallback(uint8_t chNum, const std::function<void()> &callback)
    {
      if(chNum > DmacNumChannels) return;
      dmacTransferErrorCallbacks[chNum] = callback;
    }

    void DmaController::setChannelSuspendedCallback(uint8_t chNum, const std::function<void()> &callback)
    {
      if(chNum > DmacNumChannels) return;
      dmacChannelSuspendedCallbacks[chNum] = callback;
    }

    void DmaController::enableInterrupts()
    {
      for (int i = 0; i < 5; ++i)
      {
        IRQn_Type irqn = static_cast<IRQn_Type>(static_cast<int>(DMAC_0_IRQn) + i);
        NVIC_DisableIRQ(irqn);
        NVIC_ClearPendingIRQ(irqn);
        NVIC_EnableIRQ(irqn);
      }
    }

    // common interrupt handler for every DMAC interrupt
    static void DmacInterruptHandler()
    {
      using namespace BareCpper::SAMD51;
      // find the number of channel
      uint8_t chNum = DMAC->INTPEND.bit.ID;
      // find what kind of interrupt was fired
      // transfer error interrupt
      if(DMAC->Channel[chNum].CHINTFLAG.bit.TERR)
      {
        if(dmacTransferErrorCallbacks[chNum]) dmacTransferErrorCallbacks[chNum]();
        DMAC->Channel[chNum].CHINTFLAG.bit.TERR = 1;
      }
      // transfer complete interrupt
      else if(DMAC->Channel[chNum].CHINTFLAG.bit.TCMPL)
      {
        if(dmacTransferCompletedCallbacks[chNum]) dmacTransferCompletedCallbacks[chNum]();
        DMAC->Channel[chNum].CHINTFLAG.bit.TCMPL = 1;
      }
      // channel suspend interrupt
      else if(DMAC->Channel[chNum].CHINTFLAG.bit.SUSP)
      {
        if(dmacChannelSuspendedCallbacks[chNum]) dmacChannelSuspendedCallbacks[chNum]();
        DMAC->Channel[chNum].CHINTFLAG.bit.SUSP = 1;
      }
    }

  }
}

// datasheet, Table 10-1. Interrupt Line Mapping, p.66
// we will use same code for every handler
extern "C"
{
  // DMAC0 handles DMAC channel 0 interrupts
  void DMAC_0_Handler()
  {
    BareCpper::SAMD51::DmacInterruptHandler();
  }

  // DMAC0 handles DMAC channel 1 interrupts
  void DMAC_1_Handler()
  {
    BareCpper::SAMD51::DmacInterruptHandler();
  }

  // DMAC0 handles DMAC channel 2 interrupts
  void DMAC_2_Handler()
  {
    BareCpper::SAMD51::DmacInterruptHandler();
  }

  // DMAC0 handles DMAC channel 3 interrupts
  void DMAC_3_Handler()
  {
    BareCpper::SAMD51::DmacInterruptHandler();
  }

  // DMAC0 handles DMAC channel 4-31 interrupts
  void DMAC_4_Handler()
  {
    BareCpper::SAMD51::DmacInterruptHandler();
  }
}