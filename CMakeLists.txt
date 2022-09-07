cmake_minimum_required(VERSION 3.15)

# Only supported for target cross-compiled build
if ( NOT CMAKE_CROSSCOMPILING )
    return()
endif()

project( BareCpper )

add_library( BareCpper STATIC )

target_include_directories( BareCpper
  PUBLIC
	  ${CMAKE_CURRENT_LIST_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
)

target_sources( BareCpper
  PUBLIC
    "include/BareCpper/AnalogToDigitalConverter.hpp"
    "include/BareCpper/assert.h"
    #"include/BareCpper/BareCpper.cpp"
    #"include/BareCpper/BareCpper.hpp"
    #"include/BareCpper/CharacterSet_8x5.cpp"
    #"include/BareCpper/CharacterSet_8x5.h"
    "include/BareCpper/Chrono.hpp"
    "include/BareCpper/CodeProfiler.hpp"
    "include/BareCpper/Common.hpp"
    "include/BareCpper/CycleCounter.hpp"
    "include/BareCpper/DateTime.hpp"
    "include/BareCpper/Delay.hpp"
    "include/BareCpper/DeviceUid.hpp"
    "include/BareCpper/fastmath.h"
    "include/BareCpper/FifoQueue.hpp"
    "include/BareCpper/gpio.hpp"
    "include/BareCpper/GpioLegacy.hpp"
    "include/BareCpper/Io.hpp"
    "include/BareCpper/MonotonicClock.hpp"
    "include/BareCpper/MonotonicCounter.hpp"
    "include/BareCpper/Queue.hpp"
    "include/BareCpper/RealtimeClock.hpp"
    "include/BareCpper/RealtimeCounter.hpp"
    "include/BareCpper/spi.hpp"
    "include/BareCpper/SpiDma.hpp"
    "include/BareCpper/SpiLegacy.hpp"
    "include/BareCpper/StdChronoExt.hpp"
    "include/BareCpper/stream.h"
    "include/BareCpper/Sub0Std.hpp"
    "include/BareCpper/Swi.hpp"
    "include/BareCpper/TemperatureSensor.hpp"
    "include/BareCpper/TimerCounter.hpp"
    "include/BareCpper/TwiAsync.hpp"
    "include/BareCpper/Arduino/Spi_ArduinoSW.hpp"
    "include/BareCpper/Arduino/Spi_Ardunio.hpp"
    "include/BareCpper/ARM/CycleCounter_ARMm4.hpp"
    "include/BareCpper/ATmega/Gpio_ATmega328P.hpp"
    "include/BareCpper/ATmega/serial.h"
    "include/BareCpper/ATmega/Spi_ATmega328P.hpp"
    "include/BareCpper/ATmega/Spi_SoftDevice.hpp"
    "include/BareCpper/ATsamd/AnalogToDigitalConverter_ATsamd5x.hpp"
    "include/BareCpper/ATsamd/Common_ATsamd5x.hpp"
    "include/BareCpper/ATsamd/CycleCounter_ATsamd5x.hpp"
    "include/BareCpper/ATsamd/dbg_temp.h"
    "include/BareCpper/ATsamd/DeviceUid_ATsamd5x.hpp"
    "include/BareCpper/ATsamd/Dmac_ATsamd5x.hpp"
    "include/BareCpper/ATsamd/EventSystem_ATsamd5x.hpp"
    "include/BareCpper/ATsamd/Gpio_ATsam5x.hpp"
    "include/BareCpper/ATsamd/MonotonicCounter_ATsamd5x.hpp"
    "include/BareCpper/ATsamd/Sercom_ATsamd5x.hpp"
    "include/BareCpper/ATsamd/Spi_ATsamd5x.hpp"
    "include/BareCpper/ATsamd/Spi_SoftDevice.hpp"
    "include/BareCpper/ATsamd/Swi_SoftDevice.hpp"
    "include/BareCpper/ATsamd/TemperatureSensor_ATsamd5x.hpp"
    "include/BareCpper/ATsamd/TimerCounter_ATsamd5x.hpp"
    "include/BareCpper/ATsamd/TimerCounterControl_ATsamd5x.hpp"
    "include/BareCpper/ATsamd/TwiAsync_ATsamd5x.hpp"
    "include/BareCpper/ATsamd/Watchdog_ATsamd5x.hpp"
    "include/BareCpper/ATtiny/Spi_SoftDevice.hpp"
    "include/BareCpper/Ftdi/Spi_libMPSSE.hpp"
    "include/BareCpper/internal/Spi-inl.hpp"
    "include/BareCpper/nRF52/CycleCounter_nRF52.hpp"
    "include/BareCpper/nRF52/DeviceUid_nRF52.hpp"
    "include/BareCpper/nRF52/Gpio_nRF52832.hpp"
    "include/BareCpper/nRF52/MonotonicCounter_nRF52.hpp"
    "include/BareCpper/nRF52/Uart_nRF52.hpp"
    "include/BareCpper/nRF52/UartDma_nRF52.hpp"
    #"include/BareCpper/Philips/LcdDisplay_PCD8544.cpp"
    #"include/BareCpper/Philips/LcdDisplay_PCD8544.hpp"
    #"include/BareCpper/Philips/LcdPanel_PCD8544.cpp"
    #"include/BareCpper/Philips/LcdPanel_PCD8544.hpp"
    #"include/BareCpper/Philips/PCD8544.hpp"
    "include/BareCpper/SoftDevice/Spi_SoftDevice.hpp"
  PRIVATE
    "src/BareCpper/ATsamd/AnalogToDigitalConverter_ATsamd5x.cpp"
    "src/BareCpper/ATsamd/Dmac_ATsamd5x.cpp"
    "src/BareCpper/ATsamd/EventSystem_ATsamd5x.cpp"
    "src/BareCpper/ATsamd/ExternalInterruptController_ATsamd5x.cpp"
    "src/BareCpper/ATsamd/TimerCounter_ATsamd5x.cpp"
    "src/BareCpper/ATsamd/TimerCounterControl_ATsamd5x.cpp"
    "src/BareCpper/ATsamd/TemperatureSensor_ATsamd5x.cpp"
    "src/BareCpper/ATsamd/TwiAsync_ATsamd5x.cpp"
    "src/BareCpper/ATsamd/Watchdog_ATsamd5x.cpp"
)

target_compile_features( BareCpper 
  PRIVATE
    cxx_constexpr
    cxx_std_14
    cxx_variadic_templates
)

# link BareCpper before you are using it with your CSP/BSP

add_subdirectory(examples)