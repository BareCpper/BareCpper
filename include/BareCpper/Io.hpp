#ifndef BARECPPER_IO_HPP
#define BARECPPER_IO_HPP

#include <cassert> //< assert

namespace BareCpper
{
    // Forward declaration
    struct IoDescriptor;

    /** IO read function prototype
     * @return Bytes read. 
     * @note If Bytes-read > bufferLength this indicates insufficient buffer storage was given to complete a read of fixed-size.
     */
    typedef int32_t (*ReadFn) (const IoDescriptor& descriptor, uint8_t* const buffer, const uint16_t bufferLength);
    
    /** IO write function prototype
     */
    typedef int32_t (*WriteFn) (const IoDescriptor& descriptor, const uint8_t* const buffer, const uint16_t bufferLength);

    struct IoDescriptor {
        ReadFn  read; ///< The read function pointer
        WriteFn write; ///< The write function pointer
    };

    /** Read from IO device
    */
    inline int32_t read( const IoDescriptor& descriptor, uint8_t* const buffer, const uint16_t bufferLength) 
    {
        assert(buffer);
        return descriptor.read(descriptor, buffer, bufferLength);
    }

    /** Write to IO device 
    */
    inline int32_t write(const IoDescriptor& descriptor, const uint8_t* const buffer, const uint16_t bufferLength)
    {
        assert(buffer);
        return descriptor.write(descriptor, buffer, bufferLength);
    }

} //END: BareCpper

#endif