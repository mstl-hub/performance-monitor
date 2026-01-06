#ifndef _PORT_INTERNAL_H
#define _PORT_INTERNAL_H

/* AT32F403A Modbus Port Configuration */

/* UART Configuration for Modbus */
#define MB_USART                    USART1
#define MB_USART_CLK                CRM_USART1_PERIPH_CLOCK
#define MB_USART_CLK_ENABLE()       crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE)
#define MB_USART_IRQn               USART1_IRQn
#define MB_USART_IRQ_priority       5
#define MB_USART_IRQ_subpriority    0

/* GPIO Configuration for Modbus UART */
#define MB_TX_GPIO_PORT             GPIOA
#define MB_TX_GPIO_PIN              GPIO_PINS_9
#define MB_TX_GPIO_CLK              CRM_GPIOA_PERIPH_CLOCK
#define MB_TX_GPIO_CLK_ENABLE()     crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE)

#define MB_RX_GPIO_PORT             GPIOA
#define MB_RX_GPIO_PIN              GPIO_PINS_10
#define MB_RX_GPIO_CLK              CRM_GPIOA_PERIPH_CLOCK
#define MB_RX_GPIO_CLK_ENABLE()     crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE)

/* Timer Configuration for Modbus */
#define MB_TIM                      TMR2
#define MB_TIM_CLK                  CRM_TMR2_PERIPH_CLOCK
#define MB_TIM_CLK_ENABLE()         crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE)
#define MB_TIM_IRQn                 TMR2_GLOBAL_IRQn
#define MB_TIM_IRQ_priority         6
#define MB_TIM_IRQ_subpriority      0

/* Timer debug pin (optional) */
#define MB_TIMER_DEBUG              0  /* Set to 1 to enable timer debug output */
#if MB_TIMER_DEBUG == 1
#define MB_TIMER_DEBUG_PORT         GPIOB
#define MB_TIMER_DEBUG_PIN          GPIO_PINS_0
#define vMBTimerDebugSetHigh()      gpio_bits_set(MB_TIMER_DEBUG_PORT, MB_TIMER_DEBUG_PIN)
#define vMBTimerDebugSetLow()       gpio_bits_reset(MB_TIMER_DEBUG_PORT, MB_TIMER_DEBUG_PIN)
#else
#define vMBTimerDebugSetHigh()
#define vMBTimerDebugSetLow()
#endif

#endif /* _PORT_INTERNAL_H */
