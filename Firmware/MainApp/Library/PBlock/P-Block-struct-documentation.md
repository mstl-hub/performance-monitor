# P-Block Register Structure Documentation  

## Overview  

The `PBlockRegisters_t` structure provides a type-safe API and storage for Modbus RTU registers of the P‑Block device, aligned with the current header `P-Block-struct.h`.  

The P‑Block includes:  
- **12 discrete relay outputs** (including 1 emergency relay)  
- **11 universal inputs** (configurable as analog, digital, or temperature sensors)  
- **6 analog outputs**  
- **Diagnostic and status registers**  

## Structure Organization  

```cpp
struct PBlockRegisters_t  
{  
    // Static data  
    static HoldingRegisters_t holding_registers;  
    static Coils_t coils;  
    static UniversalInput_t universal_inputs[11];  
    static uint16_t analog_output[6];  

    // Reserved input registers for emergency block (non-static)  
    uint16_t emergency_battery_voltage;  
    uint16_t emergency_room_state_1;  
    uint16_t emergency_room_state_2;  

    // Static member functions (API)  
    static void Init(void);  
    static void UpdateInputs(void);  
    static void SetRelay(uint8_t relay_number, bool state);  
    static bool GetRelay(uint8_t relay_number);  
    static void SetAnalogOutput(uint8_t output_number, uint16_t value);  
    static uint16_t GetAnalogOutput(uint8_t output_number);  
    static void SetInputMode(uint8_t input_number, uint16_t mode);  
    static uint16_t GetInputMode(uint8_t input_number);  
    static uint16_t GetUniversalInput(uint8_t input_number);  
    static uint16_t GetUniversalInputType(uint8_t input_number);  
    static bool GetUniversalInputDiscrete(uint8_t input_number);  
    static void LogError(uint16_t error_code);  
    static void ClearErrorStatus(void);  
    static void IncrementBootCount(void);  
};  
```

## Holding Registers (Read/Write)  

Holding registers store configuration and diagnostics. Analog outputs are provided via a dedicated static array `analog_output[6]`.  

### Analog Outputs (Addresses 0–5)  
```cpp
static uint16_t analog_output[6];  // 0–10000 mV  
```

| Array Index | Terminal | Modbus Address | Description |  
|-------------|----------|----------------|-------------|  
| 0 | B3 | 40001 | Analog output 1 |  
| 1 | B16 | 40002 | Analog output 2 |  
| 2 | B2-A | 40003 | Analog output 3 |  
| 3 | B2-B | 40004 | Analog output 4 |  
| 4 | B15-A | 40005 | Analog output 5 |  
| 5 | B15-B | 40006 | Analog output 6 |  

**Value Range:** 0–10000 mV  
**Data Type:** `uint16_t`  

### Hybrid Channel Configuration (Address 119)  
```cpp
HybridConfigType hybrid_config; // 0=4 outputs, 2=2 inputs (B), 4=4 extra inputs  
```

### Diagnostic Registers  

```cpp
uint16_t status;        // 0=OK, 1–65535=error code  
uint16_t boot_count;    // increments on each boot  
uint16_t error_log[10]; // ERR1..ERR10 (index 0 is latest)  
```  

## Universal Inputs  

Universal inputs provide configurable analog/digital/temperature sensor data.  

### Universal Input Types and Structure  
```cpp
enum class UniversalInputType : uint8_t  
{  
    ANALOG = 0,     // 0–10000 mV  
    DIGITAL = 1,    // 0/1 (counter use up to 65535)  
    DOL12_TEMP = 2, // Dol12 temperature sensor  
    TAFCO_TEMP = 3  // Tafco temperature sensor  
};  

struct UniversalInput_t  
{  
    UniversalInputType input_type;  
    uint16_t analog_value;         // 0–10000 mV  
    uint16_t discrete_value;       // digital/counter value  
};  
```

### Universal Inputs Array (Addresses 0–10)  
```cpp
static UniversalInput_t universal_inputs[11];  
```

