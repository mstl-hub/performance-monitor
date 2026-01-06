#include "mb.h"
#include "mbport.h"
#include "port_internal.h"
#include "UartDrv.h"
#include "at32f403a_407_usart.h"
#include "at32f403a_407_gpio.h"
#include "at32f403a_407_dma.h"

/* UART handle for Modbus */
static usart_type* mb_usart = MB_USART;

BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity, UCHAR ucStopBits)
{
    UNUSED(ucPORT);

    /* Map Modbus parameters to AT32 USART types */
    usart_data_bit_num_type data_bits;
    usart_stop_bit_num_type stop_bits;
    usart_parity_selection_type parity;
    
    /* Map stop bits from argument */
    switch (ucStopBits)
    {
        case 1:
            stop_bits = USART_STOP_1_BIT;
            break;
        case 2:
            stop_bits = USART_STOP_2_BIT;
            break;
        default:
            return FALSE;
    }

    /* Configure data bits and parity from arguments */
    if (ucDataBits == 8)
    {
        if (eParity == MB_PAR_NONE)
        {
            data_bits = USART_DATA_8BITS;
            parity = USART_PARITY_NONE;
        }
        else
        {
            data_bits = USART_DATA_9BITS;
            parity = (eParity == MB_PAR_ODD) ? USART_PARITY_ODD : USART_PARITY_EVEN;
        }
    }
    else
    {
        return FALSE; /* Unsupported data bits configuration */
    }

    /* Initialize UART using flexible UartDrv in INTERRUPT mode */
    usart_init_type mb_uart_config = {
        .usart = mb_usart,
        .baudrate = ulBaudRate,
        .data_bit = data_bits,
        .stop_bit = stop_bits,
        .parity = parity,
        .mode = USART_MODE_TX_RX,
        .hardware_flow_control = USART_HARDWARE_FLOW_NONE,
        .rx_mode = UART_RX_MODE_INTERRUPT,  // ← Use INTERRUPT mode for Modbus
        .tx_gpio_port = MB_TX_GPIO_PORT,
        .tx_gpio_pin = MB_TX_GPIO_PIN,
        .rx_gpio_port = MB_RX_GPIO_PORT,
        .rx_gpio_pin = MB_RX_GPIO_PIN,
        .tx_gpio_clk = MB_TX_GPIO_CLK,
        .rx_gpio_clk = MB_RX_GPIO_CLK,
        .usart_clk = MB_USART_CLK,
        .usart_irq = MB_USART_IRQn,
        .irq_priority = MB_USART_IRQ_priority,
        .irq_subpriority = MB_USART_IRQ_subpriority
    };
    
    /* drv_uart_init will handle DMA cleanup if switching from DMA to INTERRUPT mode */
    drv_uart_init(&mb_uart_config);

    /* Disable UART interrupts initially - will be enabled by vMBPortSerialEnable() */
    usart_interrupt_enable(mb_usart, USART_RDBF_INT, FALSE);
    usart_interrupt_enable(mb_usart, USART_TDBE_INT, FALSE);

    return TRUE;
}

void vMBPortSerialEnable(BOOL rxEnable, BOOL txEnable)
{
    /* Configure receive interrupt */
    if (rxEnable)
    {
        usart_interrupt_enable(mb_usart, USART_RDBF_INT, TRUE);
    }
    else
    {
        usart_interrupt_enable(mb_usart, USART_RDBF_INT, FALSE);
    }

    /* Configure transmit interrupt */
    if (txEnable)
    {
        usart_interrupt_enable(mb_usart, USART_TDBE_INT, TRUE);
    }
    else
    {
        usart_interrupt_enable(mb_usart, USART_TDBE_INT, FALSE);
    }

    /* NVIC always enabled - control via interrupt enable bits */
    nvic_irq_enable(MB_USART_IRQn, MB_USART_IRQ_priority, MB_USART_IRQ_subpriority);
}

BOOL xMBPortSerialPutByte(CHAR byte)
{
    usart_data_transmit(mb_usart, (uint16_t)byte);
    return TRUE;
}

BOOL xMBPortSerialGetByte(CHAR *byte)
{
    *byte = (CHAR)usart_data_receive(mb_usart);
    return TRUE;
}

/* UART interrupt handler */
void USART1_IRQHandler(void)
{
    /* Check if RX interrupt is enabled AND data is available */
    if ((mb_usart->ctrl1_bit.rdbfien) && (mb_usart->sts_bit.rdbf))
    {
        vMBTimerDebugSetLow();
        pxMBFrameCBByteReceived();
    }

    /* Check if TX interrupt is enabled AND buffer is empty */
    if ((mb_usart->ctrl1_bit.tdbeien) && (mb_usart->sts_bit.tdbe))
    {
        pxMBFrameCBTransmitterEmpty();
    }
}
