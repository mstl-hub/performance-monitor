#include "Command.h"

CMD_Map_t cmdMap[] = {
    // bootloader
    {REQUEST_COM_PREFIX, 0x01, 3, ResetRestBootloaderCnt_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x10, 3, WhatAppIsIt_cmd, MAP_FLGS(0x0, 0x0)},
    // versions
    {REQUEST_COM_PREFIX, 0x20, 3, GetHardwareVersion_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x21, 3, GetDeviceId_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x22, 3, GetBootloaderVersion_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x23, 7, SetHardwareVersion_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x24, 3, GetProtocolVersion_cmd, MAP_FLGS(0x0, 0x0)},
    // time
    {REQUEST_COM_PREFIX, 0x30, 3, GetCurrentTimeMs_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x31, 3, GetCurrentTimeS_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x32, 3, GetRtcTime_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x33, 11, SetRtcTime_cmd, MAP_FLGS(0x0, 0x0)},
    // updating
    {REQUEST_COM_PREFIX, 0x40, 3, StartUpdating_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x41, 3, EraseMainApp_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x42, 135, ProgrammData_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x43, 3, StopUpdating_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x44, 8, CheckAppCRC_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x45, 3, CheckAppExistence_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x46, 3, IsMemoryProtect_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x47, 4, SetMemoryProtection_cmd, MAP_FLGS(0x0, 0x0)},

    // jump to main
    {REQUEST_COM_PREFIX, 0x50, 3, GoToMainApp_cmd, MAP_FLGS(0x0, 0x0)},
    // bootload journal
    {REQUEST_COM_PREFIX, 0x91, 3, GetBootloadJournalSize_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x92, 5, GetBootloadJournalItemByNum_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x93, 3, GetAverageWorkTime_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x94, 3, EraseBootloadJournal_cmd, MAP_FLGS(0x0, 0x0)},
};

uint32_t command_amount = (sizeof(cmdMap) / sizeof(CMD_Map_t));

bool GetCurrentTimeMs_cmd(uint8_t *buff)
{
    uint32_t ms = GetUpTimeMs();
    CmdHandler.SetResponce(SUCCESS_CS, &ms, sizeof(ms));
    return true;
}

bool GetCurrentTimeS_cmd(uint8_t *buff)
{
    uint32_t s = GetUpTimeS();
    CmdHandler.SetResponce(SUCCESS_CS, &s, sizeof(s));
    return true;
}

bool GetRtcTime_cmd(uint8_t *buff)
{
    calendar_type currentTime;
    rtc_module_get_time(&currentTime);
    CmdHandler.SetResponce(SUCCESS_CS, &currentTime, sizeof(currentTime));
    return true;
}

bool SetRtcTime_cmd(uint8_t *buff)
{
    calendar_type newTime = *(reinterpret_cast<calendar_type *>(buff));
    bool result = rtc_time_set(&newTime) == 0;
    CmdHandler.SetResponce(result ? SUCCESS_CS : ERROR_CS);
    return true;
}

bool ResetRestBootloaderCnt_cmd(uint8_t *buff)
{
    restBootloadCnt = DEFAULT_REST_BOOTLOAD_CNT;
    CmdHandler.SetResponce(SUCCESS_CS);
    return true;
}

bool StartUpdating_cmd(uint8_t *buff)
{
    FW_Loader::StartUpdate();
    CmdHandler.SetResponce(SUCCESS_CS);
    return true;
}

bool EraseMainApp_cmd(uint8_t *buff)
{
    bool result = FW_Loader::EraseMainApp();
    CmdHandler.SetResponce(result ? SUCCESS_CS : ERROR_CS);
    return true;
}

bool ProgrammData_cmd(uint8_t *buff)
{
    uint32_t blockNum = *reinterpret_cast<uint32_t *>(buff + COMMAND_PACK_POS);
    uint32_t nextBlockNum = 0;
    FW_Loader::SetBlockAndDecrypt(buff + COMMAND_PACK_POS + sizeof(blockNum));
    UpdateStatus result = FW_Loader::Programm(blockNum, nextBlockNum);
    uint8_t respBuff[5];
    memcpy(respBuff, &result, 1);
    memcpy(respBuff + 1, &nextBlockNum, 4);
    CmdHandler.SetResponce(result == SuccessUS ? SUCCESS_CS : ERROR_CS, respBuff, 5);
    return true;
}

bool StopUpdating_cmd(uint8_t *buff)
{
    FW_Loader::StopUpdate();
    bootloader_flags_t flags = BootloadConfig::GetBootloaderFlags();
    flags.is_main_app_ok = true;
    flags.is_reserv_using = false;
    flags.is_fuota_using = false;
    flags.need_reprogramm_from_reserv = false;
    flags.need_reprogramm_from_fuota = false;
    BootloadConfig::SetBootloaderFlags(flags);
    CmdHandler.SetResponce(SUCCESS_CS);
    return true;
}