| Array Index | Terminal | Modbus Address | Description |  
|-------------|----------|----------------|-------------|  
| 0 | B5 | 30001 | Universal input 1 |  
| 1 | B7 | 30002 | Universal input 2 |  
| 2 | B18 | 30003 | Universal input 3 |  
| 3 | B9 | 30004 | Universal input 4 |  
| 4 | B20 | 30005 | Universal input 5 |  
| 5 | B2 | 30006 | Universal input 6 |  
| 6 | B15 | 30007 | Universal input 7 |  
| 7 | B2-A | 30008 | Universal input 8 |  
| 8 | B2-B | 30009 | Universal input 9 |  
| 9 | B15-A | 30010 | Universal input 10 |  
| 10 | B15-B | 30011 | Universal input 11 |  

**Value ranges:**  
- Analog: 0–10000 mV  
- Digital/Counter: 0–65535  
- Temperature modes: 0–10000 mV (sensor voltage)  

## Input Registers (reserved)  

Emergency block status fields are reserved and part of `PBlockRegisters_t` as non-static members.  

### Emergency Block Data (Addresses 20–22)  
```cpp
uint16_t emergency_battery_voltage; // Battery voltage (mV)  
uint16_t emergency_room_state_1;    // Room 1 state  
uint16_t emergency_room_state_2;    // Room 2 state  
```  

Note: Actual acquisition will be implemented after emergency block hardware is finalized.  

## Coils (Read/Write Bits)  

Coils control the discrete relay outputs.  

### Relay Outputs (Addresses 0–12)  
```cpp
struct Coils_t  
{  
    struct RelayBits_t  
    {  
        bool relay1 : 1;          // address 0  
        bool relay2 : 1;          // address 1  
        bool relay3 : 1;          // address 2  
        bool relay4 : 1;          // address 3  
        bool relay5 : 1;          // address 4  
        bool relay6 : 1;          // address 5  
        bool relay7 : 1;          // address 6  
        bool relay8 : 1;          // address 7  
        bool relay9 : 1;          // address 8  
        bool relay10 : 1;         // address 9  
        bool relay11 : 1;         // address 10  
        bool relay12 : 1;         // address 11  
        bool emergency_relay : 1; // address 12  
        bool : 3;                 // padding  
    } relay_bits;  
};  
```

| Bit Field | Modbus Address | Description | Terminal |  
|-----------|----------------|-------------|----------|  
| relay1 | 00001 | Relay 1 | B30-32 |  
| relay2 | 00002 | Relay 2 | B33-35 |  
| relay3 | 00003 | Relay 3 | B36-38 |  
| relay4 | 00004 | Relay 4 | B39-41 |  
| relay5 | 00005 | Relay 5 | B42-44 |  
| relay6 | 00006 | Relay 6 | B45-47 |  
| relay7 | 00007 | Relay 7 | B50-52 |  
| relay8 | 00008 | Relay 8 | B53-55 |  
| relay9 | 00009 | Relay 9 | B56-58 |  
| relay10 | 00010 | Relay 10 | B59-61 |  
| relay11 | 00011 | Relay 11 | B62-64 |  
| relay12 | 00012 | Relay 12 | B65-67 |  
| emergency_relay | 00013 | Emergency Relay | B12-B13AB |  

**Values:**
- `0xFF00` (ON) - Relay activated
- `0x0000` (OFF) - Relay deactivated

## Discrete Inputs (mapping)  

When a universal input is configured as DIGITAL, its state can be read via Modbus discrete inputs (addresses 0–10). The project does not define a `DiscreteInputs_t` structure; the mapping is logical.  

| Universal Input | Discrete Address | Terminal |  
|-----------------|------------------|----------|  
| 1 | 00001 | B5 |  
| 2 | 00002 | B7 |  
| 3 | 00003 | B18 |  
| 4 | 00004 | B9 |  
| 5 | 00005 | B20 |  
| 6 | 00006 | B2 |  
| 7 | 00007 | B15 |  
| 8 | 00008 | B2-A |  
| 9 | 00009 | B2-B |  
| 10 | 00010 | B15-A |  
| 11 | 00011 | B15-B |  

## API Functions  

