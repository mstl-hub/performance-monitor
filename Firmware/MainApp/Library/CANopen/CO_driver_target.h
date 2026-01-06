/*
 * Device and application specific definitions for CANopenNode.
 *
 * @file        CO_driver_target.h
 * @author      Janez Paternoster
 * @copyright   2021 Janez Paternoster
 *
 * This file is part of <https://github.com/CANopenNode/CANopenNode>, a CANopen Stack.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 */

#ifndef CO_DRIVER_TARGET_H
#define CO_DRIVER_TARGET_H

/* This file contains device and application specific definitions. It is included from CO_driver.h, which contains
 * documentation for common definitions below. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* FreeRTOS headers for critical section macros */
#ifdef FREE_RTOS_IS_IN_USED
#include "FreeRTOS.h"
#include "task.h"
#endif

#ifdef CO_DRIVER_CUSTOM
#include "CO_driver_custom.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Stack configuration override default values. For more information see file CO_config.h. */

/* Use static allocation instead of heap (malloc/calloc)
 * This is recommended for embedded systems to avoid:
 * - Heap fragmentation
 * - Runtime allocation failures
 * - Unpredictable memory usage
 * All CANopen objects are allocated as static variables in CANopen.c
 */
#define CO_USE_GLOBALS

/* Enable Node Guarding slave (SKOV profile uses node guarding via 0x100C/0x100D) */
#define CO_CONFIG_NODE_GUARDING CO_CONFIG_NODE_GUARDING_SLAVE_ENABLE

/* Enable callbacks for FreeRTOS task synchronization
 * Callbacks are called after CAN message preprocessing in ISR,
 * allowing FreeRTOS tasks to be woken up for message processing.
 * This is useful for multi-threaded operation.
 */

/** Enable custom callback after CAN message preprocessing for mainline objects (NMT, SDO, EM, etc.)
 *  Callbacks are called after preprocessing in ISR and can wake up FreeRTOS tasks.
 *  Required for event-driven processing in multi-threaded applications.
 */
#define CO_CONFIG_GLOBAL_FLAG_CALLBACK_PRE CO_CONFIG_FLAG_CALLBACK_PRE

/** Enable custom callback after CAN message preprocessing for real-time objects (SYNC, PDO)
 *  Similar to CO_CONFIG_GLOBAL_FLAG_CALLBACK_PRE but for time-critical objects.
 *  Required for event-driven PDO processing in multi-threaded applications.
 */
#define CO_CONFIG_GLOBAL_RT_FLAG_CALLBACK_PRE CO_CONFIG_FLAG_CALLBACK_PRE

/** Enable calculation of timerNext_us variable for smooth operation
 *  Calculates the time until next periodic processing is needed.
 *  Useful for efficient task scheduling and reducing CPU usage.
 */
#define CO_CONFIG_GLOBAL_FLAG_TIMERNEXT CO_CONFIG_FLAG_TIMERNEXT

/** Enable dynamic Object Dictionary reconfiguration
 *  Allows runtime reconfiguration of CANopen objects (SDO servers, PDO mappings, etc.)
 *  by writing to OD variables during normal operation, not just during communication reset.
 */
#define CO_CONFIG_GLOBAL_FLAG_OD_DYNAMIC CO_CONFIG_FLAG_OD_DYNAMIC

/* SDO Server configuration - expedited and segmented transfers
 * Enables callback preprocessing and timer calculation for smooth operation.
 */
#define CO_CONFIG_SDO_SRV (CO_CONFIG_SDO_SRV_SEGMENTED | \
                           CO_CONFIG_FLAG_CALLBACK_PRE | \
                           CO_CONFIG_FLAG_TIMERNEXT)

/* PDO configuration for SKOV profile:
 * - 4 RPDOs (0x1400-0x1403): Relays, Analog Outputs
 * - 9 TPDOs (0x1800-0x1808): Digital/Analog Inputs, Counters, etc.
 * Enables timers for event-driven and timed transmission.
 */
#define CO_CONFIG_RPDO (CO_CONFIG_RPDO_TIMERS_ENABLE | \
                        CO_CONFIG_FLAG_CALLBACK_PRE | \
                        CO_CONFIG_FLAG_TIMERNEXT)

#define CO_CONFIG_TPDO (CO_CONFIG_TPDO_TIMERS_ENABLE | \
                        CO_CONFIG_FLAG_CALLBACK_PRE | \
                        CO_CONFIG_FLAG_TIMERNEXT)

/* Emergency (EMCY) configuration - SKOV profile uses 0x1014/0x1015 */
#define CO_CONFIG_EM (CO_CONFIG_EM_PRODUCER | \
                      CO_CONFIG_EM_PROD_INHIBIT | \
                      CO_CONFIG_FLAG_CALLBACK_PRE | \
                      CO_CONFIG_FLAG_TIMERNEXT)

