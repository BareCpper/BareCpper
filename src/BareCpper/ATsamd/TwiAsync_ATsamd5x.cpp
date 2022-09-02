#if ARDUINO
#include "BareCpper/include/BareCpper/ATsamd/TwiAsync_ATsamd5x.hpp"
#else
#include "BareCpper/ATsamd/TwiAsync_ATsamd5x.hpp"
#endif
/**
 * @note If there isn't any used symbol from this file, it
 * probably won't be linked in the target linking BareCpper,
 * as it will not include this object file.
 * The static library will include both symbols for the
 * handlers, the WEAK ones and these ones. When linking,
 * because no symbol is used from this file, the WEAK
 * ones will be included in compiled binary.
 * Can be checked with readelf -s <binary>.
 * 
 * To resolve this, one global variable is provided,
 * defined in this translation unit and used in a function
 * in the header file.
*/
namespace BareCpper
{
  bool ensureTwiAsync_ATsamd5xLink = false;
}

extern "C"
{
  void SERCOM2_0_Handler(void)
  {
    BareCpper::TwiAsync::SERCOM2_Handler();
  }

  void SERCOM2_1_Handler(void)
  {
    BareCpper::TwiAsync::SERCOM2_Handler();
  }

  void SERCOM2_2_Handler(void)
  {
    BareCpper::TwiAsync::SERCOM2_Handler();
  }

  void SERCOM2_3_Handler(void)
  {
    BareCpper::TwiAsync::SERCOM2_Handler();
  }
}