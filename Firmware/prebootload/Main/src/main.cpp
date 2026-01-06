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
#else
    ClockCfg::SetSpeed(_240Mhz);
#endif
    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

    /* jtag-dp disabled and sw-dp enabled */
    gpio_pin_remap_config(SWJTAG_GMUX_010, TRUE); // enable PB3

    init_bpr_logick();

    watchdog_init();

    load_app(state_machine.run_main_app ? MAIN_APP_ADDRESS : BOOTLOADER_ADDRESS);

    while (1)
        __NOP();
}
