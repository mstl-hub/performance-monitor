# TafcoPigstore - Complete Embedded System Suite

## Overview


### Main Projects

#### 1. P-Block (Main Application)

#### 2. Bootloader0
**Location**: `bootloader0/`
**Technology**: C/C++ on AT32F403A/407 MCU
**Purpose**: Firmware bootloader with USB and UART support
- Vector table relocation for application switching
- USB communication for firmware updates
- UART interface for debugging

#### 3. Prebootload
**Location**: `prebootload/`
**Technology**: C/C++ on AT32F403A/407 MCU
**Purpose**: Pre-bootloader for initial system setup
- Basic MCU initialization and clock configuration
- Flash service and watchdog setup
- Bridge between hardware reset and main bootloader

## Architecture

### Hardware Platform
- **MCU**: AT32F403A/407 (ARM Cortex-M4F) - Primary target
- **MCU**: AT32F435/437 (ARM Cortex-M4F) - Secondary support available
- **Clock Speed**: 240MHz (403A/407), 288MHz (435/437)
- **Build System**: CMake with GCC ARM toolchain
- **RTOS**: FreeRTOS (used in P-block main application)
- **Shared Library**: TafcoMcuCore (`../TafcoMcuCore`) - common MCU services
  - Comprehensive documentation in `TafcoMcuCore/ProjectContext.md` (1,142 lines)

### Project Dependencies
- **TafcoMcuCore**: Shared MCU abstraction layer
- **FreeRTOS**: Real-time operating system kernel

### Main Application Structure

#### Initialization Sequence (main.cpp)
1. **System Configuration**:
   - Define `SystemServices_t services` bitfield for selective service initialization
   - Configure enabled services: UpTime timer, Flash service, Unit reset, Bootloader config
   - Disable services: UART service (handled by Modbus), Watchdog timer

2. **MCU Initialization**:
   - Call `SystemApi::Init()` for basic MCU setup:
     - NVIC priority group configuration (NVIC_PRIORITY_GROUP_4)
     - Clock configuration to 240MHz (_240Mhz)
     - GPIO remapping for debug pins (JTAG disabled, SW-DP enabled)
     - Vector table relocation to `_sapp`
     - Global interrupt enable

3. **Service Initialization**:
   - Call `SystemApi::InitServices(services)` for selective service initialization
   - Services initialized based on bitfield configuration:
     - **UpTime Timer**: System uptime tracking
     - **Flash Service**: Persistent storage management
     - **Unit Reset**: System component reset functionality
     - **Bootloader Config**: Firmware update configuration
   - Command handler setup
   - P-Block Modbus registers initialization via `PBlockRegisters_t::Init()`

## Core Components

### System API (SystemApi)
**Location**: `TafcoMcuCore/Library/ArteryCore/SystemApi.h`, `SystemApi.cpp`

**Purpose**: Unified MCU initialization and service management system

**Architecture**:
- **SystemApi::Init()**: Basic MCU hardware initialization
  - NVIC priority group configuration
  - Clock speed setup (supports multiple frequencies: MAX, 240MHz, 24MHz, etc.)
  - GPIO pin remapping for SWD over JTAG
  - Vector table relocation for bootloader support
  - Global interrupt enable
- **SystemServices Bitfield**: Selective service initialization using bitfield flags
  - **UpTimeTmr**: System uptime tracking
  - **FlashService**: Persistent storage operations
  - **UnitReset**: System component reset functionality
  - **UartService**: UART communication (requires usart_init_type config)
  - **WatchdogTimer**: Watchdog monitoring
  - **BootloaderConfig**: Firmware update configuration

**Benefits**:
- Modular service initialization (enable/disable individual services)
- Memory-efficient bitfield configuration (1 byte for 6+ services)
- Error handling for missing configurations
- Backward compatibility with legacy initialization

Note:
- When `UartService` is enabled, a valid `usart_init_type* init_config` must be provided via `SystemServices`. If not provided, `SystemApi::InitServices()` returns `ERROR` and skips UART initialization.

### 2. UART Communication
**Location**: `P-block/Library/Uart/`

**Notes**:
- UART peripheral init through `SystemApi` requires `services.init_config`.
- **Protocol**: Custom command protocol with CRC validation
- **Operation**: Continuous reading, command parsing, and response transmission
- **Integration**: Works with command handler for device management

### 3. Command System
**Location**: `P-block/Library/Command/`

