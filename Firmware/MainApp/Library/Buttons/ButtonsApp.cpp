#include "ButtonsApp.h"
#include "FreeRTOS.h"
#include "task.h"

void buttonsFun(void *parameters)
{
    (void)parameters;
    // Button input handling task
    for (;;)
    {
        // Button scanning and event handling - to be implemented
        vTaskDelay(50); // Poll buttons frequently
    }
}
