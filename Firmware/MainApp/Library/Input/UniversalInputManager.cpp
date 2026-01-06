#include "UniversalInputManager.h"
// #include "ADCDriver.h"  // TODO: Recreate ADCDriver.h/cpp for ADC functionality



// Static member definitions
bool UniversalInputManager::adc_initialized_ = false;
bool UniversalInputManager::gpio_initialized_ = false;

bool UniversalInputManager::Init(void) {
    bool success = true;

    // Initialize ADC driver
    // if (!ADCDriver::Init()) {
    //     // Log error: ADC initialization failed
    //     success = false;
    // } else {
    //     adc_initialized_ = true;
    // }

    // Initialize GPIO driver
    if (!GPIOInputDriver::Init()) {
        // Log error: GPIO initialization failed
        success = false;
    } else {
        gpio_initialized_ = true;
    }

    // TODO: Initialize temperature sensor interfaces when implemented
    // if (!Dol12Sensor::Init()) { ... }
    // if (!TafcoSensor::Init()) { ... }

    return success;
}

void UniversalInputManager::UpdateAllInputs(void) {
    for (uint8_t input_num = 1; input_num <= 11; input_num++) {
        UpdateInput(input_num);
    }
}

void UniversalInputManager::UpdateInput(uint8_t input_number) {
    if (input_number < 1 || input_number > 11) {
        return;
    }

    UniversalInputType mode = PBlockRegisters_t::GetInputMode(input_number);

    switch (mode) {
        case UniversalInputType::ANALOG:
            UpdateAnalogInput(input_number);
            break;

        case UniversalInputType::DIGITAL:
            UpdateDigitalInput(input_number);
            break;

        case UniversalInputType::DOL12_TEMP:
            UpdateDol12Temperature(input_number);
            break;

        case UniversalInputType::TAFCO_TEMP:
            UpdateTafcoTemperature(input_number);
            break;

        default:
            // Invalid mode, set to default analog
            PBlockRegisters_t::SetInputMode(input_number, UniversalInputType::ANALOG);
            UpdateAnalogInput(input_number);
            break;
    }
}

bool UniversalInputManager::ConfigureInputMode(uint8_t input_number, UniversalInputType mode) {
    if (input_number < 1 || input_number > 11) {
        return false;
    }

    // Validate that required hardware is initialized
    switch (mode) {
        case UniversalInputType::ANALOG:
            if (!adc_initialized_) {
                return false;
            }
            break;

        case UniversalInputType::DIGITAL:
            if (!gpio_initialized_) {
                return false;
            }
            break;

        case UniversalInputType::DOL12_TEMP:
        case UniversalInputType::TAFCO_TEMP:
            // TODO: Check temperature sensor initialization
            break;

        default:
            return false;
    }

    // Update the configuration
    PBlockRegisters_t::SetInputMode(input_number, mode);

    // Immediately update the input with new mode
    UpdateInput(input_number);

    return true;
}

void UniversalInputManager::UpdateAnalogInput(uint8_t input_number) {
    if (!adc_initialized_) {
        // Set error value
        PBlockRegisters_t::universal_inputs[input_number - 1].analog_value = 0xFFFF;
        PBlockRegisters_t::universal_inputs[input_number - 1].discrete_value = 0;
        return;
    }

    // uint16_t adc_value_mv = ADCDriver::ReadAnalogInput(input_number);

    // if (adc_value_mv == 0xFFFF) {
    //     // ADC read error
    //     PBlockRegisters_t::universal_inputs[input_number - 1].analog_value = 0xFFFF;
    //     PBlockRegisters_t::universal_inputs[input_number - 1].discrete_value = 0;
    //     // TODO: Log error
    // } else {
    //     PBlockRegisters_t::universal_inputs[input_number - 1].analog_value = adc_value_mv;
    //     // For analog mode, discrete value represents over/under voltage
    //     PBlockRegisters_t::universal_inputs[input_number - 1].discrete_value =
    //         (adc_value_mv > 9500) ? 1 : 0;  // High if > 9.5V
    // }
}

void UniversalInputManager::UpdateDigitalInput(uint8_t input_number) {
    if (!gpio_initialized_) {
        // Set error state
        PBlockRegisters_t::universal_inputs[input_number - 1].analog_value = 0;
        PBlockRegisters_t::universal_inputs[input_number - 1].discrete_value = 0;
        return;
    }

    bool digital_value = GPIOInputDriver::ReadDigitalInput(input_number);

    // For digital mode, analog value represents digital state in mV equivalent
    PBlockRegisters_t::universal_inputs[input_number - 1].analog_value =
        digital_value ? 10000 : 0;  // 10V = HIGH, 0V = LOW

    PBlockRegisters_t::universal_inputs[input_number - 1].discrete_value = digital_value ? 1 : 0;
}

void UniversalInputManager::UpdateDol12Temperature(uint8_t input_number) {
    // TODO: Implement Dol12 sensor reading
    // For now, set placeholder values
    PBlockRegisters_t::universal_inputs[input_number - 1].analog_value = 2500;  // 25°C equivalent
    PBlockRegisters_t::universal_inputs[input_number - 1].discrete_value = 0;   // Normal state

    // When implemented:
    // float temperature = Dol12Sensor::ReadTemperature(input_number);
    // uint16_t temp_mv = TemperatureToMillivolts(temperature);
    // PBlockRegisters_t::universal_inputs[input_number - 1].analog_value = temp_mv;
    // PBlockRegisters_t::universal_inputs[input_number - 1].discrete_value = (temperature > 50.0f) ? 1 : 0;
}

void UniversalInputManager::UpdateTafcoTemperature(uint8_t input_number) {
    // TODO: Implement Tafco sensor reading
    // For now, set placeholder values
    PBlockRegisters_t::universal_inputs[input_number - 1].analog_value = 2000;  // 20°C equivalent
    PBlockRegisters_t::universal_inputs[input_number - 1].discrete_value = 0;   // Normal state

    // When implemented:
    // float temperature = TafcoSensor::ReadTemperature(input_number);
    // uint16_t temp_mv = TemperatureToMillivolts(temperature);
    // PBlockRegisters_t::universal_inputs[input_number - 1].analog_value = temp_mv;
    // PBlockRegisters_t::universal_inputs[input_number - 1].discrete_value = (temperature > 50.0f) ? 1 : 0;
}

uint16_t UniversalInputManager::TemperatureToMillivolts(float temperature_celsius) {
    // Convert temperature to millivolts equivalent for Modbus registers
    // Assuming 0°C = 0mV, 100°C = 10000mV linear scale
    // This is just an example mapping - adjust based on actual requirements

    if (temperature_celsius < 0.0f) {
        return 0;
    } else if (temperature_celsius > 100.0f) {
        return 10000;
    } else {
        return static_cast<uint16_t>((temperature_celsius / 100.0f) * 10000.0f);
    }
}
