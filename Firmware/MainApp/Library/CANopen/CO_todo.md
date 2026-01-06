# CANopen Implementation TODO

> Implementation plan for CANopenNode on AT32F403A/407

---

## Phase 1: Complete CO_driver.c Template Functions

### 1.1 CO_CANsend — Implement hardware transmission
- [x] Extract CAN ID from `buffer->ident` (bits 0-10)
- [x] Extract DLC from `buffer->ident` (bits 11-14)
- [x] Extract RTR flag from `buffer->ident` (bit 15)
- [x] Fill `can_tx_message_type` structure
- [x] Call `can_message_transmit(CAN1, &tx_message)`
- [x] Handle `CAN_TX_STATUS_NO_EMPTY` case

### 1.2 CO_CANclearPendingSyncPDOs — Verify implementation
- [x] `can_cancel_pending_tx()` already implemented
- [x] Remove placeholder comment `/* messageIsOnCanBuffer && */`
- [x] Consider tracking which mailbox has sync PDO (documented as future enhancement)

### 1.3 CO_CANmodule_process — Connect error counters
- [x] Read `can_transmit_error_counter_get(CAN1)` → `txErrors`
- [x] Read `can_receive_error_counter_get(CAN1)` → `rxErrors`
- [x] Detect RX FIFO overflow flag → `overflow`
- [x] Clear overflow flags after reading
- [x] Documentation: Call periodically from main loop or timer

### 1.4 CO_CANinterrupt — Implement RX/TX handling
- [x] **RX path:**
  - [x] Replace `if (1)` with `can_receive_message_pending_get(CAN1, CAN_RX_FIFO0) > 0`
  - [x] Call `can_message_receive(CAN1, CAN_RX_FIFO0, &rx_message)`
  - [x] Convert `can_rx_message_type` to `CO_CANrxMsg_t` (ID, DLC, RTR flag, data)
  - [x] Release FIFO with `can_receive_fifo_release()`
- [x] **TX path:**
  - [x] Replace `else if (0)` with TX complete interrupt flag check (CAN_TM0TCF_FLAG, CAN_TM1TCF_FLAG, CAN_TM2TCF_FLAG)
  - [x] Fill `can_tx_message_type` from `CO_CANtx_t` using EXTRACT_BITS macro
  - [x] Call `can_message_transmit()` for queued messages
  - [x] Clear TX completion flags after processing
- [x] **Error path:**
  - [x] Check CAN_EOIF_FLAG (error occur interrupt flag)
  - [x] Call CO_CANmodule_process() to update error status from hardware
  - [x] Clear CAN_EOIF_FLAG after processing
  - [x] Bus-off detection via TEC >= 256 (handled in CO_CANmodule_process)
  - [x] Error passive/warning detection via TEC/REC thresholds

---

## Phase 2: Wire Interrupts to CANopenNode

### 2.1 Modify CAN ISR in can_driver.cpp
- [x] Create callback registration mechanism
- [x] Add `can_interrupt_callback_t` function pointer type
- [x] Add `can_register_interrupt_callback()` function
- [x] Modify `CAN1_RX0_IRQHandler` to call registered callback
- [x] Modify `CAN1_SE_IRQHandler` to call registered callback
- [x] Keep backward compatibility with default ISR behavior
- [x] Enable TX completion interrupt (CAN_TCIEN_INT) for CANopen

### 2.2 Add TX complete interrupt
- [x] Enable `CAN_TCIEN_INT` (transmit complete interrupt) - done in can_configure_interrupts()
- [x] Add TX handling in ISR - ISRs now call registered callback
- [x] Call `CO_CANinterrupt()` for TX completion - implemented in CO_CANinterrupt() TX path

### 2.3 Error interrupt integration
- [x] Move error reading to `CO_CANmodule_process()` - implemented
- [x] `CO_CANinterrupt()` calls `CO_CANmodule_process()` on CAN_EOIF_FLAG
- [x] Error counters read from hardware via can_transmit_error_counter_get/can_receive_error_counter_get

### 2.4 CANmodule pointer access strategy
- [x] **Option B selected:** Store pointer via setter function
- [x] Implemented via `can_register_interrupt_callback(callback, can_module)`
- [x] Stores both callback function pointer and CANmodule pointer
- [x] Clean separation of concerns, no global dependencies

---

## Phase 3: Object Dictionary Configuration

### 3.1 Create/configure OD entries
- [x] Device type (0x1000) — device profile
- [x] Error register (0x1001)
- [x] Manufacturer device name (0x1008)
- [x] Identity object (0x1018)

### 3.2 Configure PDOs
- [x] RPDO communication parameters (0x1400-0x1403)
- [x] RPDO mapping parameters (0x1600-0x1603)
- [x] TPDO communication parameters (0x1800-0x1803)
- [x] TPDO mapping parameters (0x1A00-0x1A03)

