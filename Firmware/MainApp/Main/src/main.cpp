/* add user code begin Header */
/**
 **************************************************************************
 * @file     main.c
 * @brief    main program
 **************************************************************************
 *                       Copyright notice & Disclaimer
 *
 * The software Board Support Package (BSP) that is made available to
 * download from Artery official website is the copyrighted work of Artery.
 * Artery authorizes customers to use, copy, and distribute the BSP
 * software and its related documentation for the purpose of design and
 * development in conjunction with Artery microcontrollers. Use of the
 * software is governed by this copyright notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
 * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
 * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
 * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
 * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 *
 **************************************************************************
 */
/* add user code end Header */

#include "main.h"

SystemServices_t services = {
  .UpTimeTmr = true,
  .FlashService = true,
  .UnitReset = true,
  .UartService = false,
  .WatchdogTimer = false,
  .BootloaderConfig = true,
};

/**
 * @brief main function.
 * @param  none
 * @retval none
 */
int main(void) {

  SystemApi::Init(NVIC_PRIORITY_GROUP_4, _240Mhz, true, reinterpret_cast<uint32_t>(_sapp));
  SystemApi::InitServices(services);

  // ErrorJournal_LoadData();

  // rtc_module_init();

  CmdHandler.SetCommands(cmdMap, command_amount);

  PBlockConfig::Init(); // Initialize P-Block configuration
  PBlockRegisters_t::Init();   // Initialize P-Block Modbus registers
  UniversalInputManager::Init(); // Initialize universal input hardware interfaces

  // xTaskCreate(modbusFun, "modbus", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(inputUpdateTask, "inputUpdate", 128, NULL, tskIDLE_PRIORITY + 1, NULL);
  // xTaskCreate(uartFun, "uart", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
  // xTaskCreate(ledFun, "led", 128, NULL, tskIDLE_PRIORITY + 1, NULL);
  // xTaskCreate(displayFun, "display", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
  // xTaskCreate(buttonsFun, "buttons", 128, NULL, tskIDLE_PRIORITY + 1, NULL);
  // xTaskCreate(unitControlFun, "unitControl", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
  // xTaskCreate(debugTask, "debug", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(CANopen_tmrTask, "CANopen_tmr", 256, NULL, tskIDLE_PRIORITY + 2, NULL);
  xTaskCreate(CANopenTask, "CANopen", 512, NULL, tskIDLE_PRIORITY + 2, NULL);
  // xTaskCreate(unitControlFun, "unitControl", 128, NULL, tskIDLE_PRIORITY + 1,
  // NULL);



#if defined DEBUG && defined LITE_GATEWAY

#endif

  vTaskStartScheduler();

  /* add user code end 2 */

  for (;;) {
    /* add user code begin 3 */

    /* add user code end 3 */
  }
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
  (void)pxTask;
  (void)pcTaskName;
  for (;;)
    ;
}

void vApplicationIdleHook(void) {
  // This function is called by the idle task
  // Can be used for low-priority background tasks
}

void Error_Handler(void) {
  __disable_irq();
  while (1) {
  }
}
