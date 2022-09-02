#ifndef EVENT_SYSTEM_SAMD51_HPP
#define EVENT_SYSTEM_SAMD51_HPP

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <cstdint>
#include <cstddef>

#include <sam.h>

namespace BareCpper
{
  namespace SAMD51
  {
    static constexpr size_t EventSystemNumChannels = 32;

    class EventSystem
    {
      public:
        enum class EdgeSelection : uint8_t
        {
          NoEventOutput = 0x0,
          Rising = 0x1,
          Falling = 0x2,
          Both = 0x3
        };

        enum class PathSelection : uint8_t
        {
          Synchronous = 0x0,
          Resynchronized = 0x1,
          Asynchronous = 0x2
        };

        /**
         * @brief Enum for storing the possible event generator
         * for a Event System channel
         * 
         */
        enum class EventGenerator : uint8_t
        {
          None                        = 0x00,
          OscCtrlXoscFail0            = 0x01,
          OscCtrlXoscFail1            = 0x02,
          Osc32KctrlXosc32kFail       = 0x03,
          RtcPer0                     = 0x04,
          RtcPer1                     = 0x05,
          RtcPer2                     = 0x06,
          RtcPer3                     = 0x07,
          RtcPer4                     = 0x08,
          RtcPer5                     = 0x09,
          RtcPer6                     = 0x0A,
          RtcPer7                     = 0x0B,
          RtcCmp0                     = 0x0C,
          RtcCmp1                     = 0x0D,
          RtcCmp2                     = 0x0E,
          RtcCmp3                     = 0x0F,
          RtcTamper                   = 0x10,
          RtcOvf                      = 0x11,
          EicExtInt0                  = 0x12,
          EicExtInt1                  = 0x13,
          EicExtInt2                  = 0x14,
          EicExtInt3                  = 0x15,
          EicExtInt4                  = 0x16,
          EicExtInt5                  = 0x17,
          EicExtInt6                  = 0x18,
          EicExtInt7                  = 0x19,
          EicExtInt8                  = 0x1A,
          EicExtInt9                  = 0x1B,
          EicExtInt10                 = 0x1C,
          EicExtInt11                 = 0x1D,
          EicExtInt12                 = 0x1E,
          EicExtInt13                 = 0x1F,
          EicExtInt14                 = 0x20,
          EicExtInt15                 = 0x21,
          DmacCh0                     = 0x22,
          DmacCh1                     = 0x23,
          DmacCh2                     = 0x24,
          DmacCh3                     = 0x25,
          PacAccerr                   = 0x26,
          Tcc0Ovf                     = 0x29,
          Tcc0Trg                     = 0x2A,
          Tcc0Cnt                     = 0x2B,
          Tcc0Mc0                     = 0x2C,
          Tcc0Mc1                     = 0x2D,
          Tcc0Mc2                     = 0x2E,
          Tcc0Mc3                     = 0x2F,
          Tcc0Mc4                     = 0x30,
          Tcc0Mc5                     = 0x31,
          Tcc1Ovf                     = 0x32,
          Tcc1Trg                     = 0x33,
          Tcc1Cnt                     = 0x34,
          Tcc1Mc0                     = 0x35,
          Tcc1Mc1                     = 0x36,
          Tcc1Mc2                     = 0x37,
          Tcc1Mc3                     = 0x38,
          Tcc2Ovf                     = 0x39,
          Tcc2Trg                     = 0x3A,
          Tcc2Cnt                     = 0x3B,
          Tcc2Mc0                     = 0x3C,
          Tcc2Mc1                     = 0x3D,
          Tcc2Mc2                     = 0x3E,
          Tcc3Ovf                     = 0x3F,
          Tcc3Trg                     = 0x40,
          Tcc3Cnt                     = 0x41,
          Tcc3Mc0                     = 0x42,
          Tcc3Mc1                     = 0x43,
          Tcc4Ovf                     = 0x44,
          Tcc4Trg                     = 0x45,
          Tcc4Cnt                     = 0x46,
          Tcc4Mc0                     = 0x47,
          Tcc4Mc1                     = 0x48,
          Tc0Ovf                      = 0x49,
          Tc0Mc0                      = 0x4A,
          Tc0Mc1                      = 0x4B,
          Tc1Ovf                      = 0x4C,
          Tc1Mc0                      = 0x4D,
          Tc1Mc1                      = 0x4E,
          Tc2Ovf                      = 0x4F,
          Tc2Mc0                      = 0x50,
          Tc2Mc1                      = 0x51,
          Tc3Ovf                      = 0x52,
          Tc3Mc0                      = 0x53,
          Tc3Mc1                      = 0x54,
          Tc4Ovf                      = 0x55,
          Tc4Mc0                      = 0x56,
          Tc4Mc1                      = 0x57,
          Tc5Ovf                      = 0x58,
          Tc5Mc0                      = 0x59,
          Tc5Mc1                      = 0x5A,
          Tc6Ovf                      = 0x5B,
          Tc6Mc0                      = 0x5C,
          Tc6Mc1                      = 0x5D,
          Tc7Ovf                      = 0x5E,
          Tc7Mc0                      = 0x5F,
          Tc7Mc1                      = 0x60,
          PdecOvf                     = 0x61,
          PdecErr                     = 0x62,
          PdecDir                     = 0x63,
          PdecVlc                     = 0x64,
          PdecMc0                     = 0x65,
          PdecMc1                     = 0x66,
          Adc0Resrdy                  = 0x67,
          Adc0WinMon                  = 0x68,
          Adc1Resrdy                  = 0x69,
          Adc1WinMon                  = 0x6A,
          AcComp0                     = 0x6B,
          AcComp1                     = 0x6C,
          AcWin                       = 0x6D,
          DacEmpty0                   = 0x6E,
          DacEmpty1                   = 0x6F,
          DacResrdy0                  = 0x70,
          DacResrdy1                  = 0x71,
          GmacTsuCmp                  = 0x72,
          TrngReady                   = 0x73,
          CclLutOut0                  = 0x74,
          CclLutOut1                  = 0x75,
          CclLutOut2                  = 0x76,
          CclLutOut3                  = 0x77
        };