**Supported Commands**:
- **System Info** (0x10-0x11): Application type, version
- **Time Management** (0x30-0x33): Current time (ms/s), RTC get/set
- **UART Control** (0x34): UART work mode configuration
- **Device Config** (0x48-0x4B): Serial number, assembly date
- **System Config** (0x4C-0x4E): Frequency settings, error count
- **Data Management** (0x50-0x57): Cache operations, metrics retrieval
- **System Control** (0xFF): System reset

**Total Commands**: 17 active commands (LoRaWAN commands have been removed)

### 4. Configuration Management
**Location**: `P-block/Library/Config/`

**PBlockConfig Features**:
- **Flash Storage**: Sector 253 for persistent configuration
- **CRC Protection**: Data integrity validation
- **Singleton Pattern**: Global configuration access
- **Parameters**: Serial number, assembly date, Modbus settings
- **Auto-Recovery**: Default values on corruption detection

### 5. Watchdog System
**Location**: `P-block/Library/WatchDog/`

**Multi-Subject Monitoring**:
- Tracks multiple system components (unit_control, blinky, gateway, uart, usb)
- Requires all monitored subjects to report before watchdog reload
- Automatic reset on system hang detection
- 256 prescaler with 0xB00 reload value

### 6. Task Implementations

#### LED Task (`ledFun`)
- **P-Block Mode**: Simple 1-second delay loop (placeholder)
- **LITE_GATEWAY Mode**: Debug LED with blink-once functionality
- **Watchdog**: Reports to watchdog system

#### Display Task (`displayFun`)
- **Purpose**: Display management and updates
- **Update Rate**: 100ms periodic updates
- **Status**: Basic framework (implementation pending)

#### Unit Control Task (`unitControlFun`)
- **Purpose**: System health monitoring and error recovery
- **Function**: Calls `unit_control_wave()` function to monitor system units
- **Rate**: 500ms execution cycle
- **Functionality**: Monitors and resets system components (UART, SPI, MCU) on error
- **Watchdog**: Reports unit_control status

#### Debug Task (`debugTask`)
- **Purpose**: UART peripheral verification and system monitoring
- **Function**: Sends periodic debug messages via UART1
- **Rate**: 2 second transmission cycle
- **Message Format**: `[DEBUG] Msg#<counter> | Uptime: <seconds>.<milliseconds>s | FreeRTOS: OK | UART: TX`
- **Implementation**: Manual string formatting (no stdlib dependency) using `drv_uart_transmit()`
- **Use Case**: Verify UART functionality and monitor system uptime

## TafcoMcuCore Libraries

### GPIO Improvements (PinConfig)
**Location**: `TafcoMcuCore/Library/ArteryCore/Periphery/gpio.h`, `gpio.cpp`

**Recent Enhancements** (December 2025):
- **PinConfig struct**: Encapsulates GPIO port and pin configuration
  - Default constructor: `PinConfig()` - initializes to nullptr/0
  - Parameterized constructor: `PinConfig(gpio_type* port, uint16_t pin)`
  - Destructor support
- **pinInit overload**: `pinInit(PinConfig config, PinMode_t mode, gpio_pull_type pull)`
- **PinControl refactoring**: Uses `PinConfig` instead of separate port/pin members
- **Initialization order**: Fixed member initialization order in constructors

**Benefits**:
- Type-safe pin configuration
- Reduced parameter passing (single struct vs. two parameters)
- Consistent pin handling across GPIO, SPI, and peripheral drivers
- Better code organization and maintainability

### GENERAL_SPI PinConfig Integration
**Location**: `TafcoMcuCore/Library/ArteryCore/Periphery/general_spi.h`, `general_spi.cpp`, `general_spi_struct.h`

**Recent Enhancements** (December 2025):
- **PinConfig integration**: Replaced individual pin fields with `PinConfig` structs
  - `PinConfig mosi_pin` (was: `uint16_t mosi_pin` + `gpio_type* mosi_type`)
  - `PinConfig miso_pin` (was: `uint16_t miso_pin` + `gpio_type* miso_type`)
  - `PinConfig sck_pin` (was: `uint16_t sck_pin` + `gpio_type* sck_type`)
  - `PinConfig cs_pin` (was: `uint16_t cs_pin` + `gpio_type* cs_type`)

## Build Configuration

