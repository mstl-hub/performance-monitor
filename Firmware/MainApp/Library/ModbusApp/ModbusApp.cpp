#include "ModbusApp.h"
#include "FreeRTOS.h"
#include "P-Block-struct.h"
#include "PBlockConfig.h"
#include "UniversalInputManager.h"
#include "at32f403a_407_usart.h"
#include "mbutils.h"
#include "task.h"
#include "UartDrv.h"  // For drv_uart_transmit in DEBUG mode

/**
 * @brief P-Block Modbus RTU Address Map
 * 
 * COILS (Read/Write, 00001+):
 *   1-13  : Relays (12 standard + 1 emergency)
 * 
 * DISCRETE INPUTS (Read-Only, 10001+):
 *   1-11  : Universal Inputs - Digital states (GPIO/comparators)
 * 
 * INPUT REGISTERS (Read-Only, 30001+):
 *   1-11  : Universal Inputs - Analog values from ADC (0-10000 mV)
 *   20-22 : Emergency Block (reserved)
 * 
 * HOLDING REGISTERS (Read/Write, 40001+):
 *   0-5    : Analog Outputs (6 channels, 0-10000 mV)
 *   99-109 : Input Configuration (11 channels, mode 0-3)
 *   119    : Hybrid Channel Config (0/2/4)
 *   200    : System Status (0=OK, else error code)
 *   201    : Boot Count (restart counter)
 *   210-219: Error Log (10 most recent errors)
 */

void modbusFun(void *parameters) {
  (void)parameters;

  eMBErrorCode eStatus = MB_ENOERR; // Initialize error status

  /* Get Modbus configuration from PBlockConfig */
  ModbusConfig &mb_config = PBlockConfig::GetModbusConfig();

  static bool first_run = true;
  
  for (;;) { // Main loop
    /* Initialize Modbus stack with configuration from PBlockConfig */
    eStatus = eMBInit(MB_RTU, mb_config.address, 1, mb_config.baudrate,
                      mb_config.get_modbus_parity(),
                      mb_config.get_modbus_stop_bits());
    if (eStatus != MB_ENOERR) {
      // Initialization failed
      vTaskDelay(pdMS_TO_TICKS(1000));
      continue;
    }
    
    /* Give UART time to stabilize after init */
    vTaskDelay(100);
    
#ifdef DEBUG
    /* Send test pattern on TX ONCE to verify UART works */
    if (first_run)
    {
        first_run = false;
        
        const uint8_t test_msg[] = {'*', '*', 'M', 'O', 'D', 'B', 'U', 'S', ' ',
                                     'R', 'E', 'A', 'D', 'Y', '*', '*', '\r', '\n'};
        const uint16_t msg_len = sizeof(test_msg);
        
        /* Use UART driver function for transmission */
        drv_uart_transmit(MB_USART, test_msg, msg_len);
        
        /* Wait for transmission to complete before Modbus takes control */
        vTaskDelay(pdMS_TO_TICKS(100));
    }
#endif

    /* Enable the Modbus Protocol Stack */
    eStatus = eMBEnable();
    if (eStatus != MB_ENOERR) {
      // Enable failed
      continue;
    }

    for (;;) {
      eMBPoll();
      vTaskDelay(10); // Small delay between polls
    }
  }
}

/* Modbus callback functions */
/* Holding Register Address Map:
 *   0-5    : Analog Outputs (6 registers)
 *   99-109 : Input Configuration (11 registers) 
 *   119    : Hybrid Config (1 register)
 *   200    : System Status (1 register)
 *   201    : Boot Count (1 register)
 *   210-219: Error Log (10 registers)
 */
eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
                             USHORT usNRegs, eMBRegisterMode eMode) {
  eMBErrorCode eStatus = MB_ENOERR;

  // Process each register in the request
  while (usNRegs > 0) {
    USHORT value = 0;

    // [0-5] Analog Outputs - Functions expect 1-based indexing (1-6)
    if (usAddress >= 0 && usAddress <= 5) {
      const uint8_t output_num = static_cast<uint8_t>(usAddress + 1); // Convert to 1-6
      if (eMode == MB_REG_READ) {
        value = PBlockRegisters_t::GetAnalogOutput(output_num);
        *pucRegBuffer++ = static_cast<UCHAR>(value >> 8);
        *pucRegBuffer++ = static_cast<UCHAR>(value & 0xFF);
      } else { // MB_REG_WRITE
        value = *pucRegBuffer++ << 8;
        value |= *pucRegBuffer++;
        PBlockRegisters_t::SetAnalogOutput(output_num, value);
      }
    }
    // [99-109] Input Configuration - Input modes for 11 universal inputs
    else if (usAddress >= 99 && usAddress <= 109) {
      const uint8_t input_num = static_cast<uint8_t>(usAddress - 99 + 1); // 1-11
      if (eMode == MB_REG_READ) {
        value = static_cast<USHORT>(PBlockRegisters_t::GetInputMode(input_num));
        *pucRegBuffer++ = static_cast<UCHAR>(value >> 8);
        *pucRegBuffer++ = static_cast<UCHAR>(value & 0xFF);
      } else { // MB_REG_WRITE
        value = *pucRegBuffer++ << 8;
        value |= *pucRegBuffer++;
        // Use UniversalInputManager to handle mode changes and hardware reconfiguration
        if (!UniversalInputManager::ConfigureInputMode(input_num, static_cast<UniversalInputType>(value))) {
          // Mode change failed - could be due to hardware not initialized
          // For now, still update the register but hardware won't be reconfigured
          // PBlockRegisters_t::SetInputMode(input_num, static_cast<UniversalInputType>(value));
        }
      }
    }
    // [119] Hybrid Config
    else if (usAddress == 119) {
      if (eMode == MB_REG_READ) {
        value = static_cast<USHORT>(PBlockRegisters_t::holding_registers.hybrid_config);
        *pucRegBuffer++ = static_cast<UCHAR>(value >> 8);
        *pucRegBuffer++ = static_cast<UCHAR>(value & 0xFF);
      } else { // MB_REG_WRITE
        value = *pucRegBuffer++ << 8;
        value |= *pucRegBuffer++;
        PBlockRegisters_t::holding_registers.hybrid_config = static_cast<HybridConfigType>(value);
      }
    }
    // [200] System Status
    else if (usAddress == 200) {
      if (eMode == MB_REG_READ) {
        value = PBlockRegisters_t::holding_registers.status;
        *pucRegBuffer++ = static_cast<UCHAR>(value >> 8);
        *pucRegBuffer++ = static_cast<UCHAR>(value & 0xFF);
      } else { // MB_REG_WRITE
        value = *pucRegBuffer++ << 8;
        value |= *pucRegBuffer++;
        PBlockRegisters_t::holding_registers.status = value;
      }
    }
    // [201] Boot Count
    else if (usAddress == 201) {
      if (eMode == MB_REG_READ) {
        value = PBlockRegisters_t::holding_registers.boot_count;
        *pucRegBuffer++ = static_cast<UCHAR>(value >> 8);
        *pucRegBuffer++ = static_cast<UCHAR>(value & 0xFF);
      } else { // MB_REG_WRITE - typically read-only, but allow reset
        value = *pucRegBuffer++ << 8;
        value |= *pucRegBuffer++;
        PBlockRegisters_t::holding_registers.boot_count = value;
      }
    }
    // [210-219] Error Log
    else if (usAddress >= 210 && usAddress <= 219) {
      const uint8_t error_index = static_cast<uint8_t>(usAddress - 210);
      if (eMode == MB_REG_READ) {
        value = PBlockRegisters_t::holding_registers.error_log[error_index];
        *pucRegBuffer++ = static_cast<UCHAR>(value >> 8);
        *pucRegBuffer++ = static_cast<UCHAR>(value & 0xFF);
      } else { // MB_REG_WRITE - typically read-only, but allow clearing
        value = *pucRegBuffer++ << 8;
        value |= *pucRegBuffer++;
        PBlockRegisters_t::holding_registers.error_log[error_index] = value;
      }
    }
    // Invalid address
    else {
      return MB_ENOREG;
    }

    usAddress++;
    usNRegs--;
  }

  return eStatus;
}

/* Input Registers (Read-Only, Analog/Digital Values from ADC/Sensors)
 * Address Map (30001+ in Modbus notation):
 *   1-11 : Universal Inputs - Analog values (0-10000 mV from ADC)
 *   20   : Emergency Battery Voltage (reserved)
 *   21   : Emergency Room State 1 (reserved)
 *   22   : Emergency Room State 2 (reserved)
 */
eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress,
                           USHORT usNRegs) {

  while (usNRegs > 0) {
    USHORT value = 0;

    // [1-11] Universal Inputs - ADC values
    if (usAddress >= 1 && usAddress <= 11) {
      value =
          PBlockRegisters_t::GetUniversalInput(static_cast<uint8_t>(usAddress));
    }
    // [20] Emergency Battery Voltage
    else if (usAddress == 20) {
      value = 0; // reserved for future emergency block
    }
    // [21] Emergency Room State 1
    else if (usAddress == 21) {
      value = 0; // reserved
    }
    // [22] Emergency Room State 2
    else if (usAddress == 22) {
      value = 0; // reserved
    } else {
      return MB_ENOREG;
    }

    // Write big-endian
    *pucRegBuffer++ = static_cast<UCHAR>(value >> 8);
    *pucRegBuffer++ = static_cast<UCHAR>(value & 0xFF);

    usAddress++;
    usNRegs--;
  }

  return MB_ENOERR;
}

/* Discrete Inputs (Read-Only, Digital States)
 * Address Map (10001+ in Modbus notation):
 *   1-11 : Universal Inputs - Digital states (HIGH/LOW from GPIO/comparators)
 */
eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress,
                              USHORT usNDiscrete) {
  // Pack LSB-first per Modbus spec.
  UCHAR currentByte = 0;
  UCHAR bitInByte = 0;

  while (usNDiscrete > 0) {
    // [1-11] Universal Inputs - Digital states
    if (usAddress >= 1 && usAddress <= 11) {
      const uint8_t inputNum = static_cast<uint8_t>(usAddress);
      const bool state = PBlockRegisters_t::GetUniversalInputDiscrete(inputNum);
      if (state) {
        currentByte |= (1u << bitInByte);
      }
    } else {
      // Outside supported range
      return MB_ENOREG;
    }

    bitInByte++;
    if (bitInByte == 8) {
      *pucRegBuffer++ = currentByte;
      currentByte = 0;
      bitInByte = 0;
    }

    usAddress++;
    usNDiscrete--;
  }

  // Flush remaining bits if not a multiple of 8
  if (bitInByte != 0) {
    *pucRegBuffer++ = currentByte;
  }

  return MB_ENOERR;
}

/* Coils (Read/Write, Relay Outputs)
 * Address Map (00001+ in Modbus notation):
 *   1-12 : Relays 1-12 - Standard relay outputs
 *   13   : Emergency Relay - Special emergency relay
 */
eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress,
                           USHORT usNCoils, eMBRegisterMode eMode) {
  UCHAR currentByte = 0;
  UCHAR bitInByte = 0;

  if (eMode == MB_REG_READ) {
    // Read relay states and pack into buffer
    for (USHORT i = 0; i < usNCoils; i++) {
      const USHORT coilAddr = usAddress + i;
      // [1-13] Relays (1-12 standard, 13 emergency)
      if (coilAddr >= 1 && coilAddr <= 13) {
        const uint8_t relayNum = static_cast<uint8_t>(coilAddr);
        const bool state = PBlockRegisters_t::GetRelay(relayNum);
        if (state) {
          currentByte |= (1u << bitInByte);
        }
      } else {
        return MB_ENOREG;
      }

      bitInByte++;
      if (bitInByte == 8) {
        *pucRegBuffer++ = currentByte;
        currentByte = 0;
        bitInByte = 0;
      }
    }

    // Flush remaining bits
    if (bitInByte != 0) {
      *pucRegBuffer++ = currentByte;
    }
  } else // MB_REG_WRITE
  {
    // Unpack buffer and write relay states
    for (USHORT i = 0; i < usNCoils; i++) {
      const USHORT coilAddr = usAddress + i;
      // [1-13] Relays (1-12 standard, 13 emergency)
      if (coilAddr >= 1 && coilAddr <= 13) {
        // Extract bit from buffer
        if (bitInByte == 0) {
          currentByte = *pucRegBuffer++;
        }

        const bool state = (currentByte & (1u << bitInByte)) != 0;
        const uint8_t relayNum = static_cast<uint8_t>(coilAddr);
        PBlockRegisters_t::SetRelay(relayNum, state);

        bitInByte++;
        if (bitInByte == 8) {
          bitInByte = 0;
        }
      } else {
        return MB_ENOREG;
      }
    }
  }

  return MB_ENOERR;
}
