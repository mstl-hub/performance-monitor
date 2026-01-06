#ifndef __GPIO_INPUT_DRIVER_H__
#define __GPIO_INPUT_DRIVER_H__

#include <stdint.h>
#include "at32f403a_407_gpio.h"
#include "at32f403a_407_crm.h"
#include "P-Block-struct.h"

/**
 * @brief GPIO Driver for Universal Digital Inputs
 * Supports 11 digital input channels with configurable pull-up/down
 * Used for digital mode of universal inputs
 */
class GPIOInputDriver {
public:
    /**
     * @brief GPIO pin mapping for universal inputs
     * Maps input numbers (1-11) to GPIO pins
     * Note: Actual pin mappings depend on hardware design
     */
    struct GPIOPin {
        gpio_type* port;
        uint32_t pin;
    };

    /**
     * @brief Initialize GPIO peripheral and pins for digital inputs
     * @return true on success, false on failure
     */
    static bool Init(void);

    /**
     * @brief Read digital value from specified input channel
     * @param input_number Input number (1-11)
     * @return true for HIGH, false for LOW, or false on error
     */
    static bool ReadDigitalInput(uint8_t input_number);

    /**
     * @brief Configure pull-up/pull-down for digital input
     * @param input_number Input number (1-11)
     * @param pull_type GPIO_PULL_UP, GPIO_PULL_DOWN, or GPIO_PULL_NONE
     * @return true on success, false on failure
     */
    static bool ConfigurePull(uint8_t input_number, gpio_pull_type pull_type);

    /**
     * @brief Check if input is configured for digital mode
     * @param input_number Input number (1-11)
     * @return true if configured for digital input
     */
    static bool IsDigitalMode(uint8_t input_number);

private:
    /**
     * @brief Configure GPIO pins for all digital inputs
     */
    static void ConfigurePins(void);

    /**
     * @brief Get GPIO pin mapping for input number
     * @param input_number Input number (1-11)
     * @return GPIO pin structure
     */
    static GPIOPin GetPinMapping(uint8_t input_number);

    // GPIO pin mappings (to be configured based on actual hardware)
    static const GPIOPin INPUT_PINS[11];
};

#endif // __GPIO_INPUT_DRIVER_H__
