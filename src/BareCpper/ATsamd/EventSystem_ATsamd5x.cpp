#include "BareCpper/ATsamd/EventSystem_ATsamd5x.hpp"

namespace BareCpper
{
  namespace SAMD51
  {
    EventSystem& EventSystem::getInstance()
    {
      static EventSystem eventSys;
      return eventSys;
    }

    void EventSystem::init()
    {
      // enable writing to EVSYS registers
      MCLK->APBBMASK.bit.EVSYS_ = 1;
    }
    
    void EventSystem::reset()
    {
      EVSYS->CTRLA.bit.SWRST = 1;
      while(EVSYS->CTRLA.bit.SWRST);
    }
    
    void EventSystem::changePriorityScheme(bool roundRobin)
    {
      EVSYS->PRICTRL.bit.RREN = roundRobin;
    }
    
    void EventSystem::generateEvent(const uint8_t chNum)
    {
      if(chNum > EventSystemNumChannels) return;
      EVSYS->SWEVT.reg |= (1 << chNum);
    }
    
    void EventSystem::selectEventGenerator(const uint8_t chNum, const EventSystem::EventGenerator &eventGenerator)
    {
      if(chNum > EventSystemNumChannels) return;
      EVSYS->Channel[chNum].CHANNEL.bit.EVGEN = static_cast<uint8_t>(eventGenerator);
    }
    
    void EventSystem::setEventUserChannel(const uint8_t chNum, const EventSystem::EventUser &eventUser)
    {
      if(chNum > EventSystemNumChannels) return;
      EVSYS->USER[static_cast<uint8_t>(eventUser)].bit.CHANNEL = chNum + 1;
    }
    
    void EventSystem::setChannelPath(const uint8_t chNum, const EventSystem::PathSelection &path)
    {
      if(chNum > EventSystemNumChannels) return;
      if((path == PathSelection::Resynchronized || path == PathSelection::Synchronous) && (chNum >= 0 && chNum <= 11))
      {
        // channel must be between 0 and 11
        // enable GCLK for the channel
        GCLK->PCHCTRL[EVSYS_GCLK_ID_0 + chNum].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
        EVSYS->Channel[chNum].CHANNEL.bit.PATH = static_cast<uint8_t>(path);
      }
      else
      {
        EVSYS->Channel[chNum].CHANNEL.bit.PATH = static_cast<uint8_t>(PathSelection::Asynchronous);
      }
    }
    
    void EventSystem::setEdgeDetection(const uint8_t chNum, const EventSystem::EdgeSelection &edge)
    {
      if(chNum > EventSystemNumChannels) return;
      EVSYS->Channel[chNum].CHANNEL.bit.EDGSEL = static_cast<uint8_t>(edge);
    }
  }
}