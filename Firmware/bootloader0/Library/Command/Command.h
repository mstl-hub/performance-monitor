#ifndef _COMMAND_H__
#define _COMMAND_H__

#include "at32f403a_407.h"
#include "Consts.h"
#include "UartDrv.h"
#include <stdbool.h>
#include "CRC.h"
#include "UpTime.h"
#include "Bootload.h"
#include "CommandHandler.h"
#include "FW_Loader.h"
#include "TafcoCrypt.h"
#include "main_defines.h"
#include "BootloadConfig.h"
#include "BootloadJournal.h"
#include "rtc_module.h"
#include "TafcoCrypt.h"
#include "Enums.h"

bool ResetRestBootloaderCnt_cmd(uint8_t *buff);

bool WhatAppIsIt_cmd(uint8_t *buff);

bool GetHardwareVersion_cmd(uint8_t *buff);
bool GetDeviceId_cmd(uint8_t *buff);
bool GetBootloaderVersion_cmd(uint8_t *buff);
bool SetHardwareVersion_cmd(uint8_t *buff);
bool GetMainAppVersion_cmd(uint8_t *buff);
bool GetProtocolVersion_cmd(uint8_t *buff);

bool GetCurrentTimeMs_cmd(uint8_t *buff);
bool GetCurrentTimeS_cmd(uint8_t *buff);
bool GetRtcTime_cmd(uint8_t *buff);
bool SetRtcTime_cmd(uint8_t *buff);

bool StartUpdating_cmd(uint8_t *buff);
bool EraseMainApp_cmd(uint8_t *buff);
bool ProgrammData_cmd(uint8_t *buff);
bool StopUpdating_cmd(uint8_t *buff);
bool CheckAppCRC_cmd(uint8_t *buff);
bool CheckAppExistence_cmd(uint8_t *buff);
bool IsMemoryProtect_cmd(uint8_t *buff);
bool SetMemoryProtection_cmd(uint8_t *buff);

bool GetBootloadJournalSize_cmd(uint8_t *buff);
bool GetBootloadJournalItemByNum_cmd(uint8_t *buff);
bool GetAverageWorkTime_cmd(uint8_t *buff);
bool EraseBootloadJournal_cmd(uint8_t *buff);

bool GoToMainApp_cmd(uint8_t *buff);

extern CMD_Map_t cmdMap[];
extern uint32_t command_amount;

#endif
