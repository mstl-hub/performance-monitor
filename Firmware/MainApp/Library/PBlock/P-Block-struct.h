#ifndef __P_BLOCK_STRUCT_H__
#define __P_BLOCK_STRUCT_H__

#include <at32f403a_407.h>
#include <stdint.h>  

// Universal input type enumeration
enum class UniversalInputType : uint8_t {
  ANALOG = 0,     // Analog input (0-10000 mV)
  DIGITAL = 1,    // Digital input (0/1)
  DOL12_TEMP = 2, // Dol12 temperature sensor
  TAFCO_TEMP = 3  // Tafco temperature sensor
};

enum class HybridConfigType : uint8_t {
  FOUR_OUTPUTS = 0,
  TWO_INPUTS = 2,
  FOUR_EXTRA_INPUTS = 4
};

// Universal Input - Individual input with multiple value types
struct UniversalInput_t {
  UniversalInputType input_type; // Input type enumeration
  uint16_t analog_value;         // Analog value: 0-10000 mV
  uint16_t discrete_value;       // Discrete/digital value: true/false
};

// Coils (Read/Write bits) - Relay Outputs
struct Coils_t {
  // Discrete Outputs (00001-00013, addresses 0-12)
  // Bit field structure for individual relay control
  struct RelayBits_t {
    bool relay1 : 1;          // Relay 1 (address 0)
    bool relay2 : 1;          // Relay 2 (address 1)
    bool relay3 : 1;          // Relay 3 (address 2)
    bool relay4 : 1;          // Relay 4 (address 3)
    bool relay5 : 1;          // Relay 5 (address 4)
    bool relay6 : 1;          // Relay 6 (address 5)
    bool relay7 : 1;          // Relay 7 (address 6)
    bool relay8 : 1;          // Relay 8 (address 7)
    bool relay9 : 1;          // Relay 9 (address 8)
    bool relay10 : 1;         // Relay 10 (address 9)
    bool relay11 : 1;         // Relay 11 (address 10)
    bool relay12 : 1;         // Relay 12 (address 11)
    bool emergency_relay : 1; // Emergency relay (address 12)
    bool : 3;                 // Padding bits (unused)
  } relay_bits;
};

/**
 * @brief Structure containing all P-Block register data for Modbus
 * communication Based on Modbus RTU specification from modbus-rtu.md
 */
#pragma pack(push, 1)
struct PBlockRegisters_t {
  // Holding Registers (Read/Write) - Analog Outputs and Configuration
  struct HoldingRegisters_t {

    // Hybrid Channel Configuration (40120, address 119)
    HybridConfigType hybrid_config; // 0=4 outputs, 2=2 inputs (B terminals),
                                    // 4=4 extra inputs

    // Diagnostic Registers (40201-40219, addresses 200-218)
    uint16_t status;        // STATUS register (0=normal, 1-65535=error code)
    uint16_t boot_count;    // BOOT_CNT register (restart counter)
    uint16_t error_log[10]; // ERROR_LOG (10 latest errors, ERR1-ERR10)
  };

  // Static data members (declared inside struct, defined outside)

  static Coils_t coils; // Coils (Read/Write bits) - Relay Outputs
  static UniversalInput_t universal_inputs[11];
  // Emergency Block Data (30021-30023, addresses 20-22) - TODO: implement
  // after development
  uint16_t emergency_battery_voltage; // Battery voltage of emergency block
  uint16_t emergency_room_state_1;    // State 1 of room on emergency block
  uint16_t emergency_room_state_2;    // State 2 of room on emergency block
  // Analog Outputs (40001-40006, addresses 0-5)
  // Terminal mapping: [0]=B3, [1]=B16, [2]=B2-A, [3]=B2-B, [4]=B15-A,
  // [5]=B15-B
  static uint16_t analog_output[6]; // Analog outputs - 0-10000 mV

  static HoldingRegisters_t holding_registers;

  // Static member functions
  static void Init(void);
  static void UpdateInputs(void);
  static void SetRelay(uint8_t relay_number, bool state);
  static bool GetRelay(uint8_t relay_number);
  static void SetAnalogOutput(uint8_t output_number, uint16_t value);
  static uint16_t GetAnalogOutput(uint8_t output_number);
  static void SetInputMode(uint8_t input_number, UniversalInputType mode);
  static UniversalInputType GetInputMode(uint8_t input_number);
  static uint16_t GetUniversalInput(uint8_t input_number);
  static uint16_t GetUniversalInputType(uint8_t input_number);
  static bool GetUniversalInputDiscrete(uint8_t input_number);
  static void LogError(uint16_t error_code);
  static void ClearErrorStatus(void);
  static void IncrementBootCount(void);
};
#pragma pack(pop)

#endif // __P_BLOCK_STRUCT_H__