### Static Member Functions  
```cpp
// Initialization and updates  
void PBlockRegisters_t::Init(void);  
void PBlockRegisters_t::UpdateInputs(void);  

// Analog output control  
void PBlockRegisters_t::SetAnalogOutput(uint8_t output_number, uint16_t value);  
uint16_t PBlockRegisters_t::GetAnalogOutput(uint8_t output_number);  

// Relay control  
void PBlockRegisters_t::SetRelay(uint8_t relay_number, bool state);  
bool PBlockRegisters_t::GetRelay(uint8_t relay_number);  

// Input configuration  
void PBlockRegisters_t::SetInputMode(uint8_t input_number, uint16_t mode);  
uint16_t PBlockRegisters_t::GetInputMode(uint8_t input_number);  
uint16_t PBlockRegisters_t::GetUniversalInput(uint8_t input_number);  
uint16_t PBlockRegisters_t::GetUniversalInputType(uint8_t input_number);  
bool PBlockRegisters_t::GetUniversalInputDiscrete(uint8_t input_number);  

// Error management  
void PBlockRegisters_t::LogError(uint16_t error_code);  
void PBlockRegisters_t::ClearErrorStatus(void);  
void PBlockRegisters_t::IncrementBootCount(void);  
```

**Parameters:**  
- `output_number`: 1–6 (corresponds to array indices 0–5)  
- `value`: 0–10000 mV  
- `relay_number`: 1–13 (13 = emergency relay)  
- `state`: true = ON (0xFF00), false = OFF (0x0000)  
- `input_number`: 1–11 (corresponds to array indices 0–10)  
- `mode`: 0=analog, 1=digital, 2=Dol12 temp, 3=Tafco temp  

## Usage Examples  

### Initialize and Configure Device  
```cpp
PBlockRegisters_t::Init();  

// Configure first 5 inputs as analog, last 6 as digital  
for (uint8_t i = 1; i <= 5; i++) {  
    PBlockRegisters_t::SetInputMode(i, 0);  
}  
for (uint8_t i = 6; i <= 11; i++) {  
    PBlockRegisters_t::SetInputMode(i, 1);  
}  

// Set analog outputs  
PBlockRegisters_t::SetAnalogOutput(1, 5000);  
PBlockRegisters_t::SetAnalogOutput(2, 2500);  
```

### Control Relays  
```cpp
for (uint8_t i = 1; i <= 3; i++) {  
    PBlockRegisters_t::SetRelay(i, true);  
}  
bool emergency_active = PBlockRegisters_t::GetRelay(13);  
```

### Read Sensor Data  
```cpp
PBlockRegisters_t::UpdateInputs();  
uint16_t input1_voltage = PBlockRegisters_t::GetUniversalInput(1);  
uint16_t input2_voltage = PBlockRegisters_t::GetUniversalInput(2);  
uint16_t output1_setting = PBlockRegisters_t::GetAnalogOutput(1);  
```

### Error Handling  
```cpp
if (PBlockRegisters_t::holding_registers.status != 0) {  
    PBlockRegisters_t::LogError(PBlockRegisters_t::holding_registers.status);  
    PBlockRegisters_t::ClearErrorStatus();  
}  
```

## Error Codes (example)  

| Code | Description |  
|------|-------------|  
| 0 | Normal operation |  
| 1 | Processor overheating |  
| 2 | Analog input failure |  
| 3 | Analog output failure |  
| 4 | Input configuration mismatch |  
| 5 | Relay failure |  
| 6 | Memory failure |  
| 7 | Power supply error |  

## Memory Layout  

The structure is packed with `#pragma pack(push, 1)` as defined in the header to ensure Modbus register alignment. Static members provide global register storage.  

## Integration Notes  

- Ensure thread-safety if accessed from multiple RTOS tasks.  
- Analog values: mV in range 0–10000.  
- DIGITAL inputs may use full 16-bit range for counters.  
- `error_log` keeps last 10 errors, newest at index 0.  
- `boot_count` increments on each boot.  

## File References  

- **Header:** `P-Block-struct.h`  
- **Implementation:** `P-Block-struct.cpp`  
- **Protocol Spec:** `modbus-rtu.md`  

This documentation reflects the current `P-Block-struct.h` and implementation.  
