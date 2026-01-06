#include "GPIOInputDriver.h"
#include "at32f403a_407_crm.h"
#include "P-Block-struct.h"

// Define GPIO pin mappings for universal inputs
// Note: These mappings need to be updated based on actual hardware design
const GPIOInputDriver::GPIOPin GPIOInputDriver::INPUT_PINS[11] = {
    {GPIOB, GPIO_PINS_0},   // Input 1
    {GPIOB, GPIO_PINS_1},   // Input 2
    {GPIOB, GPIO_PINS_2},   // Input 3
    {GPIOB, GPIO_PINS_3},   // Input 4
    {GPIOB, GPIO_PINS_4},   // Input 5
    {GPIOB, GPIO_PINS_5},   // Input 6
    {GPIOB, GPIO_PINS_6},   // Input 7
    {GPIOB, GPIO_PINS_7},   // Input 8
    {GPIOB, GPIO_PINS_8},   // Input 9
    {GPIOB, GPIO_PINS_9},   // Input 10
    {GPIOB, GPIO_PINS_10}   // Input 11
};

bool GPIOInputDriver::Init(void) {
    // Enable GPIO clocks for input ports
    // Assuming all inputs are on GPIOB - adjust based on actual hardware
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

    // Configure GPIO pins for digital input
    ConfigurePins();

    return true;
}

bool GPIOInputDriver::ReadDigitalInput(uint8_t input_number) {
    if (input_number < 1 || input_number > 11) {
        return false; // Error case
    }

    GPIOPin pin_mapping = GetPinMapping(input_number);

    // Read GPIO pin state
    uint32_t pin_state = gpio_input_data_bit_read(pin_mapping.port, pin_mapping.pin);

    return (pin_state != 0);
}

bool GPIOInputDriver::ConfigurePull(uint8_t input_number, gpio_pull_type pull_type) {
    if (input_number < 1 || input_number > 11) {
        return false;
    }

    GPIOPin pin_mapping = GetPinMapping(input_number);

    // Reconfigure pin with new pull setting
    gpio_init_type gpio_init_struct;
    gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
    gpio_init_struct.gpio_pull = pull_type;
    gpio_init_struct.gpio_pins = pin_mapping.pin;

    gpio_init(pin_mapping.port, &gpio_init_struct);

    return true;
}

bool GPIOInputDriver::IsDigitalMode(uint8_t input_number) {
    if (input_number < 1 || input_number > 11) {
        return false;
    }

    // Check if input is configured for digital mode
    UniversalInputType mode = PBlockRegisters_t::GetInputMode(input_number);
    return (mode == UniversalInputType::DIGITAL);
}

void GPIOInputDriver::ConfigurePins(void) {
    gpio_init_type gpio_init_struct;

    // Configure all input pins as digital inputs with pull-up
    gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;  // Default to pull-up

    for (uint8_t i = 0; i < 11; i++) {
        gpio_init_struct.gpio_pins = INPUT_PINS[i].pin;
        gpio_init(INPUT_PINS[i].port, &gpio_init_struct);
    }
}

GPIOInputDriver::GPIOPin GPIOInputDriver::GetPinMapping(uint8_t input_number) {
    if (input_number >= 1 && input_number <= 11) {
        return INPUT_PINS[input_number - 1];
    }

    // Return default mapping for error case
    return {GPIOB, GPIO_PINS_0};
}
