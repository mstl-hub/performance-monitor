#ifndef _USB_APP_H_
#define _USB_APP_H_ 


#include "at32f403a_407.h"
#include "usb_drv.h"
#include "usb_app.h"
#include "Command.h"
#include "PacketWrapper.h"
#include "usb_process.h"
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

void usbFun(void* parameters);


#endif