cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Core project settings
project(PigStore-PBlock)
enable_language(C CXX ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Select MCU:
set(MCU_TYPE "AT32F407VGT7") # AT32F403ACGU7 AT32F403ACGT7 AT32F407VGT7
set(SHARED_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../TafcoMcuCore)

if("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
    message(STATUS "RELEASE_opt_1")
    add_compile_options(-O1)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
    message(STATUS "Maximum DEBUG")
    add_compile_options(-Og -g3)
endif()

# Core MCU flags, CPU type, instruction set and FPU setup
set(cpu_PARAMS)

# Sources
set(sources_SRCS)

# Include directories for all compilers
set(include_DIRS)

# Include directories for each compiler
set(include_c_DIRS)
set(include_cxx_DIRS)
set(include_asm_DIRS)

# Symbols definition for all compilers
set(symbols_SYMB)

# Symbols definition for each compiler
set(symbols_c_SYMB)
set(symbols_cxx_SYMB)
set(symbols_asm_SYMB)

# Link directories and names of libraries
set(link_DIRS)
set(link_LIBS)

# Linker script
set(linker_script_SRC)

# Compiler options
set(compiler_OPTS)

# Linker options
set(linker_OPTS)

# Now call generated cmake
# This will add script generated
# information to the project
include("cmake_proj/cmake_proj.cmake")
include("${SHARED_LIB_PATH}/Library/CayenneLPP/lpp.cmake")

# FreeRTOS ON ****************************************************************************

#include("cmake_proj/cmake_freertos.cmake")

# ****************************************************************************************

# Link directories setup
# Must be before executable is added
link_directories(${CMAKE_PROJECT_NAME} ${link_DIRS})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${sources_SRCS})


# *************************************************************************************************************************************************
# **************************************************************** FreeRTOS lib *******************************************************************
# *************************************************************************************************************************************************


add_definitions(
    -DFREE_RTOS_IS_IN_USED # !Define
)


add_definitions(
    # -D__ARM_FP
    -DSMTC_MULTICAST # !Define
    -DSX126X # !Define
    -DADD_CLASS_B # !Define
    -DADD_CLASS_C # !Define
    -DREGION_RU_864 # !Define
    -DRELAY_TX # !Define
    -DADD_ALMANAC # !Define
    -DRP2_103
    -DADD_CSMA
    -DENABLE_CSMA_BY_DEFAULT
    # -DADD_STANDBY_MODE #DADD_DEEPSLEEP_MODE DADD_STANDBY_MODE
    -DDEBUG_PRINT_USB
    -DGLOB_USE_RTT
    # -DDEBUG_PRINT_UART
    # -DLORA_SNIFFER
)

# set(GLOB_Definition_symb
#     "-DGLOB_USE_RTT")

if("${MCU_TYPE}" STREQUAL "AT32F403ACGU7")
    add_definitions(
        -DDEVELOPMENT_BOARD
        # -DDEBUG_PRINT_UART
    )
else()
    add_definitions(

    )
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
    add_definitions(
        -DUSE_WDT
    )
endif()

add_definitions(-DNUMBER_OF_STACKS=1)

#isable FreeRTOS
set(${FREERTOS_CONFIG_FILE_DIRECTORY} "Middlewares/FreeRTOS/Config")

# Add the freertos_config for FreeRTOS-Kernel
add_library(freertos_config INTERFACE "Middlewares/FreeRTOS/Config/FreeRTOSConfig.h") # INTERFACE
target_include_directories(freertos_config
    INTERFACE
    "Middlewares/FreeRTOS/Config"
)

#Select the heap port.  values between 1-4 will pick a heap.
set(FREERTOS_HEAP "1" CACHE STRING "" FORCE)

# Select the native compile PORT
set(FREERTOS_PORT "GCC_ARM_CM4F" CACHE STRING "" FORCE)

# Adding the FreeRTOS-Kernel subdirectory
add_subdirectory(${FREERTOS_KERNEL_PATH} Middlewares/FreeRTOS/Kernel)

# !Integrate options to FreeRTOS
target_compile_options(freertos_kernel PRIVATE
    ${cpu_PARAMS}
    ${compiler_OPTS}
    ### Gnu/Clang C Options
    $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>

    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wpedantic>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Werror>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wconversion>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Weverything>

    # Suppressions required to build clean with clang.
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-unused-macros>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-padded>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-missing-variable-declarations>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-covered-switch-default>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-cast-align>

    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp -MMD -MP>
)

# !Integrate options to FreeRTOS
target_compile_options(freertos_kernel_port PRIVATE
    ${cpu_PARAMS}
    ${compiler_OPTS}
    ### Gnu/Clang C Options
    $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>

    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wpedantic>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Werror>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wconversion>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Weverything>

    # Suppressions required to build clean with clang.
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-unused-macros>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-padded>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-missing-variable-declarations>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-covered-switch-default>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-cast-align>

    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp -MMD -MP>
)

target_link_libraries(${CMAKE_PROJECT_NAME} freertos_kernel freertos_config)

if(GLOB_Definition_symb MATCHES "-DGLOB_USE_RTT")
    message("USE RTT")
    set(RTT_COMPILER "GCC")
    add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../TafcoMcuCore/Middlewares/SeggerRTT
        ${CMAKE_CURRENT_BINARY_DIR}/TafcoMcuCore/Middlewares/SeggerRTT
        )
    target_compile_options(LSeggerRTT PRIVATE
        ${cpu_PARAMS}
        ${compiler_OPTS}
        ### Gnu/Clang C Options
        $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>

        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wpedantic>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Werror>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wconversion>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-Weverything>

        # Suppressions required to build clean with clang.
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-unused-macros>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-padded>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-missing-variable-declarations>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-covered-switch-default>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-cast-align>

        $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp -MMD -MP>
    )
    target_link_libraries(${CMAKE_PROJECT_NAME} LSeggerRTT)