### CMake Setup
- **Project**: PigStore-PBlock
- **Standards**: C11, C++20
- **MCU Target**: AT32F407VGT7
- **Optimization**: -Og for debug, -O1 for release

### Key Definitions
- `FREE_RTOS_IS_IN_USED`: Enables FreeRTOS integration
- `DEBUG_PRINT_USB`: USB debug output
- `GLOB_USE_RTT`: Segger RTT debugging
- `USE_WDT`: Watchdog enabled in release builds

### Memory Layout
- **Flash**: Linker script defines memory regions
- **Stack**: Individual task stacks (128-256 words)
- **Heap**: FreeRTOS heap type 1
- **Config Storage**: Flash sector 253

## Communication Interfaces

### 2. UART Command Interface
- **Purpose**: Device configuration and monitoring
- **Protocol**: Custom binary protocol with CRC
- **Commands**: 17 commands for system management
- **Response**: Structured response with status codes

### 3. USB Interface
- **Status**: Framework present but not fully implemented
- **Purpose**: Debug output and potential data interface

## Development Status

### Build Status
- ✅ **Bootloader0**: Successfully builds without errors
  - Memory usage: ~24.3 KB (Text: 17.2 KB, Data: 292 B, BSS: 6.8 KB)
  - All compilation issues resolved
  - Linker script configured with bootloader symbols
  
- ⏳ **Prebootloader**: Ready for build testing

### Implemented Features
- ✅ **SystemApi Architecture**: Unified MCU initialization and selective service management
- ✅ **SystemServices Bitfield**: Memory-efficient service configuration system
- ✅ **Bootloader0 Compilation**: Fully resolved and successfully building
- ✅ **Flash Memory Layout**: Project-specific configurations with fallback support
- ✅ **ErrorJournal Enhancement**: Structured error recording with timestamps
- ✅ FreeRTOS task structure
- ✅ Modbus RTU communication
- ✅ UART command interface (17 commands)
- ✅ Configuration management with flash storage
- ✅ Watchdog system
- ✅ Basic task framework

### Key Features

#### Serial Communication (`ModbusService.py`)
- Cross-platform COM port handling (Windows/Linux/Mac)
- Configurable baud rates and serial parameters
- Timeout management and connection recovery

## Documentation Repository - at32f403a-407-docs

### Overview
**Location**: `TafcoMcuCore/at32f403a-407-docs/`
**Status**: Standalone Git repository (can be added as submodule)
**Purpose**: Context7-optimized MCU errata documentation

The `at32f403a-407-docs` repository contains comprehensive, searchable documentation for the Artery AT32F403A/407 ARM Cortex-M4 MCU family. Originally extracted from PDF format, the documentation has been enhanced with diagrams, tables, and code examples.

### Integration with TafcoMcuCore

The documentation can be used in two ways:

#### Standalone Repository
- Independent documentation accessible to any project
- Can be cloned and referenced separately
- Optimal for Context7 indexing

#### Git Submodule (Recommended)
- Linked to TafcoMcuCore as `TafcoMcuCore/Docs/`
- Independent versioning for documentation
- Separate issue tracking for doc improvements
- Can be referenced by other projects
- Easier community contributions

### Original Source Materials

Original Artery documentation is preserved in `Docs/Artery/`:
- **`ES0002_AT32F403A_407_Errata_Sheet_EN_V2.0.11.pdf`**: Original PDF source
- **`archive/`**: Development tools and conversion scripts
  - `compare_pdf_md.py`: PDF/Markdown comparison tool
  - `analyze_images.py`: Image categorization script
  - `cleanup_unused_images.py`: Repository cleanup tool
  - Analysis reports and extracted text files

### Benefits

1. **Context7 Ready**: Optimized for AI-powered documentation search
2. **Developer Friendly**: Searchable Markdown with code examples
3. **Version Controlled**: Track documentation changes alongside code
4. **Community Accessible**: Public repository for sharing and contributions
5. **Enhanced Usability**: Visual diagrams and quick reference tables
6. **Comprehensive**: 100% coverage of all errata issues

## Documentation Repository - at32f435-437-docs

### Overview
**Location**: `TafcoMcuCore/at32f435-437-docs/`
**Status**: PDF documentation collection (not a Git submodule)
**Purpose**: Reference documentation for AT32F435/437 MCU family

The `at32f435-437-docs` directory contains official Artery documentation for the AT32F435/437 ARM Cortex-M4 MCU family. This is a secondary MCU platform with advanced features and higher performance (288MHz vs 240MHz).

