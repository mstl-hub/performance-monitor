# This is converter generated file, and shall not be touched by user
#
# It is always re-generated if converter script is called multiple times
# Use CMakeLists.txt to apply user changes
cmake_minimum_required(VERSION 3.22)

# MCU selection (choose one of: AT32F407VGT7, AT32F403ACGT7, AT32F403ACGU7)
if(NOT DEFINED MCU_TYPE)
  set(MCU_TYPE "AT32F407VGT7" CACHE STRING "Target MCU type")
  set_property(CACHE MCU_TYPE PROPERTY STRINGS AT32F407VGT7 AT32F403ACGT7 AT32F403ACGU7)
endif()

# Core MCU flags, CPU, instruction set and FPU setup
set(cpu_PARAMS ${cpu_PARAMS}
  -mthumb
  # -mcpu, -mfloat, -mfloat-abi config
  -mcpu=cortex-m4
  -mfloat-abi=hard
  -mfpu=fpv4-sp-d16
  # -mfloat-abi=soft
)

set(SHARED_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../TafcoMcuCore)

# Linker script
if("${MCU_TYPE}" STREQUAL "AT32F407VGT7")
  message(STATUS "Selected AT32F407VGT7")
  set(linker_script_SRC ${linker_script_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/../TafcoMcuCore/ldscripts/1000-main-app/AT32F407xG_FLASH.ld)
elseif("${MCU_TYPE}" STREQUAL "AT32F403ACGT7")
  message(STATUS "Selected AT32F403ACGT7")
  set(linker_script_SRC ${linker_script_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/../TafcoMcuCore/ldscripts/1000-main-app/AT32F403AxG_FLASH.ld)
elseif("${MCU_TYPE}" STREQUAL "AT32F403ACGU7")
  message(STATUS "Selected AT32F403ACGU7")
  message(STATUS "It's development board!")
  set(linker_script_SRC ${linker_script_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/../TafcoMcuCore/ldscripts/1000-main-app/AT32F403AxG_FLASH.ld)
else()
  message(FATAL_ERROR "Unsupported MCU_TYPE: ${MCU_TYPE}")
endif()

file(GLOB_RECURSE sources_SRCS
  "Library/Share/*.c*"
  "Library/Terminal/*.cpp"
  "Main/src/*.c*"
  "Library/Tasks/*.c*"
  "Library/Temperature/*.c*"
  "Library/CRC/*.c*"
  "Library/Command/*.c*"
  "Library/Config/*.c*"
  "Library/Modbus/*.c*"
  "Library/Modbus/rtu/*.c*"
  "Library/Modbus/port/*.c"
  "Library/ModbusApp/*.c*"
  "Library/Display/*.c*"
  "Library/Buttons/*.c*"
  "Library/UnitControlApp/*.c*"
  "Library/Usb/*.c*"
  "Library/Uart/*.c*"
  "Library/Voltage/*.c*"
  "Library/PBlock/*.c*"
  "Library/ADC/*.c*"
  "Library/GPIO/*.c*"
  "Library/Input/*.c*"
  "Library/Periphery/*.c*"
  "Library/CANopen/*.c*"

  # CANopenNode stack - core CANopen protocol implementation
  "Library/CANopenNode/CANopen.c"
  "Library/CANopenNode/301/*.c"
  "Library/CANopenNode/303/*.c"
  "Library/CANopenNode/305/*.c"
  "Library/CANopenNode/storage/CO_storage.c"  # Only base storage, not EEPROM

  Modbus:
  "Library/Modbus/*.c*"
  "Library/Modbus/rtu/*.c*"
  "Library/Modbus/port/*.c"
  "Library/ModbusApp/*.c*"
  "Library/Modbus/functions/*.c*"

  "${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/device_support/*.c"
  "${SHARED_LIB_PATH}/AT32F403A/drivers/src/*.c"
  "${SHARED_LIB_PATH}/AT32F403A/startup/startup_at32f403a_407.s"
  "${SHARED_LIB_PATH}/Library/ArteryCore/*.c*"
  "${SHARED_LIB_PATH}/Library/ArteryCore/Periphery/*.c*"
  "${SHARED_LIB_PATH}/Library/ArteryCore/McuClock/*.c*"
  "${SHARED_LIB_PATH}/Library/ArteryCore/SystemApi/*.c*"
  "${SHARED_LIB_PATH}/Library/ArteryCore/CAN/*.c*"
  "${SHARED_LIB_PATH}/Library/Command/*.c*"
  "${SHARED_LIB_PATH}/Library/CRC/*.c*"
  "${SHARED_LIB_PATH}/Library/Terminal/*.c*"
  "${SHARED_LIB_PATH}/Library/Usb/*.c*"
  "${SHARED_LIB_PATH}/Library/UnitControl/*.c*"
  "${SHARED_LIB_PATH}/Library/Logging/*.c*"
  "${SHARED_LIB_PATH}/Library/RTC/*.c*"
  "${SHARED_LIB_PATH}/Library/Entities/*.c*"
  "${SHARED_LIB_PATH}/Library/Shared/*.c*"
  "${SHARED_LIB_PATH}/Library/Uart/*.c*"
  "${SHARED_LIB_PATH}/Library/Timer/*.c*"
  "${SHARED_LIB_PATH}/Library/BprService/*.c*"
  "${SHARED_LIB_PATH}/Library/DelayService/*.c*"
  "${SHARED_LIB_PATH}/Library/FlashService/*.c*"
  "${SHARED_LIB_PATH}/Library/UpTime/*.c*"
  "${SHARED_LIB_PATH}/Library/TafcoTinyProtocol/*.c*"
  "${SHARED_LIB_PATH}/Library/BigBeautifulStaticArray/*.c*"
  "${SHARED_LIB_PATH}/Library/Authorize/*.c*"
  "${SHARED_LIB_PATH}/Library/Bootload/*.c*"
  "${SHARED_LIB_PATH}/Library/FW_Loader/*.c*"
  "${SHARED_LIB_PATH}/Library/TafcoCrypt/*.c*"
  "${SHARED_LIB_PATH}/Library/ArteryCore/WatchDog/SimpleWatchDog/*.c*"
  "${SHARED_LIB_PATH}/Library/ArteryCore/WatchDog/HealthCheckWdt/*.c*"
  "${SHARED_LIB_PATH}/Library/ArteryCore/WatchDog/WatchDog/*.c*"
  "${SHARED_LIB_PATH}/Library/ErrorJournal/*.c*"
  "${SHARED_LIB_PATH}/Library/Versions/*.c*"
  "${SHARED_LIB_PATH}/Library/McuClock/*.c*"
  "${SHARED_LIB_PATH}/Library/MS5614T-Api/*.c*"
  # Note: CANopenNode sources are included from P-block/Library/CANopenNode/ (see above)

  #   "Middlewares/FreeRTOS10/source/*.c"
  #   "Middlewares/FreeRTOS10/source/portable/common/*.c"
  #   "Middlewares/FreeRTOS10/source/portable/memmang/*.c"
  #   "Middlewares/FreeRTOS10/source/portable/ARM_CM4F/*.c"

  # usb_driver:
  "${SHARED_LIB_PATH}/AT32F403A/Middlewares/USB/usbd_drivers/src/*.c"
  # usb_class cdc:
  "${SHARED_LIB_PATH}/AT32F403A/Middlewares/USB/usbd_class/cdc/*.c"
)


# Include directories
set(include_c_DIRS ${include_c_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Share
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Terminal
  ${CMAKE_CURRENT_SOURCE_DIR}/Main/inc
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Tasks
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/CRC
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Uart
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Temperature
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Command
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Config
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/PBlock
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Modbus
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Modbus/include
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Modbus/port
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/ModbusApp
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Display
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Buttons
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/UnitControlApp
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Usb
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Uart
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/ADC
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/GPIO
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Input
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Periphery
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/CANopen

  # CANopenNode stack includes
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/CANopenNode
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/CANopenNode/301
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/CANopenNode/303
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/CANopenNode/305
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/CANopenNode/storage

  Modbus:
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Modbus
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Modbus/include
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Modbus/port
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/ModbusApp
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Modbus/rtu

  ${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/core_support
  ${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/device_support
  ${SHARED_LIB_PATH}/AT32F403A/drivers/inc
  ${SHARED_LIB_PATH}/Library/ArteryCore
  ${SHARED_LIB_PATH}/Library/ArteryCore/Periphery
  ${SHARED_LIB_PATH}/Library/ArteryCore/McuClock
  ${SHARED_LIB_PATH}/Library/ArteryCore/BprDriver
  ${SHARED_LIB_PATH}/Library/ArteryCore/SystemApi
  ${SHARED_LIB_PATH}/Library/ArteryCore/CAN
  ${SHARED_LIB_PATH}/Library/Command
  ${SHARED_LIB_PATH}/Library/CRC
  ${SHARED_LIB_PATH}/Library/Macros
  ${SHARED_LIB_PATH}/Library/Terminal
  ${SHARED_LIB_PATH}/Library/Usb
  ${SHARED_LIB_PATH}/Library/UnitControl
  ${SHARED_LIB_PATH}/Library/Logging
  ${SHARED_LIB_PATH}/Library/RTC
  ${SHARED_LIB_PATH}/Library/Entities
  ${SHARED_LIB_PATH}/Library/Shared
  ${SHARED_LIB_PATH}/Library/Uart
  ${SHARED_LIB_PATH}/Library/Timer
  ${SHARED_LIB_PATH}/Library/DelayService
  ${SHARED_LIB_PATH}/Library/FlashService
  ${SHARED_LIB_PATH}/Library/UpTime
  ${SHARED_LIB_PATH}/Library/TafcoTinyProtocol
  ${SHARED_LIB_PATH}/Library/BigBeautifulStaticArray
  ${SHARED_LIB_PATH}/Library/Authorize
  ${SHARED_LIB_PATH}/Library/Bootload
  ${SHARED_LIB_PATH}/Library/FW_Loader
  ${SHARED_LIB_PATH}/Library/TafcoCrypt
  ${SHARED_LIB_PATH}/Library/ArteryCore/WatchDog/SimpleWatchDog
  ${SHARED_LIB_PATH}/Library/ArteryCore/WatchDog/HealthCheckWdt
  ${SHARED_LIB_PATH}/Library/ArteryCore/WatchDog/WatchDog
  ${SHARED_LIB_PATH}/Library/ErrorJournal
  ${SHARED_LIB_PATH}/Library/Versions
  ${SHARED_LIB_PATH}/Library/McuClock
  ${SHARED_LIB_PATH}/Library/MS5614T-Api
  # Note: CANopenNode includes are from P-block/Library/CANopenNode/ (see above)

  usb_driver:
  ${SHARED_LIB_PATH}/AT32F403A/Middlewares/USB/usbd_drivers/inc
  usb_class cdc:
  ${SHARED_LIB_PATH}/AT32F403A/Middlewares/USB/usbd_class/cdc
)
set(include_cxx_DIRS ${include_cxx_DIRS} ${include_c_DIRS})
set(include_asm_DIRS ${include_asm_DIRS})

# Symbols definition
set(symbols_c_SYMB ${symbols_c_SYMB}
  "DEBUG"
  "USE_STDPERIPH_DRIVER"
)

# MCU-specific preprocessor symbol
if("${MCU_TYPE}" STREQUAL "AT32F407VGT7")
  list(APPEND symbols_c_SYMB "AT32F407VGT7")
elseif("${MCU_TYPE}" STREQUAL "AT32F403ACGT7")
  list(APPEND symbols_c_SYMB "AT32F403ACGT7")
elseif("${MCU_TYPE}" STREQUAL "AT32F403ACGU7")
  list(APPEND symbols_c_SYMB "AT32F403ACGU7")
endif()
set(symbols_cxx_SYMB ${symbols_cxx_SYMB} ${symbols_c_SYMB})
set(symbols_asm_SYMB ${symbols_asm_SYMB}
  "DEBUG"
)

# Link directories
set(link_DIRS ${link_DIRS})

# Link libraries
set(link_LIBS ${link_LIBS})

# Compiler options
set(compiler_OPTS ${compiler_OPTS})

# Linker options
set(linker_OPTS ${linker_OPTS})
