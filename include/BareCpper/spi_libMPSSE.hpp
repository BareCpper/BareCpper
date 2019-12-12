/*
 * Created: 02/07/2016 22:53:56
 *  Author: CraigH
 */
 /* barecpper/iom328p.h - definitions for ATmega328P. */

#ifndef BARECPPER_SPI_H_
#  error "Include <barecpper/spi.h> before this file."
#endif

/* Standard C libraries */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<vector>

/* OS specific libraries */
#ifdef _WIN32
#include<windows.h>
#endif

/* Include D2XX header*/
#include "ftd2xx.h"

/* Include libMPSSE header */
#include "libMPSSE_spi.h"
#include "ftdi_common.h"	/*Common across I2C, SPI, JTAG modules*/

/* Helper macros */
#define APP_CHECK_STATUS(exp) {if(exp!=FT_OK){printf("%s:%d:%s(): status(0x%x) \
!= FT_OK\n",__FILE__, __LINE__, __FUNCTION__,exp);exit(1);}else{;}};
#define CHECK_NULL(exp){if(exp==NULL){printf("%s:%d:%s():  NULL expression \
encountered \n",__FILE__, __LINE__, __FUNCTION__);exit(1);}else{;}};

/** @see  SPI_ToggleCS which has a 2ms sleep when state==false
*/
FT_STATUS SPI_ToggleCS_FAST(FT_HANDLE handle, bool state)
{
    /* For initial development only - assuming only ADBUS0 will be used for CS*/
    uint8 buffer[] = {
          MPSSE_CMD_SET_DATA_BITS_LOWBYTE
        , (TRUE == state) ? 0x00 : 0x08 //< State
        , 0x0B //direction
    };

    uint32 noOfBytesTransferred;
    return FT_Write(handle, buffer, sizeof(buffer), (DWORD*)&noOfBytesTransferred);
}


FT_STATUS SPI_Write_FAST(FT_HANDLE handle, uint8* buffer, uint32 sizeToTransfer, uint32* sizeTransferred, uint32 transferOptions)
{
    //SPI_CONFIG_OPTION_MODE0: = MPSSE_CMD_DATA_OUT_BYTES_NEG_EDGE;
    //SPI_CONFIG_OPTION_MODE1: = MPSSE_CMD_DATA_OUT_BYTES_POS_EDGE;
    //SPI_CONFIG_OPTION_MODE2: = MPSSE_CMD_DATA_OUT_BYTES_POS_EDGE;
    //SPI_CONFIG_OPTION_MODE3: = MPSSE_CMD_DATA_OUT_BYTES_NEG_EDGE;

    uint8 cmdBuffer[] = {
          MPSSE_CMD_DATA_OUT_BYTES_NEG_EDGE
        , (uint8)(sizeToTransfer - 1) /* length low byte */
        , (uint8)((sizeToTransfer - 1)>> 8) /* length high byte */
    };
    std::vector<uint8> fullCommandBuffer( sizeof(cmdBuffer) + sizeToTransfer );
    fullCommandBuffer.assign(std::begin(cmdBuffer), std::end(cmdBuffer));
    fullCommandBuffer.insert(fullCommandBuffer.end(), buffer, buffer + sizeToTransfer);
    
    /* write command and data*/
    uint32 noOfBytesTransferred;
    return FT_Write( handle, fullCommandBuffer.data(), fullCommandBuffer.size(), &noOfBytesTransferred );
}

namespace BareCpper
{
    #define SPI_DEVICE_BUFFER_SIZE		256

    static FT_HANDLE ftHandle; //< @TEMP
    static uint8 buffer[SPI_DEVICE_BUFFER_SIZE] = { 0 }; //< @TEMP

    typedef SPIConfig<0, 0, 0, 0, SPIDataMode_0, SPIBitOrder_MSBFirst> SPIConfig_MPSSE;