endif()

# *************************************************************************************************************************************************
# *************************************************************************************************************************************************
# *************************************************************************************************************************************************

# *************************************************************************************************************************************************
# *************************************************************** USB *****************************************************************************
# *************************************************************************************************************************************************
# 
# SET(${USB_CONFIG_FILE_DIRECTORY} "Library/Usb") 

# # # Add the freertos_config for FreeRTOS-Kernel
# # add_library(usb_config INTERFACE "Library/Usb/usb_conf.h") # INTERFACE

# target_include_directories(usb_config
#     INTERFACE
#     "Middlewares/USB"
# )

# SET(usb_class "USB_WINDRV" CACHE STRING "" FORCE) #USB_WINDRV USB_VIRTUAL_COMPORT

# add_subdirectory(Middlewares/USB)

# target_link_libraries(${CMAKE_PROJECT_NAME} USB_LIBS)


# *************************************************************************************************************************************************
# *************************************************************************************************************************************************
# *************************************************************************************************************************************************

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC #PRIVATE
    ${include_DIRS}
    $<$<COMPILE_LANGUAGE:C>: ${include_c_DIRS}>
    $<$<COMPILE_LANGUAGE:CXX>: ${include_cxx_DIRS}>
    $<$<COMPILE_LANGUAGE:ASM>: ${include_asm_DIRS}>
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC #PRIVATE
    ${symbols_SYMB}
    $<$<COMPILE_LANGUAGE:C>: ${symbols_c_SYMB}>
    $<$<COMPILE_LANGUAGE:CXX>: ${symbols_cxx_SYMB}>
    $<$<COMPILE_LANGUAGE:ASM>: ${symbols_asm_SYMB}>

    # Configuration specific
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>: >
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME} ${link_LIBS})

# Compiler options
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC # PRIVATE
    ${cpu_PARAMS}
    ${compiler_OPTS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:C>: >
    $<$<COMPILE_LANGUAGE:CXX>:

    # -Wno-volatile
    # -Wold-style-cast
    # -Wuseless-cast
    # -Wsuggest-override
    >
    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp -MMD -MP>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>
)

# Linker options
target_link_options(${CMAKE_PROJECT_NAME} PUBLIC # PRIVATE
    -T${linker_script_SRC}
    ${cpu_PARAMS}
    ${linker_OPTS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -u _printf_float # STDIO float formatting support (remove if not used)
    --specs=nosys.specs
    #--specs=nano.specs -                ,               
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,-z,max-page-size=8 # Allow good software remapping across address space (with proper GCC section making)
    -Wl,--print-memory-usage
)

# Execute post-build to print size, generate hex and bin
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
)

# set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_C_COMPILER> -mthumb <FLAGS> <SOURCE> -o <OBJECT>")
