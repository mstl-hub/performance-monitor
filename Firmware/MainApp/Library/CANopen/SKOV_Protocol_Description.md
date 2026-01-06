# SKOV Main IO CANopen Protocol Description

**Document Version:** 1.0  
**Date:** 2025-11-12  
**Device:** SKOV Main IO (Product Code: SKOV_Main_Io)  
**Vendor:** SKOV A/S (Vendor ID: 1116)  
**Protocol:** CANopen CiA 301  
**EDS Version:** 5.0

---

## Table of Contents

1. [Overview](#overview)
2. [Device Identification](#device-identification)
3. [Object Dictionary](#object-dictionary)
4. [PDO Communication](#pdo-communication)
5. [Analog Input Protocol](#analog-input-protocol)
6. [Digital I/O Protocol](#digital-io-protocol)
7. [DOL12 Sensor Protocol](#dol12-sensor-protocol)
8. [DOL78 Module Protocol](#dol78-module-protocol)
9. [Initialization Sequence](#initialization-sequence)
10. [Data Encoding](#data-encoding)
11. [Examples](#examples)

---

## Overview

### Device Purpose

The SKOV Main IO is a CANopen-based industrial I/O module designed for climate control applications in agricultural buildings (poultry houses, pig farms, etc.). It provides:

- **Analog Inputs:** 7-11 configurable inputs (16-bit resolution)
- **Analog Outputs:** 2-6 outputs (16-bit resolution)
- **Digital Inputs:** 12 inputs with counter functionality
- **Digital Outputs:** 12 relay outputs + 1 alarm relay
- **TRIAC Outputs:** 2 channels for phase-cut control
- **Temperature Sensors:** Support for SKOV DOL12 NTC sensors
- **Diagnostic Monitoring:** Internal temperature, voltages, calibration status

### Key Features

| Feature | Specification |
|---------|---------------|
| **CANopen Profile** | CiA 301 Device Profile |
| **Baudrate** | 125 kbit/s |
| **Node ID Range** | 1-127 (configurable) |
| **PDO Count** | 9 TPDO, 4 RPDO |
| **SDO Channels** | 1 server |
| **NMT Support** | Full state machine |
| **Heartbeat** | Supported (producer/consumer) |
| **Guard Time** | Supported |
| **Emergency** | Supported |

---

## Device Identification

### Mandatory Identity Objects

| Index | Sub | Parameter | Type | Value | Description |
|-------|-----|-----------|------|-------|-------------|
| 0x1000 | - | Device Type | UNSIGNED32 | 0x000E0191 | Device classification |
| 0x1001 | - | Error Register | UNSIGNED8 | - | Current error status |
| 0x1018 | 0 | Identity Object | UNSIGNED8 | 4 | Number of entries |
| 0x1018 | 1 | Vendor ID | UNSIGNED32 | 1116 | SKOV A/S |
| 0x1018 | 2 | Product Code | UNSIGNED32 | - | Product identifier |
| 0x1018 | 3 | Revision Number | UNSIGNED32 | - | Hardware revision (e.g., 29) |
| 0x1018 | 4 | Serial Number | UNSIGNED32 | - | Unique serial (e.g., 100412420) |

### Optional Identity Objects

| Index | Parameter | Type | Example Value | Description |
|-------|-----------|------|---------------|-------------|
| 0x1008 | Manufacturer Device Name | STRING | "SM00" | Device model code |
| 0x1009 | Manufacturer Hardware Version | STRING | "1" | Hardware version string |
| 0x100A | Manufacturer Software Version | STRING | "5202" | Firmware version |

**Note:** Serial number format is typically displayed as `{Serial}-{Revision}` (e.g., "100412420-29")

---

## Object Dictionary

### Communication Profile Objects (0x1000-0x1FFF)

#### Network Management (NMT)

| Index | Sub | Parameter | Type | Access | Default | Description |
|-------|-----|-----------|------|--------|---------|-------------|
| 0x100C | - | Guard Time | UNSIGNED16 | RW | 0 | Guard time in ms |
| 0x100D | - | Life Time Factor | UNSIGNED8 | RW | 0 | Multiplier for guard time |

**Guard Time Calculation:** `Timeout = Guard_Time × Life_Time_Factor`

Example: Guard Time = 1000ms, Life Time Factor = 5 → Timeout = 5000ms

#### Emergency (EMCY)

| Index | Sub | Parameter | Type | Access | Default | Description |
|-------|-----|-----------|------|--------|---------|-------------|
| 0x1014 | - | COB-ID EMCY | UNSIGNED32 | RW | 0x80 + NodeID | Emergency message ID |
| 0x1015 | - | Inhibit Time Emergency | UNSIGNED16 | RW | 0 | Min time between EMCY (×100μs) |
| 0x1029 | 1 | Communication Error | UNSIGNED8 | RW | 0 | Error behavior on comm loss |

#### PDO Configuration

**Receive PDO (RPDO) - Control Objects**

| Index | Name | COB-ID Formula | Mapped Objects | Purpose |
|-------|------|----------------|----------------|---------|
| 0x1400 | RPDO 0 | 0x200 + NodeID | 0x6300 | Relay control |
| 0x1401 | RPDO 1 | 0x300 + NodeID | 0x6411:01-02 | Analog outputs 1-2 |
| 0x1402 | RPDO 2 | 0x400 + NodeID | 0x6411:03-06 | Analog outputs 3-6 |
| 0x1403 | RPDO 3 | 0x500 + NodeID | 0x2302:01-02 | TRIAC 1-2 control |

**Transmit PDO (TPDO) - Status Objects**

| Index | Name | COB-ID Formula | Mapped Objects | Purpose | Typical Period |
|-------|------|----------------|----------------|---------|----------------|
| 0x1800 | TPDO 0 | 0x180 + NodeID | 0x6100:01 | Digital inputs | Event-driven |
| 0x1801 | TPDO 1 | 0x280 + NodeID | 0x6401:01-04 | Analog inputs 1-4 | 500ms |
| 0x1802 | TPDO 2 | 0x380 + NodeID | 0x6401:05-07 | Analog inputs 5-7 | 500ms |
| 0x1803 | TPDO 3 | 0x480 + NodeID | 0x6401:08-11 | Analog inputs 8-11 | 500ms |
| 0x1804 | TPDO 4 | 0x190 + NodeID | 0x2300:01-03 | DOL78 status | On change |
| 0x1805 | TPDO 5 | 0x290 + NodeID | 0x2301:01-04 | Diagnostics | 15s |
| 0x1806 | TPDO 6 | 0x390 + NodeID | 0x6100:02-05 | Counters 1-4 | On change |
| 0x1807 | TPDO 7 | 0x490 + NodeID | 0x6100:06-08 | Counters 5-7 | On change |
| 0x1808 | TPDO 8 | 0x1A0 + NodeID | 0x6100:09-12 | Counters 8-11 | On change |

**PDO Parameters:**

Each PDO has the following sub-indices:
- **Sub 0:** Largest sub-index supported
- **Sub 1:** COB-ID (bit 31 = valid/invalid)
- **Sub 2:** Transmission type (255 = event-driven, 1-240 = cyclic)
- **Sub 3:** Inhibit time (minimum time between transmissions in ×100μs)
- **Sub 5:** Event timer (cyclic transmission period in ms, 0 = disabled)

---

### Manufacturer-Specific Objects (0x2000-0x2FFF)

#### 0x2011 - CAN Error Level

| Sub | Parameter | Type | Access | Description |
|-----|-----------|------|--------|-------------|
| 0 | CAN Error Level | UNSIGNED8 | RW | CAN bus error state (0=ok, 1=warning, 2=error) |

#### 0x2101 - Bootloader Information

| Sub | Parameter | Type | Access | Description |
|-----|-----------|------|--------|-------------|
| 0 | Bootloader Info | UNSIGNED32 | RO | Bootloader version and status |

#### 0x2200 - Detailed Firmware Information

| Sub | Parameter | Type | Access | Description |
|-----|-----------|------|--------|-------------|
| 0 | Number of entries | UNSIGNED8 | RO | 7 |
| 1-7 | FW Details | Various | RO | Firmware build info, checksum, etc. |

#### 0x2201 - Status

| Sub | Parameter | Type | Access | Default | Description |
|-----|-----------|------|--------|---------|-------------|
| 0 | Number of entries | UNSIGNED8 | RO | 4 | - |
| 1 | **Calibration Status** | UNSIGNED8 | RO | 0 | 0=Not calibrated, 1=Calibrated |
| 2 | Status Flag 2 | UNSIGNED8 | RO | 0 | Additional status |
| 3 | Status Flag 3 | UNSIGNED8 | RO | 0 | Additional status |
| 4 | Status Flag 4 | UNSIGNED8 | RO | 0 | Additional status |

**Calibration Status:** The device must be calibrated at the factory. Value 1 indicates valid calibration data is present.

#### 0x2202 - Settings

| Sub | Parameter | Type | Access | Description |
|-----|-----------|------|--------|-------------|
| 0 | Number of entries | UNSIGNED8 | RO | 10 |
| 1-10 | Configuration Settings | Various | RW | Device-specific configuration parameters |

#### 0x2300 - DOL278 Module Status

**DOL278** is an optional SKOV expansion module with additional I/O and 24V power monitoring.

| Sub | Parameter | Type | Access | Unit | Description |
|-----|-----------|------|--------|------|-------------|
| 0 | Number of entries | UNSIGNED8 | RO | - | 3 |
| 1 | **DOL78 24V** | UNSIGNED16 | RO | mV | 24V supply voltage |
| 2 | **DOL78 House 1** | UNSIGNED16 | RO | - | House zone 1 status |
| 3 | **DOL78 House 2** | UNSIGNED16 | RO | - | House zone 2 status |

**Voltage Scaling:** Value in mV (e.g., 24195 = 24.195V)

**PDO Mapping:** Can be transmitted via TPDO 4 (0x190 + NodeID)

#### 0x2301 - Internal Measurements

Critical diagnostic data transmitted periodically (typically every 15 seconds via TPDO 5).

| Sub | Parameter | Type | Access | Unit | Range | Description |
|-----|-----------|------|--------|------|-------|-------------|
| 0 | Number of entries | UNSIGNED8 | RO | - | - | 3 |
| 1 | **Internal Temperature** | UNSIGNED16 | RO | °C × 10 | 0-1000 | Device internal temp |
| 2 | **Internal 15V** | UNSIGNED16 | RO | mV | 0-20000 | 15V rail voltage |
| 3 | **Internal 24V** | UNSIGNED16 | RO | mV | 0-30000 | 24V rail voltage |
| 4 | **Net Period Time** | UNSIGNED16 | RO | μs | 16666-20000 | AC mains period (50/60Hz) |

**Decoding Example:**

```
Raw Data: 0x0136 0x3BCE 0x5E83 0x4E20
Decoded:
  Temperature: 0x0136 = 310 → 31.0°C
  15V Rail: 0x3BCE = 15310 → 15.310V
  24V Rail: 0x5E83 = 24195 → 24.195V
  Net Period: 0x4E20 = 20000μs → 50Hz mains
```

#### 0x2302 - TRIAC Control

Phase-cut control for AC loads (heaters, dimmers, etc.).

| Sub | Parameter | Type | Access | Unit | Range | Description |
|-----|-----------|------|--------|------|-------|-------------|
| 0 | Number of entries | UNSIGNED8 | RO | - | - | 3 |
| 1 | **TRIAC 1 ON Time** | UNSIGNED16 | RWW | μs | 0-10000 | On-time per AC cycle |
| 2 | **TRIAC 2 ON Time** | UNSIGNED16 | RWW | μs | 0-10000 | On-time per AC cycle |

**Control Method:** Value represents microseconds of conduction per AC half-cycle
- 0 = OFF (no conduction)
- 10000 = Full power (full cycle conduction for 50Hz)
- 5000 = ~50% power

**Access Type:** RWW = Read/Write, value takes effect immediately (Write Window)

**PDO Mapping:** Controllable via RPDO 3 (0x510 + NodeID)

#### 0x2400 - Configure Amount of Extra Analog Outputs

| Index | Parameter | Type | Access | Range | Description |
|-------|-----------|------|--------|-------|-------------|
| 0x2400 | Extra AO Count | UNSIGNED8 | RW | 0/2/4 | Number of expansion outputs |

**Valid Values:**
- **0:** No extra analog outputs (only main board outputs)
- **2:** B-terminal outputs only
- **4:** Full expansion (both A and B terminals)

**Note:** This must be configured before entering operational state.

#### 0x2401 - Analog Input Configuration

Configures each analog input channel's operating mode.

| Sub | Parameter | Type | Access | Range | Description |
|-----|-----------|------|--------|-------|-------------|
| 0 | Number of entries | UNSIGNED8 | RO | - | 7 |
| 1 | **AI 1 Config** | UNSIGNED8 | RW | 0-2 | Input type selection |
| 2 | **AI 2 Config** | UNSIGNED8 | RW | 0-2 | Input type selection |
| 3 | **AI 3 Config** | UNSIGNED8 | RW | 0-2 | Input type selection |
| 4 | **AI 4 Config** | UNSIGNED8 | RW | 0-2 | Input type selection |
| 5 | **AI 5 Config** | UNSIGNED8 | RW | 0-2 | Input type selection |
| 6 | **AI 6 Config** | UNSIGNED8 | RW | 0-2 | Input type selection |
| 7 | **AI 7 Config** | UNSIGNED8 | RW | 0-2 | Input type selection |

**Configuration Values:**
- **0 = AI (Analog Input):** Direct voltage measurement (0-10V)
- **1 = DOL12:** SKOV temperature sensor (NTC with lookup table)
- **2 = DI (Digital Input):** Binary state detection

**Default:** Typically 0 (AI mode)

---

### Process Data Objects (0x6000-0x6FFF)

#### 0x6100 - Digital Inputs

| Sub | Parameter | Type | Access | Description |
|-----|-----------|------|--------|-------------|
| 0 | Number of entries | UNSIGNED8 | RO | 12 |
| 1 | **Digital Input States** | UNSIGNED16 | RO | Bit-mapped input states (bits 0-11) |
| 2 | **Counter 1** | UNSIGNED16 | RO | Pulse counter for DI1 |
| 3 | **Counter 2** | UNSIGNED16 | RO | Pulse counter for DI2 |
| 4 | **Counter 3** | UNSIGNED16 | RO | Pulse counter for DI3 |
| 5 | **Counter 4** | UNSIGNED16 | RO | Pulse counter for DI4 |
| 6 | **Counter 5** | UNSIGNED16 | RO | Pulse counter for DI5 |
| 7 | **Counter 6** | UNSIGNED16 | RO | Pulse counter for DI6 |
| 8 | **Counter 7** | UNSIGNED16 | RO | Pulse counter for DI7 |
| 9 | **Counter 8** | UNSIGNED16 | RO | Pulse counter for DI8 |
| 10 | **Counter 9** | UNSIGNED16 | RO | Pulse counter for DI9 |
| 11 | **Counter 10** | UNSIGNED16 | RO | Pulse counter for DI10 |
| 12 | **Counter 11** | UNSIGNED16 | RO | Pulse counter for DI11 |

**Bit Mapping (Sub 1 - Digital Input States):**

```
Bit:  15 14 13 12 | 11 10  9  8 | 7  6  5  4 | 3  2  1  0
      Reserved    | DI11-DI8    | DI7-DI4    | DI3-DI0
```

**Example:**
```
Value: 0x0805 = 0b0000 1000 0000 0101
         DI11=1, DI2=1, DI0=1 (all others = 0)
```

**Counters:**
- Edge-triggered (rising edge default)
- 16-bit counters (0-65535, wraps around)
- Can be reset via SDO write
- Used for flow meters, anemometers, etc.

#### 0x6106 - Interrupt Mask Any Change

| Sub | Parameter | Type | Access | Description |
|-----|-----------|------|--------|-------------|
| 0 | Number of entries | UNSIGNED8 | RO | 3 |
| 1 | DI Interrupt Mask 1 | UNSIGNED8 | RW | Enable change detection for DI 0-7 |
| 2 | DI Interrupt Mask 2 | UNSIGNED8 | RW | Enable change detection for DI 8-11 |

**Usage:** Enable event-driven TPDO transmission when specific digital inputs change state.

#### 0x6300 - Relay Outputs

| Sub | Parameter | Type | Access | Description |
|-----|-----------|------|--------|-------------|
| 0 | Number of entries | UNSIGNED8 | RO | 1 |
| 1 | **Relay States** | UNSIGNED16 | WO | Bit-mapped relay control (bits 0-11) |

**Bit Mapping:**

```
Bit:  15 14 13 12 | 11 10  9  8 | 7  6  5  4 | 3  2  1  0
      Reserved    | R11-R8      | R7-R4      | R3-R0
```

**Control:**
- Bit = 1: Relay ON (energized)
- Bit = 0: Relay OFF (de-energized)

**Example:**
```javascript
// Turn on relays 0, 1, and 5
const relayValue = (1 << 0) | (1 << 1) | (1 << 5);
// relayValue = 0x0023 = 0b0000 0000 0010 0011
```

**PDO Control:** Writable via RPDO 0 (COB-ID 0x210 + NodeID)

**Safety Notes:**
- Relays default to OFF on power-up
- NMT state STOPPED → all relays OFF
- Communication timeout can trigger safety state (all OFF)

#### 0x6401 - Analog Inputs

| Sub | Parameter | Type | Access | Unit | Range | Description |
|-----|-----------|------|--------|------|-------|-------------|
| 0 | Number of entries | UNSIGNED8 | RO | - | - | 11 |
| 1 | **Analog Input 1** | UNSIGNED16 | RO | mV | 0-10000 | AI1 voltage |
| 2 | **Analog Input 2** | UNSIGNED16 | RO | mV | 0-10000 | AI2 voltage |
| 3 | **Analog Input 3** | UNSIGNED16 | RO | mV | 0-10000 | AI3 voltage |
| 4 | **Analog Input 4** | UNSIGNED16 | RO | mV | 0-10000 | AI4 voltage |
| 5 | **Analog Input 5** | UNSIGNED16 | RO | mV | 0-10000 | AI5 voltage |
| 6 | **Analog Input 6** | UNSIGNED16 | RO | mV | 0-10000 | AI6 voltage |
| 7 | **Analog Input 7** | UNSIGNED16 | RO | mV | 0-10000 | AI7 voltage |
| 8 | **Analog Input 8** | UNSIGNED16 | RO | mV | 0-10000 | AI8 voltage |
| 9 | **Analog Input 9** | UNSIGNED16 | RO | mV | 0-10000 | AI9 voltage |
| 10 | **Analog Input 10** | UNSIGNED16 | RO | mV | 0-10000 | AI10 voltage |
| 11 | **Analog Input 11** | UNSIGNED16 | RO | mV | 0-10000 | AI11 voltage |

**Voltage Scaling:** All values in millivolts
- Raw value 5000 = 5.000V
- Raw value 8080 = 8.080V

**Input Mode Dependency:**
- **AI mode (0x2401:x = 0):** Direct voltage (0-10000 mV)
- **DOL12 mode (0x2401:x = 1):** Sensor voltage (1900-8080 mV typical)
- **DI mode (0x2401:x = 2):** Binary threshold (~0 or ~10000 mV)

**PDO Transmission:**
- AI 1-4 → TPDO 1 (0x1A1 + NodeID) every 500ms
- AI 5-7 → TPDO 2 (0x1C1 + NodeID) every 500ms
- AI 8-11 → TPDO 3 (0x1E1 + NodeID) every 500ms

#### 0x6411 - Analog Outputs

| Sub | Parameter | Type | Access | Unit | Range | Description |
|-----|-----------|------|--------|------|-------|-------------|
| 0 | Number of entries | UNSIGNED8 | RO | - | - | 6 |
| 1 | **Analog Output 1** | UNSIGNED16 | RWW | mV | 0-10000 | AO1 voltage setpoint |
| 2 | **Analog Output 2** | UNSIGNED16 | RWW | mV | 0-10000 | AO2 voltage setpoint |
| 3 | **Analog Output 3** | UNSIGNED16 | RWW | mV | 0-10000 | AO3 voltage setpoint |
| 4 | **Analog Output 4** | UNSIGNED16 | RWW | mV | 0-10000 | AO4 voltage setpoint |
| 5 | **Analog Output 5** | UNSIGNED16 | RWW | mV | 0-10000 | AO5 voltage setpoint |
| 6 | **Analog Output 6** | UNSIGNED16 | RWW | mV | 0-10000 | AO6 voltage setpoint |

**Voltage Scaling:** All values in millivolts
- Write 5000 → Output 5.000V
- Write 10000 → Output 10.000V

**Output Availability:**
- AO1-AO2: Always available (main board)
- AO3-AO6: Requires 0x2400 = 2 or 4 (expansion boards)

**PDO Control:**
- AO1-AO2 → RPDO 1 (0x310 + NodeID)
- AO3-AO6 → RPDO 2 (0x410 + NodeID)

**Default Values:** 0 mV (outputs off)

#### 0x6426 - Analog Input Interrupt Delta

| Sub | Parameter | Type | Access | Unit | Description |
|-----|-----------|------|--------|------|-------------|
| 0 | Number of entries | UNSIGNED8 | RO | - | 11 |
| 1-11 | AI Delta Threshold | UNSIGNED16 | RW | mV | Change threshold for event PDO |

**Purpose:** Configure change detection sensitivity for event-driven TPDO transmission.

**Example:**
- Set 0x6426:01 = 100 (100mV threshold)
- AI1 changes by ≥100mV → TPDO 1 transmitted immediately
- Prevents excessive PDO traffic on minor fluctuations

---

## PDO Communication

### PDO Mapping Summary

#### Transmit PDOs (Device → Master)

| TPDO | COB-ID | Period | Data Content | Bytes |
|------|--------|--------|--------------|-------|
| **0** | 0x180+ID | Event | Digital input states (0x6100:01) | 2 |
| **1** | 0x1A0+ID | 500ms | AI1-AI4 values (0x6401:01-04) | 8 |
| **2** | 0x1C0+ID | 500ms | AI5-AI7 values (0x6401:05-07) | 6 |
| **3** | 0x1E0+ID | 500ms | AI8-AI11 values (0x6401:08-11) | 8 |
| **4** | 0x190+ID | Event | DOL78 status (0x2300:01-03) | 6 |
| **5** | 0x290+ID | 15s | Diagnostics (0x2301:01-04) | 8 |
| **6** | 0x390+ID | Event | Counters 1-4 (0x6100:02-05) | 8 |
| **7** | 0x490+ID | Event | Counters 5-7 (0x6100:06-08) | 6 |
| **8** | 0x1A0+ID | Event | Counters 8-11 (0x6100:09-12) | 8 |

#### Receive PDOs (Master → Device)

| RPDO | COB-ID | Data Content | Bytes | Control |
|------|--------|--------------|-------|---------|
| **0** | 0x210+ID | Relay states (0x6300:01) | 2 | Digital outputs |
| **1** | 0x310+ID | AO1-AO2 (0x6411:01-02) | 4 | Analog outputs 1-2 |
| **2** | 0x410+ID | AO3-AO6 (0x6411:03-06) | 8 | Analog outputs 3-6 |
| **3** | 0x510+ID | TRIAC 1-2 (0x2302:01-02) | 4 | Phase control |

### PDO Data Encoding

All multi-byte values use **Little-Endian** byte order (LSB first).

**Example: TPDO 1 (Analog Inputs 1-4)**

```
CAN ID: 0x1A0 + NodeID
Data Layout:
  Byte 0-1: AI1 (UINT16 LE)
  Byte 2-3: AI2 (UINT16 LE)
  Byte 4-5: AI3 (UINT16 LE)
  Byte 6-7: AI4 (UINT16 LE)

Example Message:
  ID: 0x1A1 (NodeID = 0x01)
  Data: [0x10, 0x00, 0x12, 0x00, 0x08, 0x00, 0x12, 0x00]
  
  Decoded:
    AI1 = 0x0010 = 16 mV
    AI2 = 0x0012 = 18 mV
    AI3 = 0x0008 = 8 mV
    AI4 = 0x0012 = 18 mV
```

**Example: TPDO 5 (Internal Measurements)**

```
CAN ID: 0x290 + NodeID
Data Layout:
  Byte 0-1: Internal Temperature (UINT16 LE, °C × 10)
  Byte 2-3: Internal 15V (UINT16 LE, mV)
  Byte 4-5: Internal 24V (UINT16 LE, mV)
  Byte 6-7: Net Period Time (UINT16 LE, μs)

Example Message:
  ID: 0x2A0 (NodeID = 0x10)
  Data: [0x36, 0x01, 0xCE, 0x3B, 0x83, 0x5E, 0x20, 0x4E]
  
  Decoded:
    Temperature = 0x0136 = 310 → 31.0°C
    15V Rail = 0x3BCE = 15310 → 15.310V
    24V Rail = 0x5E83 = 24195 → 24.195V
    Net Period = 0x4E20 = 20000 → 20ms (50Hz)
```

**Example: RPDO 0 (Relay Control)**

```
CAN ID: 0x210 + NodeID
Data Layout:
  Byte 0-1: Relay states (UINT16 LE, bit-mapped)

Control Message (Turn ON relays 0, 4, 8):
  ID: 0x211 (NodeID = 0x01)
  Data: [0x11, 0x01]
  
  Binary: 0x0111 = 0b0000 0001 0001 0001
    Bit 0 = 1 → Relay 0 ON
    Bit 4 = 1 → Relay 4 ON
    Bit 8 = 1 → Relay 8 ON
```

### PDO Transmission Types

| Type Value | Description | Usage |
|------------|-------------|-------|
| 0 | Synchronous (acyclic) | Not used by SKOV |
| 1-240 | Synchronous (cyclic) | Every Nth SYNC |
| 254 | Event-driven (manufacturer) | On threshold change |
| 255 | Event-driven (device) | On any change or cyclic |

**SKOV Defaults:**
- **Type 255:** Most TPDOs (event or cyclic based on Event Timer)
- **Inhibit Time:** 100 (= 10ms minimum between PDOs)
- **Event Timer:** 0 (disabled) or specific period (e.g., 500ms for analog inputs)

---

## Analog Input Protocol

### Input Modes

The SKOV Main IO supports three analog input modes per channel, configured via 0x2401:

#### Mode 0: Analog Input (AI)

- **Range:** 0-10V DC
- **Resolution:** 16-bit (0-10000 mV)
- **Use Case:** Generic voltage measurement, 0-10V sensors
- **Encoding:** Direct voltage in millivolts

**Example:**
```
Sensor Output: 3.5V
Raw Value: 3500 (decimal) = 0x0DAC (hex)
```

#### Mode 1: DOL12 Temperature Sensor

- **Range:** -40°C to +100°C
- **Sensor Type:** SKOV proprietary NTC thermistor
- **Voltage Range:** 1.90V to 8.08V (inverted characteristic)
- **Encoding:** Voltage in millivolts (requires lookup table for temperature)

**See:** [DOL12 Sensor Protocol](#dol12-sensor-protocol) section for conversion details.

#### Mode 2: Digital Input (DI)

- **Threshold:** ~5V (typically)
- **Logic Levels:**
  - LOW: 0-2V → Value ≈ 0-2000 mV
  - HIGH: 8-10V → Value ≈ 8000-10000 mV
- **Use Case:** Binary sensors, switches, dry contacts

**Example:**
```
Switch CLOSED → Voltage ≈ 0V → Value ≈ 0
Switch OPEN → Voltage ≈ 10V → Value ≈ 10000
```

### Analog Input Sampling

- **Sample Rate:** Typically 10-100ms per channel
- **Filtering:** Hardware low-pass filter (noise reduction)
- **PDO Update:** 500ms for real-time channels (TPDO 1-3)
- **Resolution:** 16-bit ADC → 0.15mV per LSB for 0-10V range

---

## Digital I/O Protocol

### Digital Inputs

**Physical Characteristics:**
- **Type:** Optoisolated inputs (galvanic isolation)
- **Voltage Range:** 0-24V DC
- **Logic Levels:**
  - LOW: 0-5V
  - HIGH: 10-30V (24V nominal)
- **Count:** 12 inputs (DI0-DI11)

**Features:**
- Real-time state reporting via TPDO 0
- Pulse counting (16-bit counters per input)
- Edge detection (rising/falling/both)
- Interrupt-driven event PDOs

**Counter Mode:**
- Counts rising edges (default)
- 16-bit counter (0-65535, wraps around)
- Frequency range: DC to several kHz
- Applications: Flow meters, wind sensors, pulse outputs

**PDO Mapping:**
- States → TPDO 0 (0x181 for NodeID=1) - event-driven
- Counters → TPDO 6/7/8 - on change or periodic

### Digital Outputs (Relays)

**Physical Characteristics:**
- **Type:** Electromechanical relays (SPST)
- **Count:** 12 outputs + 1 alarm relay
- **Rating:** Typically 5A @ 250VAC, 5A @ 30VDC
- **Isolation:** Galvanic isolation from control circuit

**Control:**
- Bit-mapped control via 0x6300:01
- RPDO 0 for real-time control
- Individual bit per relay (bit 0 = relay 0, etc.)

**Safety Features:**
- Power-on default: All OFF
- NMT STOPPED state: All OFF
- Watchdog timeout: Configurable behavior (all OFF typical)
- Emergency stop: All OFF via EMCY or NMT command

**Typical Applications:**
- **Relay 0-7:** Ventilation fans, heaters, alarms
- **Relay 8-11:** Auxiliary equipment, pumps, motors
- **Alarm Relay:** Emergency alarm output (hardware controlled)

---

## DOL12 Sensor Protocol

### Overview

**DOL12** is a SKOV proprietary temperature sensor using an NTC (Negative Temperature Coefficient) thermistor with a specific voltage-temperature characteristic curve.

**Key Characteristics:**
- **Temperature Range:** -40°C to +100°C
- **Voltage Range:** 8.08V to 1.90V (decreases with increasing temperature)
- **Connection:** 2-wire, powered by analog input module
- **Accuracy:** Typically ±0.5°C (when calibrated)

### Voltage-Temperature Lookup Table

The DOL12 sensor follows a non-linear characteristic. Conversion requires a lookup table with linear interpolation.

**Selected Calibration Points:**

| Temperature (°C) | Voltage (V) | Temperature (°C) | Voltage (V) |
|------------------|-------------|------------------|-------------|
| -40 | 8.08 | 20 | 4.93 |
| -35 | 7.96 | 25 | 4.57 |
| -30 | 7.83 | 30 | 4.23 |
| -25 | 7.68 | 35 | 3.91 |
| -20 | 7.49 | 40 | 3.61 |
| -15 | 7.26 | 50 | 3.09 |
| -10 | 7.00 | 60 | 2.68 |
| -5 | 6.70 | 70 | 2.36 |
| 0 | 6.37 | 80 | 2.11 |
| 5 | 6.02 | 90 | 1.95 |
| 10 | 5.66 | 100 | 1.90 |
| 15 | 5.29 | - | - |

**Full Table:** 66 calibration points from -40°C to +100°C (see `dol12.js` in emulator code)

### Temperature Conversion Algorithm

#### Voltage to Temperature

```javascript
function voltageToTemperature(voltage) {
    // Handle out-of-range values
    if (voltage >= 8.08) return -40.0;  // Below minimum temp
    if (voltage <= 1.90) return 100.0;  // Above maximum temp
    
    // Find bounding points in lookup table
    let lower, upper;
    for (let i = 0; i < lookupTable.length - 1; i++) {
        if (lookupTable[i].voltage >= voltage && 
            lookupTable[i+1].voltage <= voltage) {
            lower = lookupTable[i];
            upper = lookupTable[i+1];
            break;
        }
    }
    
    // Linear interpolation
    const tempRange = upper.temp - lower.temp;
    const voltRange = upper.voltage - lower.voltage;
    const voltOffset = voltage - lower.voltage;
    
    return lower.temp + (voltOffset / voltRange) * tempRange;
}
```

#### Temperature to Voltage

```javascript
function temperatureToVoltage(temp) {
    // Handle out-of-range values
    if (temp <= -40) return 8.08;
    if (temp >= 100) return 1.90;
    
    // Find bounding points in lookup table
    let lower, upper;
    for (let i = 0; i < lookupTable.length - 1; i++) {
        if (lookupTable[i].temp <= temp && 
            lookupTable[i+1].temp >= temp) {
            lower = lookupTable[i];
            upper = lookupTable[i+1];
            break;
        }
    }
    
    // Linear interpolation
    const voltRange = upper.voltage - lower.voltage;
    const tempRange = upper.temp - lower.temp;
    const tempOffset = temp - lower.temp;
    
    return lower.voltage + (tempOffset / tempRange) * voltRange;
}
```

### Integration with SKOV Device

**Configuration:**

1. Set input mode to DOL12:
```
SDO Write: 0x2401:01 = 1  (Configure AI1 as DOL12)
```

2. Read sensor voltage:
```
SDO Read: 0x6401:01 → Returns voltage in mV (e.g., 5290 = 5.290V)
Or via TPDO 1 (real-time data)
```

3. Convert to temperature:
```
Voltage: 5290 mV = 5.29V
Lookup: 5.29V ≈ 15°C (per DOL12 table)
```

**PDO Data:**

The device transmits the **raw voltage** value, not the temperature. Temperature conversion must be performed by the master/SCADA system using the DOL12 lookup table.

```
TPDO 1 Data for NodeID 0x10:
  CAN ID: 0x1A0 + 0x10 = 0x1B0
  Data: [0xAA, 0x14, ...]  // AI1 = 0x14AA = 5290 mV
  
  Master Conversion: 5290 mV → 5.29V → ~15°C
```

### Typical Use Cases

- **Poultry Houses:** Ambient temperature monitoring
- **Pig Farms:** Zone temperature control
- **Greenhouses:** Climate monitoring
- **Storage Facilities:** Cold room monitoring

---

## DOL78 Module Protocol

### Overview

**DOL78** (also referenced as DOL278 in EDS) is an optional SKOV expansion module that provides:
- Additional I/O capacity
- 24V power supply monitoring
- Zone-based control capabilities
- Enhanced diagnostics

**Communication:** Integrated into Main IO via internal bus, appears as object 0x2300 in CANopen dictionary.

### Object Dictionary (0x2300)

| Sub | Parameter | Type | Unit | Description |
|-----|-----------|------|------|-------------|
| 1 | DOL78 24V | UINT16 | mV | 24V DC supply voltage |
| 2 | DOL78 House 1 | UINT16 | - | House zone 1 status/data |
| 3 | DOL78 House 2 | UINT16 | - | House zone 2 status/data |

### 24V Supply Monitoring

**Purpose:** Monitor power supply health for predictive maintenance and safety.

**Voltage Encoding:**
```
Raw Value in mV
Example: 24195 = 24.195V

Typical Range: 22000-26000 mV (22-26V)
Alarm Thresholds:
  - Below 20V: Under-voltage alarm
  - Above 28V: Over-voltage alarm
```

**PDO Transmission:** Via TPDO 4 (0x190 + NodeID), event-driven on significant change

**Typical Monitoring Logic:**
```javascript
if (dol78_24v < 22000) {
    // Under-voltage warning
    // Check power supply, connections
}
if (dol78_24v > 26000) {
    // Over-voltage warning
    // Risk of equipment damage
}
```

### House Zone Status

**Purpose:** Multi-zone control for large agricultural buildings divided into sections.

**Data Encoding:** Application-specific, typically:
- Temperature setpoint
- Ventilation stage
- Alarm status
- Custom control parameters

**Example Usage:**
```
House 1: Broiler section (young birds)
  - Target temp: 32°C
  - Status value: Encoded as UINT16
  
House 2: Finisher section (older birds)
  - Target temp: 22°C
  - Status value: Different encoding
```

**Note:** Exact encoding is application-specific and configured via SKOV setup software.

---

## Initialization Sequence

### Typical CANopen Master Boot Sequence

This sequence demonstrates how a SKOV Main IO device typically initializes when a CANopen master boots up the network.

#### Phase 1: Network Discovery

```
Time: T+0ms
CAN Message: ID=0x000, Data=[0x81, 0x10]
Description: NMT Command - Reset Node (NodeID 0x10)
Effect: Device performs soft reset, enters Pre-operational state
```

```
Time: T+50ms
CAN Message: ID=0x710, Data=[0x00]
Description: NMT Boot-up message from NodeID 0x10
Effect: Device signals it's ready for configuration
```

#### Phase 2: Device Identification

**Step 1: Read Device Name**
```
Master TX: ID=0x610, Data=[0x40, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00]
  SDO Upload Request: Read 0x1008 (Device Name)
  
Device RX: ID=0x590, Data=[0x43, 0x08, 0x10, 0x00, 0x53, 0x4D, 0x30, 0x30]
  SDO Upload Response: Value = "SM00" (ASCII)
```

**Step 2: Read Device Type**
```
Master TX: ID=0x610, Data=[0x40, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00]
  SDO Upload Request: Read 0x1000 (Device Type)
  
Device RX: ID=0x590, Data=[0x43, 0x00, 0x10, 0x00, 0x91, 0x01, 0x0E, 0x00]
  SDO Upload Response: Value = 0x000E0191
```

**Step 3: Read Software Version**
```
Master TX: ID=0x610, Data=[0x40, 0x0A, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00]
  SDO Upload Request: Read 0x100A (SW Version)
  
Device RX: ID=0x590, Data=[0x43, 0x0A, 0x10, 0x00, 0x35, 0x32, 0x30, 0x32]
  SDO Upload Response: Value = "5202" (ASCII)
```

**Step 4: Read Hardware Revision**
```
Master TX: ID=0x610, Data=[0x40, 0x18, 0x10, 0x03, 0x00, 0x00, 0x00, 0x00]
  SDO Upload Request: Read 0x1018:03 (Revision Number)
  
Device RX: ID=0x590, Data=[0x43, 0x18, 0x10, 0x03, 0x1D, 0x00, 0x00, 0x00]
  SDO Upload Response: Value = 29 (0x1D)
```

**Step 5: Read Serial Number**
```
Master TX: ID=0x610, Data=[0x40, 0x18, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00]
  SDO Upload Request: Read 0x1018:04 (Serial Number)
  
Device RX: ID=0x590, Data=[0x43, 0x18, 0x10, 0x04, 0x04, 0x2C, 0xFC, 0x05]
  SDO Upload Response: Value = 100412420 (0x05FC2C04)
  
Device ID: "100412420-29"
```

#### Phase 3: Configuration

**Step 6: Read Hardware Configuration**
```
Master TX: ID=0x610, Data=[0x40, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00]
  SDO Upload Request: Read 0x2400 (Extra AO Count)
  
Device RX: ID=0x590, Data=[0x4F, 0x00, 0x24, 0x00, 0x02, 0x00, 0x00, 0x00]
  SDO Upload Response: Value = 2 (B-terminals only)
```

**Step 7: Configure Guard Time (Optional)**
```
Master TX: ID=0x610, Data=[0x2B, 0x0C, 0x10, 0x00, 0xE8, 0x03, 0x00, 0x00]
  SDO Download Request: Write 0x100C = 1000ms
  
Device RX: ID=0x590, Data=[0x60, 0x0C, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00]
  SDO Download Response: OK
```

**Step 8: Configure Life Time Factor (Optional)**
```
Master TX: ID=0x610, Data=[0x2F, 0x0D, 0x10, 0x00, 0x05, 0x00, 0x00, 0x00]
  SDO Download Request: Write 0x100D = 5
  
Device RX: ID=0x590, Data=[0x60, 0x0D, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00]
  SDO Download Response: OK
  
Watchdog Timeout = 1000ms × 5 = 5000ms
```

**Step 9: Verify Calibration Status**
```
Master TX: ID=0x610, Data=[0x40, 0x01, 0x22, 0x01, 0x00, 0x00, 0x00, 0x00]
  SDO Upload Request: Read 0x2201:01 (Calibration Status)
  
Device RX: ID=0x590, Data=[0x4F, 0x01, 0x22, 0x01, 0x01, 0x00, 0x00, 0x00]
  SDO Upload Response: Value = 1 (Calibrated)
```

#### Phase 4: Enter Operational State

```
Time: T+1000ms
CAN Message: ID=0x000, Data=[0x01, 0x10]
Description: NMT Command - Start Remote Node (NodeID 0x10)
Effect: Device enters Operational state, PDO transmission begins
```

```
Time: T+1010ms
CAN Message: ID=0x710, Data=[0x05]
Description: Heartbeat - State = Operational (0x05)
Effect: Device confirms operational state
```

#### Phase 5: PDO Transmission Begins

**First PDO Messages (Real-time data):**

```
Time: T+1020ms
CAN Message: ID=0x181, Data=[0x00]
Description: TPDO 0 - Digital inputs all LOW
```

```
Time: T+1050ms
CAN Message: ID=0x1A1, Data=[0x10, 0x00, 0x12, 0x00, 0x08, 0x00, 0x12, 0x00]
Description: TPDO 1 - Analog inputs 1-4
  AI1 = 16mV, AI2 = 18mV, AI3 = 8mV, AI4 = 18mV
```

```
Time: T+1070ms
CAN Message: ID=0x1C1, Data=[0x12, 0x00, 0x08, 0x00, 0x19, 0x00, 0x15, 0x00]
Description: TPDO 2 - Analog inputs 5-7
  AI5 = 18mV, AI6 = 8mV, AI7 = 25mV
```

```
Time: T+1090ms
CAN Message: ID=0x1E1, Data=[0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]
Description: TPDO 3 - Analog inputs 8-11 (all zero, not used)
```

**Periodic PDOs (500ms cycle):**
- TPDO 1, 2, 3 repeat every 500ms with updated analog values

**Diagnostic PDO (15s cycle):**

```
Time: T+15000ms
CAN Message: ID=0x2A0, Data=[0x36, 0x01, 0xCE, 0x3B, 0x83, 0x5E, 0x20, 0x4E]
Description: TPDO 5 - Internal measurements
  Temperature = 31.0°C
  15V Rail = 15.310V
  24V Rail = 24.195V
  Net Period = 20000μs (50Hz)
```

**Heartbeat (Periodic):**

```
Time: T+2000ms, T+3000ms, T+4000ms...
CAN Message: ID=0x710, Data=[0x05] or [0x85]
Description: Heartbeat toggle (alternates between 0x05 and 0x85)
```

### Configuration Checklist

Before entering operational state, ensure:

- ✓ Device identity verified (Device Type, Serial Number)
- ✓ Software version compatible with application
- ✓ Calibration status = 1 (Calibrated)
- ✓ Extra AO count configured correctly
- ✓ Guard time/heartbeat configured (if required)
- ✓ PDO communication parameters set
- ✓ Analog input modes configured (AI/DOL12/DI)
- ✓ RPDO COB-IDs enabled (bit 31 clear)

---

## Data Encoding

### Byte Order

**All multi-byte values use Little-Endian encoding:**
- Least Significant Byte (LSB) first
- Most Significant Byte (MSB) last

**Examples:**

```
16-bit Value: 0x1234
  Byte 0: 0x34 (LSB)
  Byte 1: 0x12 (MSB)
  
32-bit Value: 0x12345678
  Byte 0: 0x78 (LSB)
  Byte 1: 0x56
  Byte 2: 0x34
  Byte 3: 0x12 (MSB)
```

### Data Type Encoding

| CANopen Type | Size | Range | SKOV Usage |
|--------------|------|-------|------------|
| UNSIGNED8 | 1 byte | 0-255 | Flags, states, small counts |
| UNSIGNED16 | 2 bytes | 0-65535 | Voltages (mV), temperatures (×10), counters |
| UNSIGNED32 | 4 bytes | 0-4.29B | Device IDs, serial numbers |
| INTEGER16 | 2 bytes | -32768 to +32767 | Signed values (rarely used) |
| STRING | Variable | ASCII | Device name, versions |

### Scaling Factors

| Parameter | Data Type | Scaling | Range | Example |
|-----------|-----------|---------|-------|---------|
| **Temperature** | UINT16 | °C × 10 | 0-1000 | 310 = 31.0°C |
| **Voltage (mV)** | UINT16 | mV | 0-30000 | 24195 = 24.195V |
| **Voltage (V)** | UINT16 | V × 1000 | 0-30 | 15310 = 15.310V |
| **Time (μs)** | UINT16 | μs | 0-65535 | 20000 = 20ms |
| **Time (ms)** | UINT16 | ms | 0-65535 | 500 = 0.5s |
| **Percentage** | UINT16 | % × 100 | 0-10000 | 7550 = 75.50% |
| **Frequency** | UINT16 | Hz × 10 | 0-6553.5 | 500 = 50.0Hz |

### Bit Field Encoding

**Digital Inputs (0x6100:01):**
```
Bit Position: 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
Meaning:      -- -- -- -- DI11-DI8     DI7-DI4     DI3-DI0

Example: 0x0A05 = 0b0000 1010 0000 0101
  Bit 0 = 1 → DI0 = HIGH
  Bit 2 = 1 → DI2 = HIGH
  Bit 9 = 1 → DI9 = HIGH
  Bit 11 = 1 → DI11 = HIGH
```

**Relay Outputs (0x6300:01):**
```
Bit Position: 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
Meaning:      -- -- -- --  R11-R8      R7-R4      R3-R0

Example: 0x0F03 = 0b0000 1111 0000 0011
  Bits 0-1 = 11 → Relays 0-1 ON
  Bits 8-11 = 1111 → Relays 8-11 ON
```

### SDO Encoding

**Expedited Transfer (≤4 bytes):**

**Upload Request (Read):**
```
Byte 0: 0x40 (Command Specifier)
Byte 1: Index LSB
Byte 2: Index MSB
Byte 3: Subindex
Bytes 4-7: Reserved (0x00)
```

**Upload Response:**
```
Byte 0: 0x43 (4 bytes data) / 0x47 (3 bytes) / 0x4B (2 bytes) / 0x4F (1 byte)
Byte 1: Index LSB
Byte 2: Index MSB
Byte 3: Subindex
Bytes 4-7: Data (Little-Endian)
```

**Download Request (Write):**
```
Byte 0: 0x23 (4 bytes) / 0x27 (3 bytes) / 0x2B (2 bytes) / 0x2F (1 byte)
Byte 1: Index LSB
Byte 2: Index MSB
Byte 3: Subindex
Bytes 4-7: Data (Little-Endian)
```

**Download Response:**
```
Byte 0: 0x60 (Success)
Byte 1: Index LSB
Byte 2: Index MSB
Byte 3: Subindex
Bytes 4-7: Reserved (0x00)
```

**Example: Read Analog Input 1**
```
Request:  [0x40, 0x01, 0x64, 0x01, 0x00, 0x00, 0x00, 0x00]
          Read 0x6401:01
Response: [0x4B, 0x01, 0x64, 0x01, 0xAA, 0x14, 0x00, 0x00]
          Value = 0x14AA = 5290 mV
```

**Example: Write Relay States**
```
Request:  [0x2B, 0x00, 0x63, 0x01, 0x07, 0x00, 0x00, 0x00]
          Write 0x6300:01 = 0x0007 (Relays 0, 1, 2 ON)
Response: [0x60, 0x00, 0x63, 0x01, 0x00, 0x00, 0x00, 0x00]
          Write successful
```

---

## Examples

### Example 1: Reading Temperature from DOL12 Sensor

**Scenario:** AI1 is configured as DOL12, read current temperature.

**Step 1: Verify Configuration**
```
SDO Upload: 0x2401:01
Request:  ID=0x610, [0x40, 0x01, 0x24, 0x01, 0x00, 0x00, 0x00, 0x00]
Response: ID=0x590, [0x4F, 0x01, 0x24, 0x01, 0x01, 0x00, 0x00, 0x00]
Result: AI1 mode = 1 (DOL12) ✓
```

**Step 2: Read Voltage via SDO**
```
SDO Upload: 0x6401:01
Request:  ID=0x610, [0x40, 0x01, 0x64, 0x01, 0x00, 0x00, 0x00, 0x00]
Response: ID=0x590, [0x4B, 0x01, 0x64, 0x01, 0xAA, 0x14, 0x00, 0x00]
Result: Voltage = 0x14AA = 5290 mV = 5.29V
```

**Step 3: Convert to Temperature**
```
Using DOL12 Lookup Table:
  5.29V is between:
    5.29V @ 15°C
    5.22V @ 16°C
    
  Linear interpolation:
    T = 15°C (closest match)
    
Result: Temperature ≈ 15°C
```

**Alternative: Read via PDO**
```
Receive TPDO 1: ID=0x1A0 + NodeID
Data: [0xAA, 0x14, ...]
      └─ AI1 = 0x14AA = 5290 mV → 15°C
```

### Example 2: Controlling Relays

**Scenario:** Turn ON relays 0, 4, 8 and turn OFF all others.

**Step 1: Calculate Bit Mask**
```
Desired State:
  Relay 0: ON  → Bit 0 = 1
  Relay 4: ON  → Bit 4 = 1
  Relay 8: ON  → Bit 8 = 1
  Others: OFF → All other bits = 0
  
Bit Mask: (1<<0) | (1<<4) | (1<<8) = 0x0111
Binary: 0b0000 0001 0001 0001
```

**Step 2: Send via SDO**
```
SDO Download: 0x6300:01 = 0x0111
Request:  ID=0x610, [0x2B, 0x00, 0x63, 0x01, 0x11, 0x01, 0x00, 0x00]
Response: ID=0x590, [0x60, 0x00, 0x63, 0x01, 0x00, 0x00, 0x00, 0x00]
Result: Relays updated successfully
```

**Alternative: Send via PDO**
```
RPDO 0: ID=0x210 + NodeID = 0x211 (for NodeID=1)
Data: [0x11, 0x01]
Result: Relays updated in real-time (<1ms)
```

### Example 3: Setting Analog Output

**Scenario:** Set AO2 to 7.5V (7500 mV).

**Step 1: Calculate Raw Value**
```
Desired Voltage: 7.5V = 7500 mV
Raw Value: 7500 (decimal) = 0x1D4C (hex)
```

**Step 2: Send via SDO**
```
SDO Download: 0x6411:02 = 7500
Request:  ID=0x610, [0x2B, 0x11, 0x64, 0x02, 0x4C, 0x1D, 0x00, 0x00]
Response: ID=0x590, [0x60, 0x11, 0x64, 0x02, 0x00, 0x00, 0x00, 0x00]
Result: AO2 set to 7.5V
```

**Alternative: Send via PDO**
```
RPDO 1: ID=0x310 + NodeID = 0x311 (for NodeID=1)
Data: [0x00, 0x00, 0x4C, 0x1D]
      └─ AO1    └─ AO2 = 7500 mV
Result: AO2 updated, AO1 unchanged (if 0x0000 interpreted as "no change")
```

**Note:** Some implementations require sending all mapped values, others support selective updates.

### Example 4: Monitoring Diagnostics

**Scenario:** Continuously monitor device health via TPDO 5.

**PDO Reception:**
```
CAN ID: 0x290 + NodeID = 0x2A0 (for NodeID=0x10)
Period: 15 seconds
Data: [0x36, 0x01, 0xCE, 0x3B, 0x83, 0x5E, 0x20, 0x4E]
```

**Decoding:**
```javascript
const data = Buffer.from([0x36, 0x01, 0xCE, 0x3B, 0x83, 0x5E, 0x20, 0x4E]);

const internalTemp = data.readUInt16LE(0) / 10;  // 310 / 10 = 31.0°C
const rail15V = data.readUInt16LE(2) / 1000;     // 15310 / 1000 = 15.310V
const rail24V = data.readUInt16LE(4) / 1000;     // 24195 / 1000 = 24.195V
const netPeriod = data.readUInt16LE(6);          // 20000 μs = 20ms → 50Hz

console.log(`Temperature: ${internalTemp}°C`);
console.log(`15V Rail: ${rail15V}V`);
console.log(`24V Rail: ${rail24V}V`);
console.log(`Mains Frequency: ${1000000/netPeriod}Hz`);
```

**Health Checks:**
```javascript
// Temperature alarm
if (internalTemp > 60) {
    console.warn('ALERT: High internal temperature!');
}

// Voltage alarms
if (rail15V < 14.0 || rail15V > 16.0) {
    console.warn('ALERT: 15V rail out of spec!');
}
if (rail24V < 22.0 || rail24V > 26.0) {
    console.warn('ALERT: 24V rail out of spec!');
}

// Mains frequency check
const mainsFreq = 1000000 / netPeriod;
if (mainsFreq < 48 || mainsFreq > 52) {
    console.warn('ALERT: Mains frequency abnormal!');
}
```

### Example 5: Event-Driven Digital Input Monitoring

**Scenario:** Monitor door switch on DI3, trigger alarm on state change.

**Step 1: Enable Interrupt Mask**
```
SDO Download: 0x6106:01 |= (1<<3)  // Enable DI3 change detection
Request:  ID=0x610, [0x2F, 0x06, 0x61, 0x01, 0x08, 0x00, 0x00, 0x00]
Response: ID=0x590, [0x60, 0x06, 0x61, 0x01, 0x00, 0x00, 0x00, 0x00]
```

**Step 2: Monitor TPDO 0**
```
CAN ID: 0x180 + NodeID = 0x181
Transmission: Event-driven (on any DI change)
Data: [0x08, 0x00]  // DI3 = HIGH (door open)
      └─ 0x0008 = 0b0000 0000 0000 1000
```

**Step 3: Detect State Change**
```javascript
let previousState = 0;

function onTPDO0(data) {
    const currentState = data.readUInt16LE(0);
    const di3 = (currentState >> 3) & 1;
    
    if (di3 !== ((previousState >> 3) & 1)) {
        if (di3 === 1) {
            console.log('ALARM: Door opened!');
            // Trigger alarm relay
            setRelayState(12, true); // Relay 12 = alarm
        } else {
            console.log('INFO: Door closed.');
            setRelayState(12, false);
        }
    }
    
    previousState = currentState;
}
```

### Example 6: TRIAC Phase Control

**Scenario:** Control heater power at 60% using TRIAC 1.

**Step 1: Calculate ON Time**
```
AC Cycle: 50Hz → Period = 20ms = 20000 μs
Half-cycle: 10ms = 10000 μs
Desired Power: 60%
ON Time: 10000 μs × 0.60 = 6000 μs
```

**Step 2: Set TRIAC 1 ON Time**
```
SDO Download: 0x2302:01 = 6000
Request:  ID=0x610, [0x2B, 0x02, 0x23, 0x01, 0x70, 0x17, 0x00, 0x00]
Response: ID=0x590, [0x60, 0x02, 0x23, 0x01, 0x00, 0x00, 0x00, 0x00]
Result: Heater now running at ~60% power
```

**Alternative: Control via PDO**
```
RPDO 3: ID=0x510 + NodeID = 0x511
Data: [0x70, 0x17, 0x00, 0x00]
      └─ TRIAC1  └─ TRIAC2
Result: TRIAC1 = 6000 μs, TRIAC2 = 0 (off)
```

**Power Curve (50Hz):**
```
ON Time (μs)  →  Approx Power
0             →  0%
2500          →  25%
5000          →  50%
7500          →  75%
10000         →  100%
```

---

## Appendices

### A. COB-ID Summary (NodeID = 0x10)

| Function | COB-ID | Direction | Data |
|----------|--------|-----------|------|
| NMT | 0x000 | Master → All | NMT commands |
| SYNC | 0x080 | Master → All | Synchronization |
| EMCY | 0x090 | Device → Master | Emergency |
| TPDO 0 | 0x190 | Device → Master | Digital inputs |
| TPDO 1 | 0x1B0 | Device → Master | AI 1-4 |
| TPDO 2 | 0x1D0 | Device → Master | AI 5-7 |
| TPDO 3 | 0x1F0 | Device → Master | AI 8-11 |
| TPDO 4 | 0x1A0 | Device → Master | DOL78 status |
| TPDO 5 | 0x2A0 | Device → Master | Diagnostics |
| TPDO 6 | 0x3A0 | Device → Master | Counters 1-4 |
| TPDO 7 | 0x4A0 | Device → Master | Counters 5-7 |
| TPDO 8 | 0x1B0 | Device → Master | Counters 8-11 |
| RPDO 0 | 0x220 | Master → Device | Relay control |
| RPDO 1 | 0x320 | Master → Device | AO 1-2 |
| RPDO 2 | 0x420 | Master → Device | AO 3-6 |
| RPDO 3 | 0x520 | Master → Device | TRIAC 1-2 |
| SDO RX | 0x610 | Master → Device | SDO requests |
| SDO TX | 0x590 | Device → Master | SDO responses |
| Heartbeat | 0x710 | Device → Master | NMT heartbeat |

### B. Common Error Codes

| Error Code | Name | Description | Typical Cause |
|------------|------|-------------|---------------|
| 0x05030000 | Toggle bit error | SDO toggle bit mismatch | Segmented transfer error |
| 0x05040000 | SDO timeout | Client/server timeout | Communication loss |
| 0x05040001 | Invalid command | Unknown SDO command | Protocol error |
| 0x06010000 | Unsupported access | Object not accessible | Wrong access mode |
| 0x06010001 | Write-only object | Attempted read of WO object | Protocol error |
| 0x06010002 | Read-only object | Attempted write of RO object | Configuration error |
| 0x06020000 | Object not exist | Object not in dictionary | Invalid index |
| 0x06040041 | Object too large | Data exceeds PDO size | Mapping error |
| 0x06040042 | Object map fail | PDO mapping invalid | Configuration error |
| 0x06070010 | Type mismatch | Data type incorrect | Protocol error |
| 0x06070012 | Length error | Data length mismatch | Protocol error |
| 0x06090011 | Subindex not exist | Subindex not in object | Invalid subindex |
| 0x06090030 | Value range error | Value out of limits | Data validation error |
| 0x06090031 | Value too high | Value above max | Data validation error |
| 0x06090032 | Value too low | Value below min | Data validation error |
| 0x08000000 | General error | Unspecified device error | Various |
| 0x08000020 | Data error | Invalid data detected | CRC/parity error |
| 0x08000021 | Store error | Flash write failed | Hardware error |
| 0x08000022 | Local error | Device-specific error | Calibration, etc. |

### C. Abbreviations

| Abbreviation | Full Term |
|--------------|-----------|
| AI | Analog Input |
| AO | Analog Output |
| CAN | Controller Area Network |
| CiA | CAN in Automation |
| COB-ID | Communication Object Identifier |
| DI | Digital Input |
| DO | Digital Output |
| DOL | SKOV Device OnLine (sensor series) |
| EDS | Electronic Data Sheet |
| EMCY | Emergency |
| LSB | Least Significant Byte |
| LSS | Layer Setting Services |
| MSB | Most Significant Byte |
| NMT | Network Management |
| NTC | Negative Temperature Coefficient (thermistor) |
| OD | Object Dictionary |
| PDO | Process Data Object |
| RPDO | Receive PDO |
| RW | Read/Write |
| RWW | Read/Write/Write-Window (immediate effect) |
| RO | Read Only |
| SDO | Service Data Object |
| TPDO | Transmit PDO |
| WO | Write Only |

### D. References

1. **CiA 301** - CANopen Application Layer and Communication Profile v4.2.0
2. **CiA 305** - Layer Setting Services and Protocol (LSS) v3.0.0
3. **CiA 306** - Electronic Data Sheet Specification for CANopen v4.1.0
4. **ISO 11898-1** - Controller Area Network (CAN) - Data Link Layer
5. **SKOV Main IO Hardware Manual** - Product-specific documentation
6. **SKOV DOL12 Sensor Datasheet** - Temperature sensor specifications

### E. Document Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2025-11-12 | Development Team | Initial release |

---

**End of Document**