        enum class EventUser : uint8_t
        {
          RtcTamper       = 0,
          PortEv0         = 1,
          PortEv1         = 2,
          PortEv2         = 3,
          PortEv3         = 4,
          DmacCh0         = 5,
          DmacCh1         = 6,
          DmacCh2         = 7,
          DmacCh3         = 8,
          DmacCh4         = 9,
          DmacCh5         = 10,
          DmacCh6         = 11,
          DmacCh7         = 12,
          Cm4TraceStart   = 14,
          Cm4TraceStop    = 15,
          Cm4TraceTrig    = 16,
          Tcc0Ev0         = 17,
          Tcc0Ev1         = 18,
          Tcc0Mc0         = 19,
          Tcc0Mc1         = 20,
          Tcc0Mc2         = 21,
          Tcc0Mc3         = 22,
          Tcc0Mc4         = 23,
          Tcc0Mc5         = 24,
          Tcc1Ev0         = 25,
          Tcc1Ev1         = 26,
          Tcc1Mc0         = 27,
          Tcc1Mc1         = 28,
          Tcc1Mc2         = 29,
          Tcc1Mc3         = 30,
          Tcc2Ev0         = 31,
          Tcc2Ev1         = 32,
          Tcc2Mc0         = 33,
          Tcc2Mc1         = 34,
          Tcc2Mc2         = 35,
          Tcc3Ev0         = 36,
          Tcc3Ev1         = 37,
          Tcc3Mc0         = 38,
          Tcc3Mc1         = 39,
          Tcc4Ev0         = 40,
          Tcc4Ev1         = 41,
          Tcc4Mc0         = 42,
          Tcc4Mc1         = 43,
          Tc0Evu          = 44,
          Tc1Evu          = 45,
          Tc2Evu          = 46,
          Tc3Evu          = 47,
          Tc4Evu          = 48,
          Tc5Evu          = 49,
          Tc6Evu          = 50,
          Tc7Evu          = 51,
          PdecEvu0        = 52,
          PdecEvu1        = 53,
          PdecEvu2        = 54,
          Adc0Start       = 55,
          Adc0Sync        = 56,
          Adc1Start       = 57,
          Adc1Sync        = 58,
          AcSoc0          = 59,
          AcSoc1          = 60,
          DacStart0       = 61,
          DacStart1       = 62,
          CclLutIn0       = 63,
          CclLutIn1       = 64,
          CclLutIn2       = 65,
          CclLutIn3       = 66,
        };

      public:
        static EventSystem &getInstance();
        // remove copy and move constructors and assignment operators
        // for singleton class
        EventSystem(const EventSystem &) = delete;
        EventSystem(EventSystem &&) = delete;
        EventSystem& operator=(const EventSystem &) = delete;
        EventSystem&& operator=(EventSystem &&) = delete;

        void init();
        void reset();
        void changePriorityScheme(bool roundRobin);

        void generateEvent(const uint8_t chNum);
        void selectEventGenerator(const uint8_t chNum, const EventGenerator &eventGenerator);
        void setEventUserChannel(const uint8_t chNum, const EventUser &eventUser);
        void setChannelPath(const uint8_t chNum, const PathSelection &path);
        void setEdgeDetection(const uint8_t chNum, const EdgeSelection &edge = EdgeSelection::Rising);
      private:
        EventSystem() {}
    };
  }
}

#endif  // EVENT_SYSTEM_SAMD51_HPP