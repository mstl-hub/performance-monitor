#include "CANopen_tmrTask.h"

static volatile uint32_t CO_timer1ms = 0;

/* timer thread executes in constant intervals ********************************/
void CANopen_tmrTask(void *parameters) {
    TickType_t lastWake = xTaskGetTickCount();
    for (;;) {
      if (CO != NULL && CO->CANmodule != NULL) {
        CO_LOCK_OD(CO->CANmodule);
        if (!CO->nodeIdUnconfigured && CO->CANmodule->CANnormal) {
          bool_t syncWas = false;
          /* get time difference since last function call */
          uint32_t timeDifference_us = 1000;
  
  #if (CO_CONFIG_SYNC) & CO_CONFIG_SYNC_ENABLE
          syncWas = CO_process_SYNC(CO, timeDifference_us, NULL);
  #endif
  #if (CO_CONFIG_PDO) & CO_CONFIG_RPDO_ENABLE
          CO_process_RPDO(CO, syncWas, timeDifference_us, NULL);
  #endif
  #if (CO_CONFIG_PDO) & CO_CONFIG_TPDO_ENABLE
          CO_process_TPDO(CO, syncWas, timeDifference_us, NULL);
  #endif
  
          /* Further I/O or nonblocking application code may go here. */
        }
        CO_UNLOCK_OD(CO->CANmodule);
      }
  
      CO_timer1ms++; // increments every 1 ms
  
      vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(1)); // 1 ms period
    }
  }