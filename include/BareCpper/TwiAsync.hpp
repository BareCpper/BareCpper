#ifndef BARECPPER_TWIASYNC_HPP_
#define BARECPPER_TWIASYNC_HPP_

#include "Io.hpp"

namespace BareCpper
{

#if 0 //< @todo
    class TwiAsync
    {
    public:

        IoDescriptor io();



    };
#endif

} //END: BareCpper

#if !DOXYGEN ///< Platform specific interface definitions
#if __SAMD51__
#include "ATsamd/TwiAsync_ATsamd5x.hpp"
#endif
#endif

#endif