#include "Tasks.h"
#include "ModbusApp.h"
#include "UartDrv.h"
#include "UpTime.h"

#ifdef LITE_GATEWAY

bool blink_once{false};

void ledFun(void *parameters)
{
    (void)parameters;
    crm_periph_clock_enable(DEBUG_LED_CRM_PERIPH_CLOCK, TRUE);
    pinInit(DEBUG_LED_PORT, DEBUG_LED_PIN, OUTPUT, GPIO_PULL_NONE);
    gpio_bits_set(DEBUG_LED_PORT, DEBUG_LED_PIN);
    for (;;)
    {
        if (blink_once)
        {
            blink_once = false;
            gpio_bits_reset(DEBUG_LED_PORT, DEBUG_LED_PIN);
            vTaskDelay(20);
            gpio_bits_set(DEBUG_LED_PORT, DEBUG_LED_PIN);
        }
        wdt_notify_to_subject({.blinky = true});
        vTaskDelay(10);
    }
}

void vApplicationIdleHook(void)
{
    wdt_try_reload();

}

#else

// P-block specific LED function
void ledFun(void *parameters)
{
    (void)parameters;
    // Simple LED blink task for P-block
    for (;;)
    {
        // Basic LED functionality - can be enhanced later
        vTaskDelay(1000); // Delay 1 second
    }
}

// Helper function to convert uint32_t to string
static uint16_t uint32_to_str(uint32_t value, uint8_t* buffer, uint16_t max_len)
{
    if (max_len < 2)
    {
        return 0;
    }
    
    uint16_t len{0};
    uint32_t divisor{1000000000}; // Max 10 digits for uint32_t
    bool started{false};
    
    // Handle zero case
    if (value == 0)
    {
        buffer[0] = '0';
        return 1;
    }
    
    while (divisor > 0)
    {
        uint8_t digit = static_cast<uint8_t>((value / divisor) % 10);
        if (digit != 0 || started)
        {
            if (len >= max_len - 1)
            {
                break;
            }
            buffer[len++] = '0' + digit;
            started = true;
        }
        divisor /= 10;
    }
    
    return len;
}

// Helper function to append string literal
static void append_literal(uint8_t* dest, uint16_t* pos, const char* src)
{
    while (*src != '\0')
    {
        dest[(*pos)++] = static_cast<uint8_t>(*src++);
    }
}

// Debug task for UART peripheral verification
void debugTask(void *parameters)
{
    (void)parameters;
    
    constexpr uint16_t BUFFER_SIZE{128};
    uint8_t debug_buffer[BUFFER_SIZE]{};
    uint32_t message_counter{0};
    
    // Wait for system to stabilize
    vTaskDelay(1000);
    
    for (;;)
    {
        // Get system uptime
        uint32_t uptime_s = GetUpTimeS();
        uint32_t uptime_ms = GetUpTimeMs() % 1000;
        
        uint16_t pos{0};
        
        // Build message manually
        append_literal(debug_buffer, &pos, "\r\n[DEBUG] Msg#");
        pos += uint32_to_str(message_counter++, &debug_buffer[pos], 
                            BUFFER_SIZE - pos);
        append_literal(debug_buffer, &pos, " | Uptime: ");
        pos += uint32_to_str(uptime_s, &debug_buffer[pos], BUFFER_SIZE - pos);
        append_literal(debug_buffer, &pos, ".");
        
        // Add milliseconds (always 3 digits)
        if (uptime_ms < 100)
        {
            debug_buffer[pos++] = '0';
        }
        if (uptime_ms < 10)
        {
            debug_buffer[pos++] = '0';
        }
        pos += uint32_to_str(uptime_ms, &debug_buffer[pos], BUFFER_SIZE - pos);
        
        append_literal(debug_buffer, &pos, "s | FreeRTOS: OK | UART: TX\r\n");
        
        // Send via UART
        drv_uart_transmit(USART1, debug_buffer, pos);
        
        // Send every 2 seconds
        vTaskDelay(2000);
    }
}

#endif
