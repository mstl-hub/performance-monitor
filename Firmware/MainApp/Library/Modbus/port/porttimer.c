#include "mb.h"
#include "mbport.h"
#include "port_internal.h"
#include "at32f403a_407_tmr.h"
#include "TimerDrv.h"
#include <stddef.h>

/* Static variables */
static USHORT timeout = 0;
static USHORT downcounter = 0;

/* Static timer handle for easy access */
static tmr_type* mb_timer_handle = NULL;

BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
    /* Configure timer using new wrapper */
    timer_init_type mb_timer_config = {
        .timer = MB_TIM,
        .period_us = 50,                    /* 50us tick period */
        .count_mode = TMR_COUNT_UP,
        .clock_div = TMR_CLOCK_DIV1,
        .repetition_counter = 0,
        .enable_irq = TRUE,                 /* Enable NVIC interrupts */
        .timer_irq = MB_TIM_IRQn,
        .irq_priority = MB_TIM_IRQ_priority,
        .irq_subpriority = MB_TIM_IRQ_subpriority,
        .timer_clk = MB_TIM_CLK
    };

    /* Store timeout value */
    timeout = usTim1Timerout50us;
    
    /* Store timer handle for later use */
    mb_timer_handle = MB_TIM;

    /* Initialize timer using flexible driver */
    drv_timer_init(&mb_timer_config);

    /* Setup debug output pin if enabled */
#if MB_TIMER_DEBUG == 1
    gpio_init_type gpio_init_struct;

    /* Enable GPIO clock */
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

    /* Configure debug GPIO pin */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_pins = MB_TIMER_DEBUG_PIN;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(MB_TIMER_DEBUG_PORT, &gpio_init_struct);

    /* Initial state: low */
    vMBTimerDebugSetLow();
#endif

    return TRUE;
}

void vMBPortTimersEnable(void)
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit() */
    downcounter = timeout;

    /* Set debug pin high to indicate timer is active */
    vMBTimerDebugSetHigh();

    /* Clear timer interrupt flag */
    drv_timer_clear_flag(mb_timer_handle, TIMER_INT_OVERFLOW);

    /* Enable timer interrupt */
    drv_timer_interrupt_enable(mb_timer_handle, TIMER_INT_OVERFLOW, TRUE);

    /* Enable timer counter */
    drv_timer_enable(mb_timer_handle, TRUE);
}

void vMBPortTimersDisable(void)
{
    /* Disable timer counter */
    drv_timer_enable(mb_timer_handle, FALSE);

    /* Disable timer interrupt */
    drv_timer_interrupt_enable(mb_timer_handle, TIMER_INT_OVERFLOW, FALSE);

    /* Set debug pin low to indicate timer is disabled */
    vMBTimerDebugSetLow();
}

/* Timer interrupt handler */
void TMR2_GLOBAL_IRQHandler(void)
{
    /* Check if overflow interrupt flag is set */
    if (drv_timer_get_flag(mb_timer_handle, TIMER_INT_OVERFLOW))
    {
        /* Clear overflow interrupt flag */
        drv_timer_clear_flag(mb_timer_handle, TIMER_INT_OVERFLOW);

        /* Decrement down-counter and check if reached zero */
        if (--downcounter == 0)
        {
            /* Timer expired, call the callback function */
            vMBTimerDebugSetLow();
            pxMBPortCBTimerExpired();
        }
    }
}
