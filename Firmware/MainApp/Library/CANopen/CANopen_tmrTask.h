#ifndef _CANOPEN_TMR_TASK_H_
#define _CANOPEN_TMR_TASK_H_

#include <at32f403a_407.h>
#include "FreeRTOS.h"
#include "task.h"
#include "CANopenTask.h"

void CANopen_tmrTask(void *parameters);

#endif