#ifndef _MODBUS_CONFIG_H
#define _MODBUS_CONFIG_H

#include <stddef.h>
#include "at32f403a_407.h"
#include "mbport.h"

#define DEFAULT_MODBUS_ADDRESS (1U)
#define DEFAULT_MODBUS_BAUDRATE (9600U)
#define DEFAULT_MODBUS_DATA_BITS (USART_DATA_8BITS)
#define DEFAULT_MODBUS_PARITY (USART_PARITY_NONE)
#define DEFAULT_MODBUS_STOP_BITS (USART_STOP_1_BIT)

/// @brief Modbus RTU configuration parameters
struct ModbusConfig
{
    uint8_t address;                           // Modbus slave address
    uint32_t baudrate;                         // UART baudrate
    usart_data_bit_num_type data_bits;         // Number of data bits
    usart_parity_selection_type parity;        // Parity type
    usart_stop_bit_num_type stop_bits;         // Number of stop bits

    /// @brief Set default Modbus configuration values
    void set_default(void);

    /// @brief Get total size of the structure for serialization
    /// @return Size in bytes
    static constexpr size_t get_size(void)
    {
        return sizeof(address) + sizeof(baudrate) + sizeof(data_bits) + 
               sizeof(parity) + sizeof(stop_bits);
    }

    /// @brief Serialize configuration to buffer
    /// @param cursor Pointer to buffer position
    void serialize(uint8_t *&cursor) const;

    /// @brief Deserialize configuration from buffer
    /// @param cursor Pointer to buffer position
    void deserialize(const uint8_t *&cursor);
    
    /// @brief Convert AT32 parity type to Modbus parity type
    /// @return Modbus parity type
    eMBParity get_modbus_parity(void) const;
    
    /// @brief Convert AT32 stop bits type to Modbus stop bits count
    /// @return Stop bits count (1 or 2)
    uint8_t get_modbus_stop_bits(void) const;
    
    /// @brief Convert AT32 data bits type to Modbus data bits count
    /// @return Data bits count (always 8 for Modbus)
    uint8_t get_modbus_data_bits(void) const;
};

#endif