### Key Features

#### Documentation Collection
- **Errata Sheet**: ES0003_AT32F435_437_Errata_Sheet_V2.0.15_EN.pdf
- **Datasheet**: DS_AT32F435_437_V2.20_EN.pdf
- **Reference Manual**: RM_AT32F435_437_V2.07_EN.pdf
- **Application Notes**: Security Library, Getting Started Guide
- **Migration Guide**: MG0018 - Migrating from AT32F403A/407 to AT32F435/437

#### Vendor SDK Management
- **SDK directories excluded** via `.gitignore` (document/, libraries/, middlewares/, project/, utilities/)
- **Only PDFs tracked** in Git repository
- **Vendor SDK available** in working directory for local development
- **Reduced repository size** by excluding thousands of example files

### AT32F435/437 MCU Features
- **Core**: ARM Cortex-M4F with FPU and DSP
- **Clock**: Up to 288MHz
- **Advanced Peripherals**: EMAC (Ethernet), DVP (Digital Video Port), QSPI
- **Enhanced Connectivity**: Dual CAN, USB OTG HS/FS
- **Memory**: Up to 4032KB Flash, 512KB SRAM

### Usage
The AT32F435/437 support is available in `TafcoMcuCore/AT32F435/` with:
- Complete CMSIS and peripheral drivers
- Advanced middleware (FatFS, LVGL, LwIP 2.1.2)
- FreeRTOS support
- USB device and host classes

## Bootloader Projects

### Bootloader0
**Location**: `bootloader0/`
**MCU Target**: AT32F403ACGT7 (default), AT32F407VGT7, AT32F403ACGU7
**Purpose**: Main firmware bootloader with advanced features
**Build Status**: ✅ Successfully compiles and links

**Memory Layout**:
- Flash Region: 0x08001000 - 0x08008000 (32 KB)
- Memory Usage: ~24.3 KB (75% of allocated space)
  - Text (Code): 17.2 KB (50% of total flash)
  - Data: 292 B
  - BSS (Uninitialized): 6.8 KB
  - Available: ~8 KB for updates/extensions

**Features**:
- Vector table relocation for seamless application switching via `_sboot` symbol
- USB CDC class support for firmware updates
- UART debugging interface
- Flash memory management via FlashService
- Watchdog monitoring during boot process
- Project-specific flash_map.h for memory configuration

**Key Components**:
- Command processing system
- USB communication stack
- Flash service integration (corrected void* pointer handling)
- Error logging and recovery via ErrorJournal
- Watchdog driver with stdint support

### Prebootload
**Location**: `prebootload/`
**MCU Target**: AT32F403ACGT7 (default), AT32F407VGT7, AT32F403ACGU7
**Purpose**: Pre-bootloader for initial hardware setup
**Build Status**: ⏳ Ready for testing (dependencies resolved)

**Memory Layout**:
- Flash Region: 0x08000000 - 0x08001000 (4 KB)
- Minimal footprint for fast boot

**Features**:
- Basic MCU clock and peripheral initialization
- Minimal flash service for bootloader validation
- Watchdog configuration
- Bridge to main bootloader execution
- Project-specific flash_map.h for memory configuration

**Key Components**:
- Core system initialization
- BPR (Backup Power Register) service
- Simple watchdog implementation
- Boot sequence management
- ✅ **Bootloader Build Success**: Fixed all compilation errors in bootloader0
  - Added missing `<cstring>` include to wdt.cpp for memset function
  - Fixed void* pointer casting in FlashService (Read, IsDiffer, Reprogramm methods)
  - Added PinDefines.h include to Bootload.cpp for GPIO macro definitions
  - Added `_sboot` linker symbol for bootloader VTOR configuration
  - Added `PIGSTORE_PBLOCK` define to bootloader0 CMakeLists.txt
  
- ✅ **Flash Memory Layout Reorganization**: Implemented Option 1 project-specific approach
  - Moved flash_map.h from TafcoMcuCore/Library/ to project-specific locations:
    - `P-block/Main/inc/flash_map.h` (existing)
    - `bootloader0/Main/inc/flash_map.h` (created)
    - `prebootload/Main/inc/flash_map.h` (created)
  - Updated ErrorJournal.h with conditional `__has_include` fallback mechanism
  - Each project now owns its memory layout configuration
  
