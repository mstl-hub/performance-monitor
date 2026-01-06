#ifndef _MAIN_H_
#define _MAIN_H_

#include "at32f403a_407.h"
#include "Tasks.h"
#include "UartApp.h"
#include "UpTime.h"
#include "UartDrv.h"
#include "mcu_hal.h"
#include "UnitControlApp.h"
#include "wdt.h"
#include "usbApp.h"
#include "ModbusApp.h"
#include "DisplayApp.h"
#include "ButtonsApp.h"
#include "Command.h"
#include "BootloadConfig.h"
#include "ClockCfg.h"
#include "PBlockConfig.h"
#include "SystemApi.h"
#include "P-Block-struct.h"
#include "UniversalInputManager.h"
#include "Periphery.h"
#include "CANopenTask.h"
#include "CANopen_tmrTask.h"

#ifdef FREE_RTOS_IS_IN_USED
extern "C" {
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <semphr.h>
}

#endif

extern const uint8_t _sapp[];  // Linker script symbol for application start address (absolute address)

#endif /* _MAIN_H_ */

