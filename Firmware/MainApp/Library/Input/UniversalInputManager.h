#ifndef __UNIVERSAL_INPUT_MANAGER_H__
#define __UNIVERSAL_INPUT_MANAGER_H__

#include <stdint.h>
#include "P-Block-struct.h"
#include "GPIOInputDriver.h"
// #include "ADCDriver.h"

/**
 * @brief Universal Input Manager - Coordinates ADC, GPIO, and temperature sensor interfaces
 * Manages 11 universal inputs with 4 configurable modes:
 * - ANALOG: 0-10V ADC reading
 * - DIGITAL: GPIO digital input
 * - DOL12_TEMP: Dol12 temperature sensor
 * - TAFCO_TEMP: Tafco temperature sensor
 */
class UniversalInputManager {
public:
    /**
     * @brief Initialize all input drivers and interfaces
     * @return true on success, false on failure
     */
    static bool Init(void);

    /**
     * @brief Update all universal inputs based on their configured modes
     * Reads from appropriate hardware interfaces (ADC/GPIO/sensors)
     * Updates PBlockRegisters_t with new values
     */
    static void UpdateAllInputs(void);

    /**
     * @brief Update single input based on its mode
     * @param input_number Input number (1-11)
     */
    static void UpdateInput(uint8_t input_number);

    /**
     * @brief Configure input mode and reconfigure hardware accordingly
     * @param input_number Input number (1-11)
     * @param mode New input mode
     * @return true on success, false on failure
     */
    static bool ConfigureInputMode(uint8_t input_number, UniversalInputType mode);

private:
    /**
     * @brief Update analog input from ADC
     * @param input_number Input number (1-11)
     */
    static void UpdateAnalogInput(uint8_t input_number);

    /**
     * @brief Update digital input from GPIO
     * @param input_number Input number (1-11)
     */
    static void UpdateDigitalInput(uint8_t input_number);

    /**
     * @brief Update temperature input from Dol12 sensor
     * @param input_number Input number (1-11)
     */
    static void UpdateDol12Temperature(uint8_t input_number);

    /**
     * @brief Update temperature input from Tafco sensor
     * @param input_number Input number (1-11)
     */
    static void UpdateTafcoTemperature(uint8_t input_number);

    /**
     * @brief Convert temperature to millivolts equivalent for Modbus registers
     * @param temperature_celsius Temperature in Celsius
     * @return Equivalent millivolts (0-10000 range)
     */
    static uint16_t TemperatureToMillivolts(float temperature_celsius);

    // Initialization flags
    static bool adc_initialized_;
    static bool gpio_initialized_;
};

#endif // __UNIVERSAL_INPUT_MANAGER_H__
