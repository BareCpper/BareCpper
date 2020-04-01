
#ifndef BARECPPER_DEVICEUID_HPP_
#  error "Include <BareCpper/DeviceUid.hpp> instead of this file."
#endif

#if !NRF52
#  error "nRF52 library error, please check and update accordingly."
#endif

#include <cstring> //< std::memcpy

namespace BareCpper
{

    inline IoDescriptor DeviceUid::io()
    {
        return {
            [](const IoDescriptor& descriptor, uint8_t* const buffer, const uint16_t bufferLength) ->int32_t
            {
                const uint32_t serialNumber[4] = { 
                     0, 0, 0, 0 //< @todo Implemtn reading from nrf52 serial
                };
                std::memcpy(buffer, reinterpret_cast<const uint8_t*>(serialNumber), std::min(bufferLength, (uint16_t)sizeof(serialNumber) ) );

                return sizeof(serialNumber); //< Read result
            },  
            nullptr
        };
    }

} //END: BareCpper