- ✅ **ErrorJournal System Redesign**: Enhanced error recording from simple counters to structured records
  - Added ErrorJournalRecordType enum (WDT, SPI1, UART1, CAN, MCU)
  - Replaced simple ErrorJournalCounts_t with ErrorJournalRecord_t struct
  - Added timestamp support (calendar_type dateTime field)
  - Increased capacity from single count to array of 1280 error records
  - Added ErrorJournalRecordMap for tracking record metadata
  - RTC integration for timestamping events

## File Structure
```
TafcoPigstore/
├── P-block/                          # Main Modbus RTU application
│   ├── Main/
│   │   ├── src/main.cpp              # Application entry point
│   │   └── inc/main.h                # Main includes and definitions
│   ├── Library/                      # P-block specific libraries
│   │   ├── Uart/                     # UART communication
│   │   ├── Command/                  # Command processing system
│   │   ├── Config/                   # Configuration management
│   │   ├── WatchDog/                 # Watchdog implementation
│   │   ├── Tasks/                    # FreeRTOS task definitions
│   │   ├── Display/                  # Display management
│   │   ├── Buttons/                  # Button input handling
│   │   ├── UnitControlApp/           # Unit control operations
│   │   ├── PBlock/                   # P-block data structures
│   │   ├── GPIO/                     # GPIO management
│   │   └── Input/                    # Universal input system
│   ├── Middlewares/
│   │   └── FreeRTOS/                 # FreeRTOS kernel and config
│   ├── cmake_proj/
│   │   └── cmake_proj.cmake          # CMake build configuration
│   ├── ldscript/                     # Linker scripts
│   └── CMakeLists.txt                # Project build configuration
│
├── bootloader0/                      # Main bootloader
│   ├── Main/
│   ├── Library/
│   ├── cmake_proj/cmake_proj.cmake   # Build config
│   └── ldscript/                     # Linker scripts
│
├── prebootload/                      # Pre-bootloader
│   ├── Main/
│   ├── Library/
│   ├── cmake_proj/cmake_proj.cmake   # Build config
│   └── ldscript/                     # Linker scripts
│
├── Console/                          # Python testing suite
│   ├── tests.py                       # Automated test suite (24 tests)
│   ├── main.py                        # Interactive console
│   ├── CommandGenerator.py            # Modbus command generation
│   ├── ModbusService.py               # Serial communication
│   ├── CommandInterpreter.py          # Packet analysis
│   ├── Models.py                      # Data models
│   └── requirements.txt               # Python dependencies
│
├── TafcoMcuCore/                     # Shared MCU library
│   ├── ProjectContext.md              # TafcoMcuCore comprehensive documentation (1,142 lines)
│   ├── AT32F403A/                     # AT32F403A/407 MCU support
│   │   ├── cmsis/                     # CMSIS core and DSP libraries
│   │   ├── drivers/                   # Peripheral drivers (51 files)
│   │   ├── ldscript/                  # Linker scripts
│   │   ├── Middlewares/               # FreeRTOS, USB, Segger RTT
│   │   ├── startup/                   # Startup assembly code
│   │   └── svd/                       # System View Description files
│   ├── AT32F435/                      # AT32F435/437 MCU support
│   │   ├── cmsis/                     # CMSIS core and DSP libraries
│   │   ├── drivers/                   # Peripheral drivers (57 files)
│   │   └── middlewares/               # FatFS, LVGL, LwIP, FreeRTOS, USB
│   ├── Library/                       # Common services and utilities
│   │   ├── ArteryCore/                # SystemApi, peripherals, watchdog
│   │   │   ├── Periphery/             # GPIO, SPI, ADC drivers
│   │   │   │   ├── gpio.h/cpp         # GPIO with PinConfig struct
│   │   │   │   ├── general_spi.h/cpp  # SPI with PinConfig integration
│   │   │   │   └── general_spi_struct.h  # SPI config with PinConfig
│   │   ├── MS5614T-Api/               # MS5614/5614T DAC driver (✅ Complete)
│   │   │   ├── MS5614-Api.h/cpp       # Quad-channel 12-bit DAC API
│   │   │   └── Readme.md              # Complete datasheet documentation
│   │   ├── Command/                   # Command handler system
│   │   ├── FlashService/              # Flash memory management
│   │   ├── ErrorJournal/              # Error logging system
│   │   ├── Uart/                      # UART communication
│   │   ├── Usb/                       # USB communication
│   │   ├── UnitControl/               # Unit control
│   │   ├── externals/                 # CANopenNode submodule
│   │   ├── sx126x_driver/             # LoRa driver submodule
│   │   └── llcc68_driver/             # LoRa driver submodule
│   ├── ldscripts/                     # Centralized linker scripts
│   │   ├── 0-preboot/                 # Prebootloader region (4 KB)
│   │   ├── 400-boot/                  # Bootloader region (32 KB)
│   │   └── 1000-main-app/             # Main application region
│   ├── utils/                         # Utility applications
│   │   └── usb-can-converter/         # USB-CAN bridge converter (✅ Complete)
│   │       ├── CMakeLists.txt         # Cross-platform build config
│   │       ├── README.md              # User guide & protocol spec
│   │       ├── BUILD_GUIDE.md         # Build system documentation
│   │       ├── PROJECT_SUMMARY.md     # Architecture reference
│   │       ├── ProjectContext.md      # Complete project documentation
│   │       └── Main/                  # Source code (1,575 lines)
│   ├── at32f403a-407-docs/           # AT32F403A/407 documentation (Git submodule)
│   │   ├── ES0002_AT32F403A_407_Errata_Sheet_EN_V2.0.11.md
│   │   ├── DS_AT32F403A_V2.04_EN.md  # Datasheet (converted to Markdown)
│   │   ├── README.md                  # Repository documentation
│   │   ├── FAQ.md                     # Frequently asked questions
│   │   ├── context7.json              # Context7 integration config
│   │   ├── images/                    # Embedded diagrams
│   │   └── AT32F403A_407_Firmware_Library/  # 1,985 code examples
│   ├── at32f435-437-docs/            # AT32F435/437 documentation (PDF collection)
│   │   ├── .gitignore                 # Excludes vendor SDK directories
│   │   ├── ES0003_AT32F435_437_Errata_Sheet_V2.0.15_EN.pdf
│   │   ├── DS_AT32F435_437_V2.20_EN.pdf  # Datasheet
│   │   ├── RM_AT32F435_437_V2.07_EN.pdf  # Reference manual
│   │   └── [PDF documentation only - SDK directories excluded]
│   └── 3_ADD_AS_SUBMODULE.ps1        # Submodule setup script
│
├── Docs/                             # Project documentation
│   └── Artery/                        # Original Artery documentation
│       ├── ES0002...pdf               # Original errata PDF
│       ├── README.md                  # Documentation overview
│       └── archive/                   # Development tools archive
│
├── cursor-memory-bank/               # AI assistant memory
├── activeContext.md                  # Current session state
├── progress.md                       # Implementation progress tracking
├── tasks.md                          # Task management (single source of truth)
├── TODO.md                           # Quick TODO list
├── projectbrief.md                   # Project brief overview
├── ProjectContext.md                 # This file (comprehensive reference)
└── CMakeLists.txt                    # Root build configuration
```

