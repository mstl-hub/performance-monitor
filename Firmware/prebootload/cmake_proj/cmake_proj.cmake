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
  set(linker_script_SRC ${linker_script_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/../TafcoMcuCore/ldscripts/0-preboot/AT32F407xG_FLASH.ld)
elseif("${MCU_TYPE}" STREQUAL "AT32F403ACGT7")
  message(STATUS "Selected AT32F403ACGT7")
  set(linker_script_SRC ${linker_script_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/../TafcoMcuCore/ldscripts/0-preboot/AT32F403AxG_FLASH.ld)
elseif("${MCU_TYPE}" STREQUAL "AT32F403ACGU7")
  message(STATUS "Selected AT32F403ACGU7")
  message(STATUS "It's development board!")
  set(linker_script_SRC ${linker_script_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/../TafcoMcuCore/ldscripts/0-preboot/AT32F403AxG_FLASH.ld)
else()
  message(FATAL_ERROR "Unsupported MCU_TYPE: ${MCU_TYPE}")
endif()

file(GLOB_RECURSE sources_SRCS
  "Main/src/*.c*"

  "${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/device_support/*.c"
  "${SHARED_LIB_PATH}/AT32F403A/drivers/src/*.c"
  "${SHARED_LIB_PATH}/AT32F403A/startup/startup_at32f403a_407.s"
  "${SHARED_LIB_PATH}/Library/ArteryCore/McuClock/*.c*"
  "${SHARED_LIB_PATH}/Library/ArteryCore/BprDriver/*.c*"
  "${SHARED_LIB_PATH}/Library/DelayService/*.c*"
  "${SHARED_LIB_PATH}/Library/Bootload/*.c*"
  "${SHARED_LIB_PATH}/Library/ArteryCore/WatchDog/WatchDog/*.c*"
  "${SHARED_LIB_PATH}/Library/ArteryCore/WatchDog/SimpleWatchDog/*.c*"
)


# Include directories
set(include_c_DIRS ${include_c_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/Main/inc

  ${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/core_support
  ${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/device_support
  ${SHARED_LIB_PATH}/AT32F403A/drivers/inc
  ${SHARED_LIB_PATH}/Library/Macros
  ${SHARED_LIB_PATH}/Library/ArteryCore/McuClock
  ${SHARED_LIB_PATH}/Library/ArteryCore/BprDriver
  ${SHARED_LIB_PATH}/Library/DelayService
  ${SHARED_LIB_PATH}/Library/Bootload
  ${SHARED_LIB_PATH}/Library/ArteryCore/WatchDog/WatchDog
  ${SHARED_LIB_PATH}/Library/ArteryCore/WatchDog/SimpleWatchDog
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
