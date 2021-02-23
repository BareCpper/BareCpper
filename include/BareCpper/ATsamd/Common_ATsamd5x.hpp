#ifndef BARECPPER_COMMON_ATSAM5X_H_
#define BARECPPER_COMMON_ATSAM5X_H_

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include "sam.h"

// @{ Map new v2.x names to Samd51 v1.x @note Old names may conflict/overlap with user code 
#if HEADER_FORMAT_VERSION_MAJOR>1
#define PORT_REGS using ::Port; PORT  //< Disambiguate Sam.h Port vs BareCpper::Gpio::Port
#define GROUP Group ///< Port->Group in v1 headers bs ->GROUP in v2
#error "TODO: Support newer Atmel DFP!"
#endif

#if   defined(__SAMD51G18A__) || defined(__ATSAMD51G18A__) || defined(__SAMD51G19A__) || defined(__ATSAMD51G19A__)
#   define SAM_IC_PINCOUNT 48 ///< VQFN 48 
#elif defined(__SAMD51J18A__) || defined(__ATSAMD51J18A__) || defined(__SAMD51J19A__) || defined(__ATSAMD51J19A__) || defined(__SAMD51J20A__) || defined(__ATSAMD51J20A__)
#   define SAM_IC_PINCOUNT 64 ///< TQFP/VQFN/WLCSP 64 
#elif defined(__SAMD51N19A__) || defined(__ATSAMD51N19A__) || defined(__SAMD51N20A__) || defined(__ATSAMD51N20A__)
#   define SAM_IC_PINCOUNT 100 ///< TQFP 100
#elif defined(__SAMD51P19A__) || defined(__ATSAMD51P19A__) || defined(__SAMD51P20A__) || defined(__ATSAMD51P20A__)
#   if SAM_IC_TQFP
#       define SAM_IC_PINCOUNT 128 ///< TQFP 128
#   else 
#       define SAM_IC_PINCOUNT 120 ///< TFBGA 120
#   endif
#else
#   error Library does not support the specified device.
#endif

#if SAM_IC_PINCOUNT >= 48
#   define SAM_G_cond( T_, R_ ) T_
#else
#   define SAM_G_cond( T_ , R_) R_
#endif
#if SAM_IC_PINCOUNT >= 64
#   define SAM_J_cond( T_ , R_) T_
#else
#   define SAM_J_cond( T_, R_) R_
#endif
#if SAM_IC_PINCOUNT >= 100
#   define SAM_N_cond( T_, R_ ) T_
#else
#   define SAM_N_cond( T_, R_ ) R_
#endif
#if SAM_IC_PINCOUNT >= 120
#   define SAM_P_cond( T_, R_ ) T_
#else
#   define SAM_P_cond( T_, R_ ) R_
#endif

/// @{ Platform optional - comiples to nothing if not set
# define SAM_G_if( T_ ) SAM_G_cond( T_, )
# define SAM_J_if( T_ ) SAM_J_cond( T_, )
# define SAM_N_if( T_ ) SAM_N_cond( T_, )
# define SAM_P_if( T_ ) SAM_P_cond( T_, )
/// @} Platform optional - comiples to nothing if not set

/// @{ Platform optional - compiles to Void if not set
# define SAM_G( T_ ) SAM_G_cond( T_,void)
# define SAM_J( T_ ) SAM_J_cond( T_,void)
# define SAM_N( T_ ) SAM_N_cond( T_,void)
# define SAM_P( T_ ) SAM_P_cond( T_,void)
/// @} Platform optional - compiles to Void if not set

namespace BareCpper {
namespace ATsamd5x {

    /* @param[in] function  The pin function is given by a 32 - bit wide bitfield found in the header files for the device
                            e.g. PINMUX_PA12C_SERCOM2_PAD0
    */
    inline void setPinFunction(const uint8_t iPort, const uint8_t iPin, const uint32_t function)
    {
        using ::Port; //< Disambiguate Sam.h vs BareCpper::Gpio

        PORT->Group[iPort].PINCFG[iPin].bit.PMUXEN = (function <= PORT_PMUX_PMUXE_Msk); //< 0xF is maximum valid function

        if (iPin & 0x1) // Odd numbered pin 
        {
            PORT->Group[iPort].PMUX[iPin / 2].bit.PMUXO = function;
        }
        else // Even numbered pin
        {
            PORT->Group[iPort].PMUX[iPin / 2].bit.PMUXE = function;
        }
    }

} //END: ATsamd5x
} //END: BareCpper

#endif //BARECPPER_COMMON_ATSAM5X_H_