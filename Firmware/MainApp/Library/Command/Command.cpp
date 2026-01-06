#include "Command.h"
#include "Structs.h"
#include "rtc_module.h"

CMD_Map_t cmdMap[] = {
    // Application identification
    {REQUEST_COM_PREFIX, 0x10, 3, WhatAppIsIt_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x11, 3, GetMainAppVersion_cmd, MAP_FLGS(0x0, 0x0)},

    // Time commands
    {REQUEST_COM_PREFIX, 0x30, 3, GetCurrentTimeMs_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x31, 3, GetCurrentTimeS_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x32, 3, GetRtcTime_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x33, 11, SetRtcTime_cmd, MAP_FLGS(0x0, 0x0)},

    // UART control
    {REQUEST_COM_PREFIX, 0x34, 4, SetWorkWithUart_cmd, MAP_FLGS(0x0, 0x0)},

    // Serial number and assembly
    {REQUEST_COM_PREFIX, 0x48, 3, GetSerialNum_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x49, 11, SetSerialNum_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x4A, 3, GetAssemblyDate_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x4B, 11, SetAssemblyDate_cmd, MAP_FLGS(0x0, 0x0)},

    // Frequency and error count
    {REQUEST_COM_PREFIX, 0x4C, 3, GetFrequency_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x4D, 7, SetFrequency_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x4E, 3, GetErrCnt_cmd, MAP_FLGS(0x0, 0x0)},

    // Cache/handled items management
    {REQUEST_COM_PREFIX, 0x50, 3, GetCacheSize_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x51, 3, GetHandledSize_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x52, 7, GetSerialByNum_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x53, 11, AddSerialToHandled_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x54, 11, RemoveSerialAsHandled_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x55, 3, SaveHandledItems_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x56, 7, GetMetricsBySerialNum_cmd, MAP_FLGS(0x0, 0x0)},
    {REQUEST_COM_PREFIX, 0x57, 3, ClearHandledList_cmd, MAP_FLGS(0x0, 0x0)},

    // Reset command
    {REQUEST_COM_PREFIX, 0xFF, 3, Reset_cmd, MAP_FLGS(0x0, 0x0)},
};

uint32_t command_amount = (sizeof(cmdMap) / sizeof(CMD_Map_t));

// Command implementations

bool WhatAppIsIt_cmd(uint8_t* buff)
{
    // Return application type - peripheral-block
    AppType appType = end_device;
    CmdHandler.SetResponce(SUCCESS_CS, &appType, sizeof(appType));
    return true;
}

bool GetMainAppVersion_cmd(uint8_t* buff)
{
    // Return main application version
    version_t version = {1, 0, 0, release}; // Example version
    CmdHandler.SetResponce(SUCCESS_CS, &version, sizeof(version));
    return true;
}

bool GetCurrentTimeMs_cmd(uint8_t* buff)
{
    uint32_t ms = GetUpTimeMs();
    CmdHandler.SetResponce(SUCCESS_CS, &ms, sizeof(ms));
    return true;
}

bool GetCurrentTimeS_cmd(uint8_t* buff)
{
    uint32_t s = GetUpTimeS();
    CmdHandler.SetResponce(SUCCESS_CS, &s, sizeof(s));
    return true;
}

bool GetRtcTime_cmd(uint8_t* buff)
{
    calendar_type currentTime;
    rtc_module_get_time(&currentTime);
    CmdHandler.SetResponce(SUCCESS_CS, &currentTime, sizeof(currentTime));
    return true;
}

bool SetRtcTime_cmd(uint8_t* buff)
{
    calendar_type newTime = *(reinterpret_cast<calendar_type*>(buff));
    bool result = rtc_time_set(&newTime) == 0;
    CmdHandler.SetResponce(result ? SUCCESS_CS : ERROR_CS);
    return true;
}

bool SetWorkWithUart_cmd(uint8_t* buff)
{
    // TODO: Implement UART work mode setting
    CmdHandler.SetResponce(SUCCESS_CS);
    return true;
}


bool GetSerialNum_cmd(uint8_t* buff)
{
    // TODO: Implement Serial Number retrieval
    uint32_t serialNum = 0;
    CmdHandler.SetResponce(SUCCESS_CS, &serialNum, sizeof(serialNum));
    return true;
}

bool SetSerialNum_cmd(uint8_t* buff)
{
    // TODO: Implement Serial Number setting
    CmdHandler.SetResponce(SUCCESS_CS);
    return true;
}

bool GetAssemblyDate_cmd(uint8_t* buff)
{
    // TODO: Implement Assembly Date retrieval
    calendar_type assemblyDate;
    CmdHandler.SetResponce(SUCCESS_CS, &assemblyDate, sizeof(assemblyDate));
    return true;
}

bool SetAssemblyDate_cmd(uint8_t* buff)
{
    // TODO: Implement Assembly Date setting
    CmdHandler.SetResponce(SUCCESS_CS);
    return true;
}


bool GetFrequency_cmd(uint8_t* buff)
{
    // TODO: Implement Frequency retrieval
    uint32_t frequency = 0;
    CmdHandler.SetResponce(SUCCESS_CS, &frequency, sizeof(frequency));
    return true;
}

bool SetFrequency_cmd(uint8_t* buff)
{
    // TODO: Implement Frequency setting
    CmdHandler.SetResponce(SUCCESS_CS);
    return true;
}

bool GetErrCnt_cmd(uint8_t* buff)
{
    // TODO: Implement Error Count retrieval
    uint32_t errCnt = 0;
    CmdHandler.SetResponce(SUCCESS_CS, &errCnt, sizeof(errCnt));
    return true;
}

bool GetCacheSize_cmd(uint8_t* buff)
{
    // TODO: Implement Cache Size retrieval
    uint32_t cacheSize = 0;
    CmdHandler.SetResponce(SUCCESS_CS, &cacheSize, sizeof(cacheSize));
    return true;
}

bool GetHandledSize_cmd(uint8_t* buff)
{
    // TODO: Implement Handled Size retrieval
    uint32_t handledSize = 0;
    CmdHandler.SetResponce(SUCCESS_CS, &handledSize, sizeof(handledSize));
    return true;
}

bool GetSerialByNum_cmd(uint8_t* buff)
{
    // TODO: Implement Get Serial By Number
    uint32_t serialNum = 0;
    CmdHandler.SetResponce(SUCCESS_CS, &serialNum, sizeof(serialNum));
    return true;
}

bool AddSerialToHandled_cmd(uint8_t* buff)
{
    // TODO: Implement Add Serial To Handled
    CmdHandler.SetResponce(SUCCESS_CS);
    return true;
}

bool RemoveSerialAsHandled_cmd(uint8_t* buff)
{
    // TODO: Implement Remove Serial As Handled
    CmdHandler.SetResponce(SUCCESS_CS);
    return true;
}

bool SaveHandledItems_cmd(uint8_t* buff)
{
    // TODO: Implement Save Handled Items
    CmdHandler.SetResponce(SUCCESS_CS);
    return true;
}

bool GetMetricsBySerialNum_cmd(uint8_t* buff)
{
    // TODO: Implement Get Metrics By Serial Number
    uint8_t metrics[64] = {0}; // Example metrics buffer
    CmdHandler.SetResponce(SUCCESS_CS, metrics, sizeof(metrics));
    return true;
}

bool ClearHandledList_cmd(uint8_t* buff)
{
    // TODO: Implement Clear Handled List
    CmdHandler.SetResponce(SUCCESS_CS);
    return true;
}

bool Reset_cmd(uint8_t* buff)
{
    // Reset the system
    NVIC_SystemReset();
    return true;
}
