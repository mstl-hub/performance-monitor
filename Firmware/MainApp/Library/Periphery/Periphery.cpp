#include "Periphery.h"
#include "P-Block-struct.h"
#include "FreeRTOS.h"
#include "task.h"

void inputUpdateTask(void *parameters)
{
    (void)parameters;

    for (;;)
    {
        // Update universal inputs every 100ms
        PBlockRegisters_t::UpdateInputs();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