## Integration Points

### Flash Memory Layout Organization
**Architecture**: Project-specific flash_map.h configuration approach
- Each project defines its own memory layout in `Main/inc/flash_map.h`
- ErrorJournal.h uses `__has_include` for conditional, fallback-safe includes
- Memory layout definitions include:
  - Prebootloader region (0x08000000, 4 KB)
  - Bootloader region (0x08001000, 32 KB)
  - Main application region (variable size)
  - Configuration storage sector (253)
  - Error journal sector (243)
  - Backup power register (BPR) layout

**Benefits**:
- ✅ Project isolation: Each project owns its memory configuration
- ✅ No shared library pollution: flash_map.h is project-specific, not in TafcoMcuCore
- ✅ Flexibility: Projects can customize their memory layout
- ✅ Robustness: Conditional includes with sensible defaults

### TafcoMcuCore Integration
- Shared library path: `../TafcoMcuCore`
- Common services: FlashService, UartService, Timer, etc.
- HAL abstraction layer for AT32 peripherals
- Note: flash_map.h is NOT shared (project-specific per Option 1 design)

### Bootloader Integration
- Vector table relocation support via `_sboot` linker symbol
- Bootloader configuration management
- Flash memory partitioning via flash_map.h
- Separate boot and main application regions

This P-Block implementation serves as a comprehensive **climate control system main controller** with extensive Modbus RTU I/O capabilities. It's designed specifically for microclimate management applications, providing precise control over HVAC systems, sensors, and actuators through a robust Modbus RTU interface.