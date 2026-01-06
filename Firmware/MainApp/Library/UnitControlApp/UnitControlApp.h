#include "at32f403a_407.h"
#include "unitControl.h"
#include "wdt.h"

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

void unitControlFun(void* parameters);