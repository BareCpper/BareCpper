#ifndef BARECPPER_SPIDMA_HPP_
#define BARECPPER_SPIDMA_HPP_

#include "Io.hpp"

namespace BareCpper
{

    class SpiDma
    {
    public:

        IoDescriptor io()
        {
            return {
                [](IoDescriptor& descriptor, const uint8_t* const buffer, const uint16_t bufferLength) ->int32_t
                {
                    return 0; //< Read result
                },
                [](IoDescriptor& descriptor, uint8_t* const buffer, const uint16_t bufferLength) ->int32_t
                {
                    return 0; //< Write result
                }
            };
        }




    };

} //END: BareCpper

#if !DOXYGEN ///< Platform specific interface definitions
    #if __SAMD51__
     #include "ATsamd/SpiDma_ATsamd5x.hpp"
    #endif
#endif

#endif