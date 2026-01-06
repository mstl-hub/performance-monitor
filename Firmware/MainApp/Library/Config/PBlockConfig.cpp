#include "PBlockConfig.h"
#include "Shared.h"
#include "string.h"

uint8_t PBlockConfig::config_version;
uint32_t PBlockConfig::SerialNum;
calendar_type PBlockConfig::AssemblyDate;
ModbusConfig PBlockConfig::modbus_config;

uint8_t PBlockConfig::crc;
bool PBlockConfig::isOkay;

void PBlockConfig::Init(void)
{
    LoadConfig();
    isOkay = CheckCRC();
    if (!isOkay)
    {
        SetDefault();
        SaveConfig();
        isOkay = true;
    }
}

void PBlockConfig::LoadConfig(void)
{
    size_t size = GetSize();
    uint8_t buff[size];
    FlashService::Read(ADDRESS_DEVICE_CONGIG, buff, size);
    Deserialize(buff, size);
}

void PBlockConfig::SaveConfig(void)
{
    size_t size = GetSize();
    uint8_t buff[size];
    Serialize(buff, size);
    FlashService::CheckDiffAndReprogramm(ADDRESS_DEVICE_CONGIG, buff, size);
}

bool PBlockConfig::CheckCRC(void)
{
    return CountCRC() == crc;
}

/// @brief Set default values to DEVICE config
/// @param
void PBlockConfig::SetDefault(void)
{
    config_version = VERSION_DEVICE_CONFIG;
    SerialNum = DEFAULT_DEVICE_CONFIG_SERIAL_NUM;
    AssemblyDate = DEFAULT_DEVICE_CONFIG_ASSEMBLY_DATE;
    modbus_config.set_default();
    SetCRC();
}

uint32_t PBlockConfig::GetSerialNum(void)
{
    return isOkay ? SerialNum : DEFAULT_DEVICE_CONFIG_SERIAL_NUM;
}

void PBlockConfig::SetSerialNum(uint32_t new_SerialNum)
{
    if (SerialNum != new_SerialNum)
    {
        SerialNum = new_SerialNum;
        SetCRC();
        SaveConfig();
    }
}

calendar_type PBlockConfig::GetAssemblyDate(void)
{
    return isOkay ? AssemblyDate : DEFAULT_DEVICE_CONFIG_ASSEMBLY_DATE;
}

void PBlockConfig::SetAssemblyDate(calendar_type new_AssemblyDate)
{
    if (memcmp(&AssemblyDate, &new_AssemblyDate, sizeof(calendar_type)) != 0)
    {
        AssemblyDate = new_AssemblyDate;
        SetCRC();
        SaveConfig();
    }
}

ModbusConfig& PBlockConfig::GetModbusConfig(void)
{
    return modbus_config;
}

void PBlockConfig::SetModbusConfig(const ModbusConfig &new_modbus_config)
{
    if (memcmp(&modbus_config, &new_modbus_config, sizeof(ModbusConfig)) != 0)
    {
        modbus_config = new_modbus_config;
        SetCRC();
        SaveConfig();
    }
}

/// @brief count current crc value and set it to PBlockConfig
/// @param
void PBlockConfig::SetCRC(void)
{
    crc = CountCRC();
}

/// @brief count crc
/// @param
/// @return crc as uint8_t
uint8_t PBlockConfig::CountCRC(void)
{
    uint16_t size = GetSize() - sizeof(crc);
    uint8_t buff[size];
    uint8_t *cursor = buff;

    WriteToBuffer(cursor, config_version);
    WriteToBuffer(cursor, SerialNum);
    WriteToBuffer(cursor, AssemblyDate);
    
    modbus_config.serialize(cursor);

    return count_CRC(buff, size);
}

// Serialize function
void PBlockConfig::Serialize(uint8_t *buffer, size_t &size)
{
    size = GetSize();
    uint8_t *cursor = buffer;
    
    WriteToBuffer(cursor, config_version);
    WriteToBuffer(cursor, SerialNum);
    WriteToBuffer(cursor, AssemblyDate);
    
    modbus_config.serialize(cursor);

    WriteToBuffer(cursor, crc);
}

// Deserialize function
void PBlockConfig::Deserialize(const uint8_t *buffer, size_t size)
{
    size = GetSize();
    
    ReadFromBuffer(buffer, config_version);
    ReadFromBuffer(buffer, SerialNum);
    ReadFromBuffer(buffer, AssemblyDate);
    
    modbus_config.deserialize(buffer);
    
    ReadFromBuffer(buffer, crc);
}

size_t PBlockConfig::GetSize(void)
{
    return sizeof(config_version) + sizeof(SerialNum) + sizeof(AssemblyDate) + 
           ModbusConfig::get_size() + sizeof(crc);
}

// PBlockConfig* _PBlockConfig;
