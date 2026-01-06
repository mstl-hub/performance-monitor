#ifndef _DEVICE_CONFIG_H
#define _DEVICE_CONFIG_H

#include "at32f403a_407.h"
#include "FlashService.h"
#include "CRC.h"
#include "ModbusConfig.h"

#define VERSION_DEVICE_CONFIG (1U)
#define SEC_DEVICE_CONGIG (253U)
#define ADDRESS_DEVICE_CONGIG (SECTOR_ADDRESS(SEC_DEVICE_CONGIG))

/*Singletone*/
struct PBlockConfig
{
private:
    static uint8_t config_version;
    static uint32_t SerialNum;
    static calendar_type AssemblyDate;
    static ModbusConfig modbus_config;
    
    static uint8_t crc;

    static bool CheckCRC(void);
    static void SetDefault(void);
    static void SetCRC(void);
    static uint8_t CountCRC(void);
    static void Serialize(uint8_t *buffer, size_t &size);
    static void Deserialize(const uint8_t *buffer, size_t size);
    static size_t GetSize(void);

    static void LoadConfig(void);
    static void SaveConfig(void);

    static bool isOkay; // doesn't save to flash

    PBlockConfig(/* args */);

public:
    // Deleting the copy constructor to prevent copies
    PBlockConfig(const PBlockConfig &obj) = delete;

    ~PBlockConfig() {}

    static void Init(void);

    static uint8_t GetConfigVersion(void);
    static uint32_t GetSerialNum(void);
    static void SetSerialNum(uint32_t new_SerialNum);
    static calendar_type GetAssemblyDate(void);
    static void SetAssemblyDate(calendar_type new_AssemblyDate);
    
    static ModbusConfig& GetModbusConfig(void);
    static void SetModbusConfig(const ModbusConfig &new_modbus_config);

    static uint8_t GetCRC(void);
};
// -----------------------
// FlashMap
// -----------------------
// config_version    1b
// SerialNum         4b
// AssemblyDate      4b
// modbus_config     variable size (see ModbusConfig::get_size())
// crc               1b  crc must be last
// -----------------------

#define DEFAULT_DEVICE_CONFIG_TX_DELAY_S (10U)
#define DEFAULT_DEVICE_CONFIG_DEV_ADDR (1U)
#define DEFAULT_DEVICE_CONFIG_DEV_EUI (0U)
const uint8_t DEFAULT_DEVICE_CONFIG_NETWORK_KEY[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const uint8_t DEFAULT_DEVICE_CONFIG_APP_KEY[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#define DEFAULT_DEVICE_CONFIG_SERIAL_NUM (0U)
const calendar_type DEFAULT_DEVICE_CONFIG_ASSEMBLY_DATE{0, 0, 0, 0, 0, 0, 0};
#define DEFAULT_DEVICE_CONFIG_JOIN_EUI (0U)
#define DEFAULT_DEVICE_CONFIG_FREQUENCY (868900000U)

#endif
