#include "ModbusConfig.h"
#include "Shared.h"

void ModbusConfig::set_default(void)
{
    address = DEFAULT_MODBUS_ADDRESS;
    baudrate = DEFAULT_MODBUS_BAUDRATE;
    data_bits = DEFAULT_MODBUS_DATA_BITS;
    parity = DEFAULT_MODBUS_PARITY;
    stop_bits = DEFAULT_MODBUS_STOP_BITS;
}

void ModbusConfig::serialize(uint8_t *&cursor) const
{
    WriteToBuffer(cursor, address);
    WriteToBuffer(cursor, baudrate);
    WriteToBuffer(cursor, data_bits);
    WriteToBuffer(cursor, parity);
    WriteToBuffer(cursor, stop_bits);
}

void ModbusConfig::deserialize(const uint8_t *&cursor)
{
    ReadFromBuffer(cursor, address);
    ReadFromBuffer(cursor, baudrate);
    ReadFromBuffer(cursor, data_bits);
    ReadFromBuffer(cursor, parity);
    ReadFromBuffer(cursor, stop_bits);
}

eMBParity ModbusConfig::get_modbus_parity(void) const
{
    switch (parity)
    {
    case USART_PARITY_NONE:
        return MB_PAR_NONE;
    case USART_PARITY_EVEN:
        return MB_PAR_EVEN;
    case USART_PARITY_ODD:
        return MB_PAR_ODD;
    default:
        return MB_PAR_NONE;
    }
}

uint8_t ModbusConfig::get_modbus_stop_bits(void) const
{
    switch (stop_bits)
    {
    case USART_STOP_1_BIT:
        return 1;
    case USART_STOP_2_BIT:
        return 2;
    case USART_STOP_1_5_BIT:
        return 2; // Modbus doesn't support 1.5, round up to 2
    default:
        return 1;
    }
}

uint8_t ModbusConfig::get_modbus_data_bits(void) const
{
    switch (data_bits)
    {
    case USART_DATA_8BITS:
        return 8;
    case USART_DATA_9BITS:
        return 8; // Modbus doesn't support 9, use 8
    default:
        return 8;
    }
}