### 3.3 Application-specific objects
- [x] Define manufacturer-specific objects (0x2000-0x5FFF)
- [x] Map process variables
- [x] Define configuration parameters

### 3.4 Generate OD files
- [x] Use CANopenEditor tool, or
- [x] Copy from `example/OD.c` and modify
- [x] Create `OD.h` with extern declarations

---

## Phase 4: Application Integration

### 4.1 Create CANopenTask.c
- [x] Include CANopenNode headers

### 4.2 Implement 1ms timer
- [x] Use SysTick or TIM peripheral
- [x] Create `CO_TIMER_ISR()` or flag-based approach
- [x] Track `CO_timer1ms` counter
- [x] Call time-critical processing from timer

### 4.3 Main loop processing
```c
while (1) {
    CO_CANinterrupt(CO->CANmodule);  // if polling
    CO_process_SYNC(CO, timeDiff_us, NULL);
    CO_process_RPDO(CO, ...);
    CO_process_TPDO(CO, ...);
    reset = CO_process_NMT(CO, ...);
    CO_process_SDO_server(CO, ...);
    CO_CANmodule_process(CO->CANmodule);
    // Handle reset if needed
}
```
[x] mostly done

### 4.4 NMT state handling
- [x] Start in pre-operational
- [x] Transition to operational on NMT command
- [x] Handle reset node / reset communication
- [x] Send bootup message on init

### 4.5 Node Guarding / Heartbeat configuration
- [x] Enable Node Guarding slave in CO_driver_target.h (CO_CONFIG_NODE_GUARDING)
- [x] OD entries 0x100C and 0x100D already exist in OD.h/OD.c
- [x] Node guarding automatically initialized by CO_CANopenInit() when enabled
- [x] Guard Time (0x100C) default = 0 (disabled), to be configured by CANopen master via SDO
- [x] Life Time Factor (0x100D) default = 0 (disabled), to be configured by CANopen master via SDO
- [x] Verified: SKOV profile does NOT use heartbeat producer (0x1017) - confirmed not in EDS file
- [x] Skipped: Heartbeat consumer (0x1016) - not in SKOV EDS file, node guarding (0x100C/0x100D) is used instead

---

## Phase 5: Build Configuration

### 5.1 Configure CO_driver_target.h
- [x] Define `CO_CONFIG_GLOBAL_FLAG_CALLBACK_PRE` (enabled for FreeRTOS task synchronization)
- [x] Define `CO_CONFIG_GLOBAL_RT_FLAG_CALLBACK_PRE` (enabled for FreeRTOS RT objects)
- [x] Define `CO_CONFIG_GLOBAL_FLAG_TIMERNEXT` (timer calculation for smooth operation)
- [x] Define `CO_CONFIG_GLOBAL_FLAG_OD_DYNAMIC` (dynamic OD reconfiguration)
- [x] Set `CO_CONFIG_SDO_SRV` options (segmented transfer, callbacks, timer)
- [x] Set `CO_CONFIG_RPDO` options (timers, callbacks for 4 RPDOs per SKOV EDS)
- [x] Set `CO_CONFIG_TPDO` options (timers, callbacks for 9 TPDOs per SKOV EDS)
- [x] Set `CO_CONFIG_EM` options (EMCY producer with inhibit time per 0x1014/0x1015)
- [x] Define `CO_LOCK_CAN_SEND()` / `CO_UNLOCK_CAN_SEND()` macros (FreeRTOS taskENTER_CRITICAL)
- [x] Define `CO_LOCK_EMCY()` / `CO_UNLOCK_EMCY()` macros (FreeRTOS taskENTER_CRITICAL)
- [x] Define `CO_LOCK_OD()` / `CO_UNLOCK_OD()` macros (FreeRTOS taskENTER_CRITICAL)
- [x] Define `CO_MemoryBarrier()` for ARM Cortex-M4 (DMB instruction)

