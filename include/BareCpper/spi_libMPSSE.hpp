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

/* OS specific libraries */
#ifdef _WIN32
#include<windows.h>
#endif

/* Include D2XX header*/
#include "ftd2xx.h"

/* Include libMPSSE header */
#include "libMPSSE_spi.h"

/* Helper macros */
#define APP_CHECK_STATUS(exp) {if(exp!=FT_OK){printf("%s:%d:%s(): status(0x%x) \
!= FT_OK\n",__FILE__, __LINE__, __FUNCTION__,exp);exit(1);}else{;}};
#define CHECK_NULL(exp){if(exp==NULL){printf("%s:%d:%s():  NULL expression \
encountered \n",__FILE__, __LINE__, __FUNCTION__);exit(1);}else{;}};

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
        uint8 latency = 255; //< @ what?

        ChannelConfig channelConf = {};
        channelConf.ClockRate = 5000;
        channelConf.LatencyTimer = latency;
        channelConf.configOptions = SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_DBUS3;// | SPI_CONFIG_OPTION_CS_ACTIVELOW;
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

    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::end(uint8_t kPin_SS)
    {
    }

    template<>
    inline void SerialImpl<Serial_SPI, SPI_Hardware>::write(uint8_t kPin_MOSI, uint8_t kPin_SCLK, uint8_t val)
    {
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
