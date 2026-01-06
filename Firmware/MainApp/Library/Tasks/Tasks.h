#ifndef _TASKS_H_
#define _TASKS_H_

#include "at32f403a_407.h"
#include "FlashService.h"
#include "gpio.h"
#include "wdt.h"
#include "PinDefines.h"

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

// #define LED2_PORT GPIOA
// #define LED2_PIN GPIO_PINS_1

void ledFun(void* parameters) __attribute__((noreturn));
void debugTask(void* parameters) __attribute__((noreturn));
void vApplicationIdleHook(void);

extern bool blink_once;

#endif