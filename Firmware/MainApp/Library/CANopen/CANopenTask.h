#ifndef _CANOPEN_TASK_H_
#define _CANOPEN_TASK_H_

#include "CanApi.h"
#include "FreeRTOS.h"
#include "at32f403a_407.h"
#include "task.h"
#include "CANopen.h"          // Main CANopenNode API (includes all 301/* headers)
#include "CO_driver.h"        // Driver glue (P-block/Library/CANopen)
#include "OD.h"               // Object Dictionary
#include "CO_storageBlank.h"  // Storage support

extern CO_t *CO;

void CANopenTask(void *parameters);
void tmrTask_thread(void *parameters);

#endif