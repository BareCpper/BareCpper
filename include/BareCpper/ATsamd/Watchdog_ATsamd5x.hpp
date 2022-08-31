#ifndef WATCHDOG_SAMD51_HPP
#define WATCHDOG_SAMD51_HPP

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <cstdint>
#include <sam.h>

namespace BareCpper
{
  namespace SAMD51
  {
    class Watchdog
    {
      public:
        void init(const uint32_t intervalUs);
        void start();
        void stop();
        void feed();
    };
  }
}

#endif //WATCHDOG_SAMD51_HPP