### 5.2 Update build system
- [x] Add CANopenNode source files to cmake_proj.cmake:
  - Library/CANopenNode/CANopen.c (main stack)
  - Library/CANopenNode/301/*.c (core: SDO, PDO, NMT, Emergency, etc.)
  - Library/CANopenNode/303/*.c (LEDs)
  - Library/CANopenNode/305/*.c (LSS)
  - Library/CANopenNode/storage/*.c (storage support)
- [x] Add include paths for 301/, 303/, 305/, storage/
- [x] Local driver `CO_driver.c` and `OD.c` already included via Library/CANopen/*.c*

### 5.3 Memory configuration
- [x] Calculate required RAM for buffers (~4-5 KB estimated for SKOV profile)
- [x] Use static allocation (CO_USE_GLOBALS defined in CO_driver_target.h)
- [x] Stack size documented in CO_driver_target.h (CANopen task: 512 words recommended)
- [x] FreeRTOS heap: 8 KB configured (sufficient, but heap not used by CANopen with CO_USE_GLOBALS)

### 5.4 CAN bitrate
- [x] Configurable via `can_communication_configuration(bitrate_kbps)` function
- [x] Set to 125 kbps in CANopenTask.cpp to match SKOV EDS file
- [x] Standard rates supported: 125, 250, 500, 1000 kbps
- [x] Correct timing for 120 MHz APB1 clock (div=80 for 125kbps)

### 5.5 Node ID
- [x] Fixed compile-time constant (default 0x10), or
- [ ] Read from hardware (DIP switches), or
- [x] LSS service for dynamic assignment (runtime-only, no persistent storage)

---

## Phase 6: Testing

### 6.1 Hardware CAN test
- [ ] Loopback mode test
- [ ] Send/receive raw frames
- [ ] Verify with oscilloscope/analyzer

### 6.2 Bootup test
- [ ] Device sends 0x700+NodeID on power-up
- [ ] Verify with CAN sniffer

### 6.3 SDO test
- [ ] Read 0x1000 (device type)
- [ ] Read 0x1018 (identity)
- [ ] Write/read test object
- [ ] Test expedited transfer (≤4 bytes)
- [ ] Test segmented transfer (>4 bytes)

### 6.4 PDO test
- [ ] Configure TPDO mapping
- [ ] Trigger TPDO (event, timer, sync)
- [ ] Receive RPDO, verify callback
- [ ] Test sync-triggered PDOs

### 6.5 Error handling test
- [ ] Disconnect bus → bus-off state
- [ ] Reconnect → auto-recovery
- [ ] Check 0x1001 error register
- [ ] Check 0x1003 pre-defined errors

### 6.6 Interoperability
- [ ] Connect to CANopen master
- [ ] Test with standard tools (CANopen Magic, etc.)
- [ ] Verify EDS/DCF file accuracy

---

## Phase 7: Advanced Features (Optional)

### 7.1 Emergency (EMCY)
- [ ] Configure emergency COB-ID (0x1014)
- [ ] Define application error codes
- [ ] Trigger EMCY on critical faults

### 7.2 SYNC producer
- [ ] Configure as SYNC producer if needed
- [ ] Set cycle time (0x1006)
- [ ] Synchronous PDO timing

### 7.3 Parameter storage
- [ ] Implement flash/EEPROM storage
- [ ] Handle 0x1010 (store) commands
- [ ] Handle 0x1011 (restore) commands

### 7.4 Firmware update
- [ ] SDO block transfer support
- [ ] Bootloader integration
- [ ] Program command handling

---

## Quick Reference

### Files to create/modify:
| File | Status | Description |
|------|--------|-------------|
| `CO_driver.c` | ✅ Complete | Hardware abstraction layer (618 lines) |
| `CO_driver_target.h` | ✅ Complete | Configuration and macros (225 lines) |
| `OD.c` | ✅ Complete | Object Dictionary data (auto-generated) |
| `OD.h` | ✅ Complete | OD declarations (452 lines) |
| `CANopenTask.cpp` | ✅ Complete | Application integration (210 lines) |
| `CANopenTask.h` | ✅ Complete | Task header |
| `CO_storageBlank.c/.h` | ✅ Complete | Storage template |
| `CANopen_tmrTask.cpp/.h` | ✅ Complete | Timer task for 1ms processing |

### Key AT32 functions:
```c
can_message_transmit(CAN1, &tx_msg);
can_message_receive(CAN1, CAN_RX_FIFO0, &rx_msg);
can_transmit_status_get(CAN1, mailbox);
can_transmit_cancel(CAN1, mailbox);
can_receive_message_pending_get(CAN1, CAN_RX_FIFO0);
can_transmit_error_counter_get(CAN1);
can_receive_error_counter_get(CAN1);
can_operating_mode_set(CAN1, mode);
```

### Priority order:
1. ✅ ~~**HIGH**: CO_CANsend, CO_CANinterrupt~~ (Complete)
2. ✅ ~~**HIGH**: Wire ISR → CANopenNode~~ (Complete)
3. ✅ ~~**HIGH**: Create OD.c/OD.h~~ (Complete)
4. ✅ ~~**MEDIUM**: CANopenTask integration~~ (Complete)
5. ✅ ~~**HIGH**: CAN baud rate: 125 kbps~~ (Complete - matches SKOV EDS)
6. ✅ ~~**HIGH**: Build system integration (CMake)~~ (Complete)
7. 🔴 **HIGH**: Build and verify compilation
8. 🟡 **MEDIUM**: Testing (loopback, analyzer, master)
9. 🟢 **LOW**: Advanced features (EMCY, storage)

---

*Last updated: 2025-12-22*

