#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "at32f403a_407.h"
#include "Consts.h"
#include "UartDrv.h"
#include "Metrics.h"
#include <stdbool.h>
#include "ErrorJournal.h"
#include "CommandHandler.h"
#include "UpTime.h"
#include "main_defines.h"
#include "Enums.h"


bool WhatAppIsIt_cmd(uint8_t* buff);

bool GetMainAppVersion_cmd(uint8_t* buff);

bool GetCurrentTimeMs_cmd(uint8_t* buff);
bool GetCurrentTimeS_cmd(uint8_t* buff);
bool GetRtcTime_cmd(uint8_t* buff);
bool SetRtcTime_cmd(uint8_t* buff);

bool SetWorkWithUart_cmd(uint8_t* buff);

bool GetSerialNum_cmd(uint8_t* buff);
bool SetSerialNum_cmd(uint8_t* buff);
bool GetAssemblyDate_cmd(uint8_t* buff);
bool SetAssemblyDate_cmd(uint8_t* buff);
bool GetFrequency_cmd(uint8_t* buff);
bool SetFrequency_cmd(uint8_t* buff);

bool GetErrCnt_cmd(uint8_t* buff);

bool GetCacheSize_cmd(uint8_t* buff);
bool GetHandledSize_cmd(uint8_t* buff);
bool GetSerialByNum_cmd(uint8_t* buff);
bool AddSerialToHandled_cmd(uint8_t* buff);
bool RemoveSerialAsHandled_cmd(uint8_t* buff);
bool SaveHandledItems_cmd(uint8_t* buff);
bool GetMetricsBySerialNum_cmd(uint8_t* buff);
bool ClearHandledList_cmd(uint8_t* buff);

bool Reset_cmd(uint8_t* buff);

extern CMD_Map_t cmdMap[];
extern uint32_t command_amount;

#endif