bool CheckAppCRC_cmd(uint8_t *buff)
{
    uint8_t crc = *reinterpret_cast<uint8_t *>(buff + COMMAND_PACK_POS);
    uint32_t length = *reinterpret_cast<uint32_t *>(buff + COMMAND_PACK_POS + sizeof(crc));
    UpdateStatus result = FW_Loader::CheckCRC(crc, length);
    CmdHandler.SetResponce(result == SuccessUS ? SUCCESS_CS : ERROR_CS, &result, sizeof(result));
    return true;
}

bool GetBootloaderVersion_cmd(uint8_t *buff)
{
    version_t version = BootloadConfig::GetBootloaderVersion();
    CmdHandler.SetResponce(SUCCESS_CS, &version, sizeof(version));
    return true;
}

bool GetHardwareVersion_cmd(uint8_t *buff)
{
    hardware_version_t version;
    version.v  = BootloadConfig::GetHardwareVersion();
    version.t = HW_TYPE;
    CmdHandler.SetResponce(SUCCESS_CS, &version, sizeof(version));
    return true;
}

bool SetHardwareVersion_cmd(uint8_t *buff)
{
    version_t version = *reinterpret_cast<version_t *>(buff + COMMAND_PACK_POS);
    BootloadConfig::SetHardwareVersion(version);
    CmdHandler.SetResponce(SUCCESS_CS);
    return true;
}

bool GetDeviceId_cmd(uint8_t *buff)
{
    uint32_t ids[3];
    ids[0] = *reinterpret_cast<uint32_t*>(MCU_ID1);
    ids[1] = *reinterpret_cast<uint32_t*>(MCU_ID2);
    ids[2] = *reinterpret_cast<uint32_t*>(MCU_ID3);
    CmdHandler.SetResponce(SUCCESS_CS, ids, sizeof(ids));
    return true;
}

bool GetProtocolVersion_cmd(uint8_t *buff)
{
    uint8_t version = WIRE_PROTOCOL_VERSION;
    CmdHandler.SetResponce(SUCCESS_CS, &version, sizeof(version));
    return true;
}

bool GoToMainApp_cmd(uint8_t *buff)
{
    bootloader_flags_t flags = BootloadConfig::GetBootloaderFlags();
    if (flags.is_main_app_ok)
    {
        CmdHandler.SetPostCmd(delay_and_load_app);
        CmdHandler.SetResponce(SUCCESS_CS);
    }
    else
        CmdHandler.SetResponce(ERROR_CS);
    return true;
}

bool WhatAppIsIt_cmd(uint8_t *buff)
{
    AppType appType = bootloader0;
    CmdHandler.SetResponce(SUCCESS_CS, &appType, sizeof(appType));
    return true;
}

bool GetBootloadJournalSize_cmd(uint8_t *buff)
{
    uint16_t size = BootloadJournal::GetSize();
    CmdHandler.SetResponce(SUCCESS_CS, &size, sizeof(size));
    return true;
}

bool GetBootloadJournalItemByNum_cmd(uint8_t *buff)
{
    uint16_t num = *reinterpret_cast<uint16_t *>(&buff[COMMAND_PACK_POS]);
    calendar_type item = BootloadJournal::GetJournalItem(num);
    CmdHandler.SetResponce(SUCCESS_CS, &item, sizeof(item));
    return true;
}

bool GetAverageWorkTime_cmd(uint8_t *buff)
{
    uint32_t averageWorkTime = BootloadJournal::GetAverageWorkTime();
    CmdHandler.SetResponce(SUCCESS_CS, &averageWorkTime, sizeof(averageWorkTime));
    return true;
}

bool EraseBootloadJournal_cmd(uint8_t *buff)
{
    BootloadJournal::EraseJournal();
    CmdHandler.SetResponce(SUCCESS_CS);
    return true;
}

bool CheckAppExistence_cmd(uint8_t *buff)
{
    bool existing = BootloadConfig::CheckMainAppExistence();
    bootloader_flags_t flags = BootloadConfig::GetBootloaderFlags();
    existing &= flags.is_main_app_ok;
    CmdHandler.SetResponce(SUCCESS_CS, &existing, sizeof(existing));
    return true;
}

bool IsMemoryProtect_cmd(uint8_t *buff)
{
    bool protection = FlashService::IsMemoryProtect();
    CmdHandler.SetResponce(SUCCESS_CS, &protection, sizeof(protection));
    return true;
}

bool SetMemoryProtection_cmd(uint8_t *buff)
{
    bool protection = *reinterpret_cast<bool *>(buff + COMMAND_PACK_POS);
    bool result = FlashService::SetMemoryProtect(protection);
    CmdHandler.SetResponce(result ? SUCCESS_CS : ERROR_CS);
    return true;
}

// bool GetMainAppVersion_cmd (uint8_t *buff){
//     uint32_t version = 0;
//     uint8_t respSize = sizeof(version);
//     memcpy(CmdHandler.resp, &version, respSize);
//     CmdHandler.respCnt = respSize;
//     return true;
// }
