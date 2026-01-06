#ifndef FLASH_MAP_H
#define FLASH_MAP_H

#include <stdint.h>

/**
 * @file flash_map.h
 * @brief Flash memory layout and constants for AT32F407 MCU
 * 
 * This file defines the memory organization for the pre-bootloader,
 * including flash sectors, storage areas, and backup power register layout.
 */

/* ============================================================================
 * Flash Memory Base Address
 * ========================================================================== */
#define FLASH_BASE_ADDRESS 0x08000000U

/* ============================================================================
 * Main Flash Regions (AT32F407VGT7 - 1024 KB total)
 * ========================================================================== */

/// Prebootloader region
#define PREBOOTLOADER_ADDRESS 0x08000000U
#define PREBOOTLOADER_SIZE 0x1000U  // 4 KB
#define PREBOOTLOADER_SECTOR 0U

/// Main Bootloader (Bootloader0)
#define BOOTLOADER0_ADDRESS 0x08001000U
#define BOOTLOADER0_SIZE 0x8000U  // 32 KB
#define BOOTLOADER0_SECTOR 2U

/// Main Application
#define APPLICATION_ADDRESS 0x08009000U
#define APPLICATION_SIZE 0x40000U  // 256 KB
#define APPLICATION_SECTOR_START 18U

/// Bootloader for rewriting main bootloader
#define BOOTLOADER_UPDATE_ADDRESS 0x08000000U + (173 * 2048U)
#define BOOTLOADER_UPDATE_SIZE 0x8000U  // 32 KB
#define BOOTLOADER_UPDATE_SECTOR 173U

/// Block error journal
#define ERROR_JOURNAL_ADDRESS 0x08000000U + (243 * 2048U)
#define ERROR_JOURNAL_SIZE 0x2800U  // 10 KB
#define ERROR_JOURNAL_SECTOR 243U

/// Block logs (authentication, settings)
#define BLOCK_LOGS_ADDRESS 0x08000000U + (248 * 2048U)
#define BLOCK_LOGS_SIZE 0x2800U  // 10 KB
#define BLOCK_LOGS_SECTOR 248U

/// Main application configuration storage
#define APP_CONFIG_ADDRESS 0x807E800U
#define APP_CONFIG_SIZE 0x800U  // 2 KB
#define APP_CONFIG_SECTOR 253U

/// Bootloader journal/diagnostic storage
#define BOOTLOADER_JOURNAL_ADDRESS 0x807F000U
#define BOOTLOADER_JOURNAL_SIZE 0x800U  // 2 KB
#define BOOTLOADER_JOURNAL_SECTOR 254U

/// Bootloader configuration storage
#define BOOTLOADER_CONFIG_ADDRESS 0x807F800U
#define BOOTLOADER_CONFIG_SIZE 0x800U  // 2 KB
#define BOOTLOADER_CONFIG_SECTOR 255U

/// Reserve/Extension application area
#define RESERVE_APPLICATION_ADDRESS 0x8080000U
#define RESERVE_APPLICATION_SIZE 0x40000U  // 256 KB
#define RESERVE_APPLICATION_SECTOR 256U

/* ============================================================================
 * Flash Sector Information
 * ========================================================================== */

/// Total flash memory size
#define TOTAL_FLASH_SIZE 0x100000U  // 1024 KB

/// Flash sector size (2 KB per sector for AT32F407)
#define FLASH_SECTOR_SIZE 0x800U

/* ============================================================================
 * Memory Layout Summary (in bytes)
 * ========================================================================== */

/**
 * Flash Memory Organization:
 * 
 * 0x08000000 - 0x08000FFF (4 KB)   : Prebootloader
 * 0x08001000 - 0x08008FFF (32 KB)  : Bootloader0
 * 0x08009000 - 0x08048FFF (256 KB) : Main Application
 * ...
 * 0x807E800  - 0x807EFFF (2 KB)    : App Config
 * 0x807F000  - 0x807FFFF (2 KB)    : Bootloader Journal
 * 0x807F800  - 0x807FFFF (2 KB)    : Bootloader Config
 * 0x8080000  - 0x80BFFFF (256 KB)  : Reserve Application
 */

#endif /* FLASH_MAP_H */


