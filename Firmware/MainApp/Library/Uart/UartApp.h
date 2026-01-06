#ifndef _UART_APP_H_
#define _UART_APP_H_

#include "at32f403a_407.h"
#include "UartApi.h"
#include "Command.h"
#include "gpio.h"

#ifdef FREE_RTOS_IS_IN_USED
extern "C"
{
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <semphr.h>
}

#include "wdt.h"

#endif

#include "at32f403a_407_usart.h"

void uartFun(void *parameters);

#endif /* _UART_APP_H_ */