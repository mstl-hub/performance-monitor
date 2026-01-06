#include "usbApp.h"


// UsbService usbService = UsbService();

void usbFun(void *parameters)
{
    (void)parameters;
    usbService.Init();
    while (true)
    {
        if (usbService.IsConnect())
        {
            uint16_t new_data_len = usbService.Read();
            if (new_data_len)
            {
                if (usbService.TryGetCommand())
                {
                    usbService.HandleCurrentRxBuff();
                    usbService.Send(CmdHandler.GetRespBuff(), CmdHandler.GetRespCnt());
                    vTaskDelay(10);
                    CmdHandler.DoPostCommand();
                }
            }
        }
        wdt_notify_to_subject({.usb = true});
        vTaskDelay(5);
    }
}
