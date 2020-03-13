#ifndef BARECPPER_IO_HPP
#define BARECPPER_IO_HPP


namespace BareCpper
{
    // Forward declaration
    struct IoDescriptor;

    /** IO read function prototype
     */
    typedef int32_t (*ReadFn) (IoDescriptor& descriptor, uint8_t* const buffer, const uint16_t bufferLength);
    
    /** IO write function prototype
     */
    typedef int32_t (*WriteFn) (IoDescriptor& descriptor, const uint8_t* const buffer, const uint16_t bufferLength);

    struct IoDescriptor {
        ReadFn  read; ///< The read function pointer
        WriteFn write; ///< The write function pointer
    };

    /** Read from IO device
    */
    inline int32_t read(IoDescriptor& descriptor, uint8_t* const buffer, const uint16_t bufferLength) 
    {
        ASSERT(buffer);
        return descriptor.read(descriptor, buffer, bufferLength);
    }

    /** Write to IO device 
    */
    inline int32_t write( IoDescriptor& descriptor, const uint8_t* const buffer, const uint16_t bufferLength)
    {
        ASSERT(buffer);
        return descriptor.write(descriptor, buffer, bufferLength);
    }

} //END: BareCpper

#endif