/* ============================================================================
 * Memory Configuration for SKOV Profile (4 RPDO, 9 TPDO)
 * ============================================================================
 * Estimated static RAM usage (~4-5 KB):
 * - CO_CANmodule_t:      ~40 bytes
 * - RX buffers (32):     ~384 bytes  (32 × 12)
 * - TX buffers (16):     ~256 bytes  (16 × 16)
 * - SDO server:          ~100 bytes
 * - 4 RPDOs:             ~160 bytes  (4 × 40)
 * - 9 TPDOs:             ~450 bytes  (9 × 50)
 * - NMT:                 ~30 bytes
 * - Emergency:           ~50 bytes
 * - Node Guarding:       ~20 bytes
 * - LSS slave:           ~50 bytes
 * - OD data (OD.c):      ~2-3 KB
 *
 * Stack size recommendation for ISR nesting:
 * - CANopen task stack:  512 words (2048 bytes) minimum
 * - Main stack (ISR):    256 words (1024 bytes) minimum for nested interrupts
 * - FreeRTOS configures ISR stack via configISR_STACK_SIZE_WORDS
 * ============================================================================
 */

/* Basic definitions. If big endian, CO_SWAP_xx macros must swap bytes. */
#define CO_LITTLE_ENDIAN
#define CO_SWAP_16(x) x
#define CO_SWAP_32(x) x
#define CO_SWAP_64(x) x
/* NULL is defined in stddef.h */
/* true and false are defined in stdbool.h */
/* int8_t to uint64_t are defined in stdint.h */
typedef uint_fast8_t bool_t;
typedef float float32_t;
typedef double float64_t;

/* Access to received CAN message */
#define CO_CANrxMsg_readIdent(msg) ((uint16_t)0)
#define CO_CANrxMsg_readDLC(msg)   ((uint8_t)0)
#define CO_CANrxMsg_readData(msg)  ((const uint8_t*)NULL)

/* Received message object */
typedef struct {
    uint16_t ident;
    uint16_t mask;
    void* object;
    void (*CANrx_callback)(void* object, void* message);
} CO_CANrx_t;

/* Transmit message object */
typedef struct {
    uint32_t ident;
    uint8_t DLC;
    uint8_t data[8];
    volatile bool_t bufferFull;
    volatile bool_t syncFlag;
} CO_CANtx_t;

/* CAN module object */
typedef struct {
    void* CANptr;
    CO_CANrx_t* rxArray;
    uint16_t rxSize;
    CO_CANtx_t* txArray;
    uint16_t txSize;
    uint16_t CANerrorStatus;
    volatile bool_t CANnormal;
    volatile bool_t useCANrxFilters;
    volatile bool_t bufferInhibitFlag;
    volatile bool_t firstCANtxMessage;
    volatile uint16_t CANtxCount;
    uint32_t errOld;
} CO_CANmodule_t;

/* Data storage object for one entry */
typedef struct {
    void* addr;
    size_t len;
    uint8_t subIndexOD;
    uint8_t attr;
    /* Additional variables (target specific) */
    void* addrNV;
} CO_storage_entry_t;

/* (un)lock critical section in CO_CANsend()
 * Uses FreeRTOS critical sections to protect CAN TX operations from concurrent access.
 * taskENTER_CRITICAL/taskEXIT_CRITICAL disable interrupts and are ISR-safe.
 */
#ifdef FREE_RTOS_IS_IN_USED
#define CO_LOCK_CAN_SEND(CAN_MODULE)   taskENTER_CRITICAL()
#define CO_UNLOCK_CAN_SEND(CAN_MODULE) taskEXIT_CRITICAL()
#else
#define CO_LOCK_CAN_SEND(CAN_MODULE)
#define CO_UNLOCK_CAN_SEND(CAN_MODULE)
#endif

/* (un)lock critical section in CO_errorReport() or CO_errorReset()
 * Protects emergency message generation from concurrent access.
 */
#ifdef FREE_RTOS_IS_IN_USED
#define CO_LOCK_EMCY(CAN_MODULE)       taskENTER_CRITICAL()
#define CO_UNLOCK_EMCY(CAN_MODULE)     taskEXIT_CRITICAL()
#else
#define CO_LOCK_EMCY(CAN_MODULE)
#define CO_UNLOCK_EMCY(CAN_MODULE)
#endif

/* (un)lock critical section when accessing Object Dictionary
 * Protects OD read/write operations from concurrent access.
 */
#ifdef FREE_RTOS_IS_IN_USED
#define CO_LOCK_OD(CAN_MODULE)         taskENTER_CRITICAL()
#define CO_UNLOCK_OD(CAN_MODULE)       taskEXIT_CRITICAL()
#else
#define CO_LOCK_OD(CAN_MODULE)
#define CO_UNLOCK_OD(CAN_MODULE)
#endif

/* Synchronization between CAN receive and message processing threads.
 * ARM Cortex-M4 data memory barrier ensures all memory accesses complete
 * before continuing. Required for proper ISR/task synchronization.
 */
#if defined(__ARM_ARCH) || defined(__arm__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
#define CO_MemoryBarrier() __asm volatile("dmb" ::: "memory")
#else
#define CO_MemoryBarrier()
#endif
#define CO_FLAG_READ(rxNew) ((rxNew) != NULL)
#define CO_FLAG_SET(rxNew)                                                                                             \
    {                                                                                                                  \
        CO_MemoryBarrier();                                                                                            \
        rxNew = (void*)1L;                                                                                             \
    }
#define CO_FLAG_CLEAR(rxNew)                                                                                           \
    {                                                                                                                  \
        CO_MemoryBarrier();                                                                                            \
        rxNew = NULL;                                                                                                  \
    }

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CO_DRIVER_TARGET_H */
