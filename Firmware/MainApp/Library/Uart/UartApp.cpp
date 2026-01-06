#include "UartApp.h"

/*void uartFun(void *parameters) {
  UartService uartService = UartService();
  uint32_t max_time_s = GetUpTimeS() + 600; // ten minutes
  for (uint32_t now = GetUpTimeS(); now - max_time_s; now = GetUpTimeS()) {
    if (uartService.Read() > 0)
      while (uartService.TryGetCommand()) {
        uartService.HandleCurrentRxBuff();
        uartService.Send(CmdHandler.GetRespBuff(), CmdHandler.GetRespCnt());
      }
#if USE_WDT
    simple_wdt_reload();
#endif
  }
}*/

void uartFun(void *parameters) {
  (void)parameters;
  // Initialize USART1 here
  UartService uartService = UartService();
  for (;;) {
    if (uartService.Read() > 0)
      while (uartService.TryGetCommand()) {
        uartService.HandleCurrentRxBuff();
        vTaskDelay(5);
      }
    uartService.Send(CmdHandler.GetRespBuff(), CmdHandler.GetRespCnt());
    CmdHandler.SetResponce(UNKNOWN_CS, NULL, 0);
    
  }
  wdt_notify_to_subject({.uart = true});
  vTaskDelay(20); // Delay to prevent spamming the transmission
}