    struct SPI_Hardware;

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::initialise(uint8_t kPin_SS, uint8_t kPin_MISO, uint8_t kPin_MOSI, uint8_t kPin_SCLK)
    {
        const uint8 latency = 255; //< Maximum time before retry?

        ChannelConfig channelConf = {};
        channelConf.ClockRate = 30000000;// MAX_CLOCK_RATE;// / 2;
        channelConf.LatencyTimer = latency;
        channelConf.configOptions = SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_DBUS3 | SPI_CONFIG_OPTION_CS_ACTIVELOW;
        channelConf.Pin = 0x00000000;/*FinalVal-FinalDir-InitVal-InitDir (for dir 0=in, 1=out)*/

        /* init library */
#ifdef _MSC_VER
        Init_libMPSSE();
#endif

        uint32 channels = 0;
        FT_STATUS status = SPI_GetNumChannels(&channels);
        APP_CHECK_STATUS(status);

        printf("Number of available SPI channels = %d\n", (int)channels);

        if (channels == 0)
            return;

        for (uint8_t i = 0; i < channels; ++i)
        {
            FT_DEVICE_LIST_INFO_NODE devList = { 0 };
            status = SPI_GetChannelInfo(i, &devList);
            APP_CHECK_STATUS(status);
            printf("Information on channel number %d:\n", i);
            /* print the dev info */
            printf("		Flags=0x%x\n", devList.Flags);
            printf("		Type=0x%x\n", devList.Type);
            printf("		ID=0x%x\n", devList.ID);
            printf("		LocId=0x%x\n", devList.LocId);
            printf("		SerialNumber=%s\n", devList.SerialNumber);
            printf("		Description=%s\n", devList.Description);
            printf("		ftHandle=0x%x\n", (unsigned int)devList.ftHandle);/*is 0 unless open*/
        }

        /* Open the first available channel */
        const uint32_t cChannelToOpen = 0;
        status = SPI_OpenChannel(cChannelToOpen, &ftHandle);
        APP_CHECK_STATUS(status);
        printf("\nhandle=0x%x status=0x%x\n", (unsigned int)ftHandle, status);
        status = SPI_InitChannel(ftHandle, &channelConf);
        APP_CHECK_STATUS(status);
        
#if 0
#define DISABLE_CLOCK_DIVIDE			0x8A
        uint32 noOfBytesTransferred;
        uint8_t cmdDisableClockDivide[] = { DISABLE_CLOCK_DIVIDE }; //< No /5
        status = FT_Write(ftHandle, cmdDisableClockDivide, sizeof(cmdDisableClockDivide), &noOfBytesTransferred);
        APP_CHECK_STATUS(status);
        
#define MID_SET_CLOCK_FREQUENCY_CMD		0x86
        uint8_t cmdSetClockDivisor[] = { MID_SET_CLOCK_FREQUENCY_CMD, 0x0, 0x0 }; //< No divide  = 30MHz
        status = FT_Write(ftHandle, cmdSetClockDivisor, sizeof(cmdSetClockDivisor), &noOfBytesTransferred);
        APP_CHECK_STATUS(status);
#endif
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::destroy( /*uint8_t kPin_SS, uint8_t kPin_MISO, uint8_t kPin_MOSI, uint8_t kPin_SCLK*/)
    {

        FT_STATUS status = SPI_CloseChannel(ftHandle);

    #ifdef _MSC_VER
        Cleanup_libMPSSE();
    #endif
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::begin(uint8_t kPin_SS)
    {
        FT_STATUS status = SPI_ToggleCS_FAST(ftHandle, true);
        APP_CHECK_STATUS(status);

    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::end(uint8_t kPin_SS)
    {
        FT_STATUS status = SPI_ToggleCS_FAST(ftHandle, false);
        APP_CHECK_STATUS(status);
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::write(const uint8_t kPin_MOSI, const uint8_t kPin_SCLK, const uint8_t val)
    {
        uint32 sizeTransfered;
        FT_STATUS status = SPI_Write_FAST(ftHandle, const_cast<uint8_t*>(&val), 1, &sizeTransfered, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES );
        APP_CHECK_STATUS(status);
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::write(const uint8_t kPin_MOSI, const uint8_t kPin_SCLK, const uint8_t* values, const size_t valuesCount )
    {
        uint32 sizeTransfered;
        FT_STATUS status = SPI_Write_FAST(ftHandle, const_cast<uint8_t*>(values), valuesCount, &sizeTransfered, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES);
        APP_CHECK_STATUS(status);
    }

    template<>
    inline uint8_t SerialImpl<Serial_SPI, SPI_Hardware>::read(uint8_t kPin_MOSI, uint8_t kPin_SCLK)
    {
        uint8_t val;
        uint32 sizeTransfered;
        FT_STATUS status = SPI_Read(ftHandle, &val, 1, &sizeTransfered, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES );
        APP_CHECK_STATUS(status);
        return val;
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::read(uint8_t kPin_MOSI, uint8_t kPin_SCLK, uint8_t* values, const size_t valuesCount)
    {
        uint32 sizeTransfered;
        FT_STATUS  status = SPI_Read(ftHandle, &*values, valuesCount, &sizeTransfered, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE | SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
        APP_CHECK_STATUS(status);
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::setClockDivider(uint8_t rate)
    {
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::setDataMode(eSPIDataMode mode)
    {

    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::setBitOrder(eSPIBitOrder bitOrder)
    {
    }

} //END: BareCpper
