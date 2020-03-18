
#ifndef BARECPPER_SPIDMA_HPP_
#  error "Include <BareCpper/SpiDma.hpp> instead of this file."
#endif

namespace BareCpper
{

    inline IoDescriptor SpiDma::io()
    {
        return {
            [](const IoDescriptor& descriptor, uint8_t* const buffer, const uint16_t bufferLength) ->int32_t
            {
                return 0; //< Read result
            },
            [](const IoDescriptor& descriptor, const uint8_t* const buffer, const uint16_t bufferLength) ->int32_t
            {
                return 0; //< Write result
            }
        };
    }

} //END: BareCpper