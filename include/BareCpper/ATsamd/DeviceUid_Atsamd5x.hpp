
#ifndef BARECPPER_DEVICEUID_HPP_
#  error "Include <BareCpper/DeviceUid.hpp> instead of this file."
#endif

#if !__SAMD51__
#  error "SAMD library error, please check and update accordingly."
#endif

#include <cstring> //< std::memcpy
#include <algorithm> //< std::min, std:max

namespace BareCpper
{

    inline DeviceUid::DeviceUid()
        : IoDescriptor{
            [](IoDescriptor& descriptor, uint8_t* const buffer, const uint16_t bufferLength) ->int32_t
            {
                /** @see http://ww1.microchip.com/downloads/en/DeviceDoc/60001507E.pdf
                * Chapter: 9.6 Serial Number 
                * Page: 60
                */
                const uint32_t serialNumber[4] = { 
                      *(uint32_t*)0x008061FC, *(uint32_t*)0x00806010
                    , *(uint32_t*)0x00806014, *(uint32_t*)0x00806018 
                };
                std::memcpy(buffer, reinterpret_cast<const uint8_t*>(serialNumber), std::min(bufferLength, (uint16_t)sizeof(serialNumber) ) );

                return sizeof(serialNumber); //< Read result
            },  
            nullptr
        }
    {}

} //END: BareCpper