#ifndef BARECPPER_DEVICEUID_HPP_
#define BARECPPER_DEVICEUID_HPP_

#include "Io.hpp"

namespace BareCpper
{
    /** Device unique identifer
    */
    class DeviceUid
    {
    public:

        IoDescriptor io();

    };

} //END: BareCpper

#if !DOXYGEN ///< Platform specific interface definitions
    #if NRF52_SERIES
        #include "nRF52/DeviceUid_nRF52.hpp"
    #elif __SAMD51__
        #include "ATsamd/DeviceUid_Atsamd5x.hpp"
    #endif
#endif

#endif