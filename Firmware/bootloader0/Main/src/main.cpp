/**
 **************************************************************************
 * @file     main.c
 * @brief    main program
 **************************************************************************
 */

#include "main.h"

/**
 * @brief main function.
 */
int main(void)
{
    nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
#ifdef END_DEVICE_TEMPERATURE_SENSOR
    ClockCfg::SetSpeed(_4dot8Mhz);
#endif

#ifdef LITE_GATEWAY
    ClockCfg::SetSpeed(_240Mhz);
#endif
    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

    // set nvic
    SCB->VTOR = reinterpret_cast<uint32_t>(&_sboot);

    __enable_irq();

    /* jtag-dp disabled and sw-dp enabled */
    gpio_pin_remap_config(SWJTAG_GMUX_010, TRUE); // enable PB3

    init_bpr_logick();
    // increment failed booting cnt
    set_failed_booting();

    if (usbService.IsConnect())
        init_UpTime_Tmr();
    else
    {
        state_machine.stop_usb = false;
        save_state_machine();
#ifdef LITE_GATEWAY
        init_UpTime_Tmr();
#endif
    }

    rtc_module_init();
    FlashService::Init();
#ifndef DEBUG
    FlashService::SetMemoryProtect(true);
#endif

    simple_wdt_init();

#ifdef LITE_GATEWAY
    CmdHandler.SetCommands(cmdMap, command_amount);
    drv_uart_init();
#endif
    simple_wdt_reload();

    if (usbService.IsConnect() && !state_machine.stop_usb)
    {
        simple_wdt_reload();
#ifdef END_DEVICE_TEMPERATURE_SENSOR
        ClockCfg::SetSpeed(_12Mhz);
        CmdHandler.SetCommands(cmdMap, command_amount);
#endif
        usbService.initUSB = true;
        simple_wdt_reload();
        usbService.Init();
        simple_wdt_reload();
    }
    else
    {
        if (state_machine.stop_usb)
        {
            state_machine.stop_usb = false;
            save_state_machine();
        }
    }
    simple_wdt_reload();
    BootloadConfig::Init();
    simple_wdt_reload();
    BootloadJournal::Init();
    simple_wdt_reload();
    BootloadJournal::CheckWDRTS();
    simple_wdt_reload();

    first_load_blink();

    if (usbService.IsConnect() && usbService.initUSB)
        usb_process();

#ifdef LITE_GATEWAY
    if (state_machine.work_with_uart)
        uartFun(NULL);
#endif
    simple_wdt_reload();
    BootloadConfig::HandleBootFlags();
    simple_wdt_reload();
    if (BootloadConfig::CanRunMainApp())
    {
        simple_wdt_reload();
        load_app();
    }
    else
    {
        app_wasnt_found_blink();
#ifdef END_DEVICE_TEMPERATURE_SENSOR
        pwc_standby_mode_enter();
#endif
#ifdef LITE_GATEWAY
        load_app();
        mcu_safe_reset();
#endif
    }

    while (1)
        __NOP();
}
