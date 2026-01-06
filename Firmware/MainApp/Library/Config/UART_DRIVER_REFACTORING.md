# UART Driver Refactoring Summary

## Overview
Updated `UartDrv` to support both DMA and interrupt modes, making it flexible for different use cases (regular UART communication and Modbus RTU).

## Changes Made

### 1. TafcoMcuCore/Library/Uart/UartDrv.h

**Added:**
- `uart_rx_mode_type` enum for selecting DMA or interrupt mode
- Extended `usart_init_type` structure with new fields:
  - `rx_mode` - Reception mode (DMA/Interrupt)
  - `tx_gpio_port`, `tx_gpio_pin` - TX GPIO configuration
  - `rx_gpio_port`, `rx_gpio_pin` - RX GPIO configuration
  - `tx_gpio_clk`, `rx_gpio_clk` - GPIO clocks
  - `usart_clk` - USART peripheral clock
  - `usart_irq` - IRQ number
  - `irq_priority`, `irq_subpriority` - Interrupt priorities

### 2. TafcoMcuCore/Library/Uart/UartDrv.cpp

**Modified `drv_uart_init_ex()`:**
- Flexible GPIO and clock configuration based on parameters
- Conditional setup for DMA or interrupt mode:
  - **DMA mode**: Enables DMA channel, circular buffer, polling-based reception
  - **Interrupt mode**: Disables DMA, configures NVIC, event-driven reception
- Proper handling of different USART peripherals

**Modified `drv_uart_init()`:**
- Updated to include all new configuration fields
- Defaults to DMA mode for backward compatibility

### 3. P-block/Library/Modbus/port/portserial.c

**Simplified `xMBPortSerialInit()`:**
- Now uses `drv_uart_init_ex()` instead of manual initialization
- Configured for **INTERRUPT mode** (required for Modbus RTU timing)
- Automatically disables DMA if USART1 is used (prevents conflicts)
- All GPIO/clock/interrupt configuration handled by UartDrv

### 4. P-block/Library/Modbus/port/port_internal.h

**Added:**
- `MB_USART_CLK` - USART clock definition
- `MB_TX_GPIO_CLK` - TX GPIO clock definition
- `MB_RX_GPIO_CLK` - RX GPIO clock definition

## Usage Examples

### Example 1: DMA Mode (Regular UART)
```cpp
usart_init_type uart_config = {
    .usart = USART1,
    .baudrate = 115200,
    .data_bit = USART_DATA_8BITS,
    .stop_bit = USART_STOP_1_BIT,
    .parity = USART_PARITY_NONE,
    .mode = USART_MODE_TX_RX,
    .hardware_flow_control = USART_HARDWARE_FLOW_NONE,
    .rx_mode = UART_RX_MODE_DMA,  // ← DMA mode
    .tx_gpio_port = GPIOA,
    .tx_gpio_pin = GPIO_PINS_9,
    .rx_gpio_port = GPIOA,
    .rx_gpio_pin = GPIO_PINS_10,
    .tx_gpio_clk = CRM_GPIOA_PERIPH_CLOCK,
    .rx_gpio_clk = CRM_GPIOA_PERIPH_CLOCK,
    .usart_clk = CRM_USART1_PERIPH_CLOCK,
    .usart_irq = (IRQn_Type)0,  // Not used
    .irq_priority = 0,
    .irq_subpriority = 0
};
drv_uart_init_ex(&uart_config);
```

### Example 2: Interrupt Mode (Modbus)
```cpp
usart_init_type mb_uart_config = {
    .usart = USART1,
    .baudrate = 9600,
    .data_bit = USART_DATA_8BITS,
    .stop_bit = USART_STOP_1_BIT,
    .parity = USART_PARITY_NONE,
    .mode = USART_MODE_TX_RX,
    .hardware_flow_control = USART_HARDWARE_FLOW_NONE,
    .rx_mode = UART_RX_MODE_INTERRUPT,  // ← Interrupt mode
    .tx_gpio_port = GPIOA,
    .tx_gpio_pin = GPIO_PINS_9,
    .rx_gpio_port = GPIOA,
    .rx_gpio_pin = GPIO_PINS_10,
    .tx_gpio_clk = CRM_GPIOA_PERIPH_CLOCK,
    .rx_gpio_clk = CRM_GPIOA_PERIPH_CLOCK,
    .usart_clk = CRM_USART1_PERIPH_CLOCK,
    .usart_irq = USART1_IRQn,
    .irq_priority = 5,
    .irq_subpriority = 0
};
drv_uart_init_ex(&mb_uart_config);
```

## Benefits

✅ **Unified Driver** - Single driver supports multiple use cases
✅ **Flexible Configuration** - Can use any USART peripheral with any GPIO pins
✅ **Mode Selection** - DMA for throughput, interrupts for timing-critical protocols
✅ **Clean Separation** - Each mode properly configured without conflicts
✅ **Backward Compatible** - Existing code using `drv_uart_init()` still works
✅ **Modbus Integration** - Modbus now uses standardized driver instead of custom code
✅ **Reduced Code Duplication** - GPIO/clock setup reused across applications

## Notes

- **DMA Mode**: Best for high-throughput, polling-based applications
- **Interrupt Mode**: Best for timing-critical protocols like Modbus RTU
- Both modes can coexist if using different USART peripherals
- Modbus initialization automatically disables DMA on USART1 to prevent conflicts

