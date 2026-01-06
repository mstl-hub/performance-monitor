# This is converter generated file, and shall not be touched by user
#
# It is always re-generated if converter script is called multiple times
# Use CMakeLists.txt to apply user changes
cmake_minimum_required(VERSION 3.22)

# MCU selection (choose one of: AT32F407VGT7, AT32F403ACGT7, AT32F403ACGU7)
if(NOT DEFINED MCU_TYPE)
  set(MCU_TYPE "AT32F403ACGT7" CACHE STRING "Target MCU type")
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

# Linker script
if("${MCU_TYPE}" STREQUAL "AT32F407VGT7")
  message(STATUS "Selected AT32F407VGT7")
  set(linker_script_SRC ${linker_script_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/../TafcoMcuCore/ldscripts/400-boot/AT32F407xG_FLASH.ld)
elseif("${MCU_TYPE}" STREQUAL "AT32F403ACGT7")
  message(STATUS "Selected AT32F403ACGT7")
  set(linker_script_SRC ${linker_script_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/../TafcoMcuCore/ldscripts/400-boot/AT32F403AxG_FLASH.ld)
elseif("${MCU_TYPE}" STREQUAL "AT32F403ACGU7")
  message(STATUS "Selected AT32F403ACGU7")
  message(STATUS "It's development board!")
  set(linker_script_SRC ${linker_script_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/../TafcoMcuCore/ldscripts/400-boot/AT32F403AxG_FLASH.ld)
else()
  message(FATAL_ERROR "Unsupported MCU_TYPE: ${MCU_TYPE}")
endif()

file(GLOB_RECURSE sources_SRCS
  "Main/src/*.c*"
  "Library/Command/*.c*"

  "${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/device_support/*.c"
  "${SHARED_LIB_PATH}/AT32F403A/drivers/src/*.c"
  "${SHARED_LIB_PATH}/AT32F403A/startup/startup_at32f403a_407.s"
  "${SHARED_LIB_PATH}/Library/ArteryCore/*.c*"
  "${SHARED_LIB_PATH}/Library/ArteryCore/Periphery/*.c*"
  "${SHARED_LIB_PATH}/Library/ArteryCore/McuClock/*.c*"
  "${SHARED_LIB_PATH}/Library/Command/*.c*"
  "${SHARED_LIB_PATH}/Library/CRC/*.c*"
  "${SHARED_LIB_PATH}/Library/Terminal/*.c*"
  "${SHARED_LIB_PATH}/Library/ArteryCore/WatchDog/SimpleWatchDog/*.c*"
  "${SHARED_LIB_PATH}/Library/ErrorJournal/*.c*"
  "${SHARED_LIB_PATH}/Library/Usb/*.c*"
  "${SHARED_LIB_PATH}/Library/TafcoCrypt/*.c*"
  "${SHARED_LIB_PATH}/Library/RTC/*.c*"
  "${SHARED_LIB_PATH}/Library/UnitControl/*.c*"
  "${SHARED_LIB_PATH}/Library/Shared/*.c*"
  "${SHARED_LIB_PATH}/Library/Uart/*.c*"
  "${SHARED_LIB_PATH}/Library/BprService/*.c*"
  "${SHARED_LIB_PATH}/Library/UpTime/*.c*"
  "${SHARED_LIB_PATH}/Library/FlashService/*.c*"
  "${SHARED_LIB_PATH}/Library/McuClock/*.c*"
  "${SHARED_LIB_PATH}/Library/DelayService/*.c*"
  "${SHARED_LIB_PATH}/Library/Bootload/*.c*"
  "${SHARED_LIB_PATH}/Library/FW_Loader/*.c*"
  "${SHARED_LIB_PATH}/Library/Versions/*.c*"

  # usb_driver:
  "${SHARED_LIB_PATH}/Middlewares/USB/usbd_drivers/src/*.c"
  # usb_class win usb:
  "${SHARED_LIB_PATH}/Middlewares/USB/usbd_class/winusb/*.c"
  # usb_class cdc:
  "${SHARED_LIB_PATH}/Middlewares/USB/usbd_class/cdc/*.c"
)


# Include directories
set(include_c_DIRS ${include_c_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/Main/inc
  ${CMAKE_CURRENT_SOURCE_DIR}/Library/Command


  ${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/core_support
  ${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/device_support
  ${SHARED_LIB_PATH}/AT32F403A/drivers/inc
  ${SHARED_LIB_PATH}/Library/ArteryCore
  ${SHARED_LIB_PATH}/Library/ArteryCore/Periphery
  ${SHARED_LIB_PATH}/Library/ArteryCore/McuClock
  ${SHARED_LIB_PATH}/Library/Command
  ${SHARED_LIB_PATH}/Library
  ${SHARED_LIB_PATH}/Library/Macros
  ${SHARED_LIB_PATH}/Library/CRC
  ${SHARED_LIB_PATH}/Library/Terminal
  ${SHARED_LIB_PATH}/Library/Logging
  ${SHARED_LIB_PATH}/Library/ArteryCore/BprDriver
  ${SHARED_LIB_PATH}/Library/ArteryCore/Rtc
  ${SHARED_LIB_PATH}/Library/ArteryCore/WatchDog/SimpleWatchDog
  ${SHARED_LIB_PATH}/Library/ArteryCore/WatchDog/HealthCheckWdt
  ${SHARED_LIB_PATH}/Library/ArteryCore/WatchDog/WatchDog
  ${SHARED_LIB_PATH}/Library/ErrorJournal
  ${SHARED_LIB_PATH}/Library/Usb
  ${SHARED_LIB_PATH}/Library/TafcoCrypt
  ${SHARED_LIB_PATH}/Library/RTC
  ${SHARED_LIB_PATH}/Library/UnitControl
  ${SHARED_LIB_PATH}/Library/Lora
  ${SHARED_LIB_PATH}/Library/Shared
  ${SHARED_LIB_PATH}/Library/Uart
  ${SHARED_LIB_PATH}/Library/UpTime
  ${SHARED_LIB_PATH}/Library/FlashService
  ${SHARED_LIB_PATH}/Library/McuClock
  ${SHARED_LIB_PATH}/Library/DelayService
  ${SHARED_LIB_PATH}/Library/sx126x_driver/src
  ${SHARED_LIB_PATH}/Library/Bootload
  ${SHARED_LIB_PATH}/Library/FW_Loader
  ${SHARED_LIB_PATH}/Library/Versions

  usb_driver:
  ${SHARED_LIB_PATH}/Middlewares/USB/usbd_drivers/inc
  usb_class winusb:
  ${SHARED_LIB_PATH}/Middlewares/USB/usbd_class/winusb
  usb_class cdc:
  ${SHARED_LIB_PATH}/Middlewares/USB/usbd_class/cdc
)
set(include_cxx_DIRS ${include_cxx_DIRS} ${include_c_DIRS})
set(include_asm_DIRS ${include_asm_DIRS})

# Symbols definition
set(symbols_c_SYMB ${symbols_c_SYMB}
  # "DEBUG"
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
# set(symbols_asm_SYMB ${symbols_asm_SYMB}
#   "DEBUG"
# )

# Link directories
set(link_DIRS ${link_DIRS})

# Link libraries
set(link_LIBS ${link_LIBS})

# Compiler options
set(compiler_OPTS ${compiler_OPTS})

# Linker options
set(linker_OPTS ${linker_OPTS})
