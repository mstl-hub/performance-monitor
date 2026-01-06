#include "P-Block-struct.h"


// Define static data members based on P-Block-struct.h  
PBlockRegisters_t::HoldingRegisters_t PBlockRegisters_t::holding_registers;  
Coils_t PBlockRegisters_t::coils;  
UniversalInput_t PBlockRegisters_t::universal_inputs[11];  
uint16_t PBlockRegisters_t::analog_output[6];  

/**
 * @brief Initialize all registers with default values (static member function)
 */
void PBlockRegisters_t::Init(void)  
{  
    // Zero all holding registers and coils  
    holding_registers = {};  
    coils = {};  

    // Zero analog outputs  
    for (uint8_t i = 0; i < 6; i++)  
    {  
        analog_output[i] = 0;  
    }  

    // Initialize universal inputs to default type and zero values  
    for (uint8_t i = 0; i < 11; i++)  
    {  
        universal_inputs[i].input_type = UniversalInputType::ANALOG;  
        universal_inputs[i].analog_value = 0;  
        universal_inputs[i].discrete_value = 0;  
    }  

    // Set default non-zero values  
    holding_registers.hybrid_config = HybridConfigType::TWO_INPUTS;  
    holding_registers.boot_count = 1;  
}  

/**
 * @brief Update input registers with current sensor readings (static member function)
 * This function should be called periodically to update sensor values
 */
void PBlockRegisters_t::UpdateInputs(void)
{
    // Use UniversalInputManager to update all inputs based on their configured modes
    // This replaces the simulated values with actual hardware reads
    // UniversalInputManager::UpdateAllInputs();
}  

/**
 * @brief Set relay state (coil) (static member function)
 * @param relay_number Relay number (1-13, where 13 is emergency relay)
 * @param state True for ON (0xFF00), False for OFF (0x0000)
 */
void PBlockRegisters_t::SetRelay(uint8_t relay_number, bool state)  
{  
    switch (relay_number)
    {
        case 1:  coils.relay_bits.relay1 = state; break;
        case 2:  coils.relay_bits.relay2 = state; break;
        case 3:  coils.relay_bits.relay3 = state; break;
        case 4:  coils.relay_bits.relay4 = state; break;
        case 5:  coils.relay_bits.relay5 = state; break;
        case 6:  coils.relay_bits.relay6 = state; break;
        case 7:  coils.relay_bits.relay7 = state; break;
        case 8:  coils.relay_bits.relay8 = state; break;
        case 9:  coils.relay_bits.relay9 = state; break;
        case 10: coils.relay_bits.relay10 = state; break;
        case 11: coils.relay_bits.relay11 = state; break;
        case 12: coils.relay_bits.relay12 = state; break;
        case 13: coils.relay_bits.emergency_relay = state; break;
        default: break;  
    }
}  

/**
 * @brief Get relay state (coil) (static member function)
 * @param relay_number Relay number (1-13, where 13 is emergency relay)
 * @return True if ON, False if OFF
 */
bool PBlockRegisters_t::GetRelay(uint8_t relay_number)  
{  
    switch (relay_number)
    {
        case 1:  return coils.relay_bits.relay1;
        case 2:  return coils.relay_bits.relay2;
        case 3:  return coils.relay_bits.relay3;
        case 4:  return coils.relay_bits.relay4;
        case 5:  return coils.relay_bits.relay5;
        case 6:  return coils.relay_bits.relay6;
        case 7:  return coils.relay_bits.relay7;
        case 8:  return coils.relay_bits.relay8;
        case 9:  return coils.relay_bits.relay9;
        case 10: return coils.relay_bits.relay10;
        case 11: return coils.relay_bits.relay11;
        case 12: return coils.relay_bits.relay12;
        case 13: return coils.relay_bits.emergency_relay;
        default: return false;  
    }
}  

/**
 * @brief Set analog output value (static member function)
 * @param output_number Output number (1-6, corresponds to array index 0-5)
 * @param value Value in mV (0-10000)
 */
void PBlockRegisters_t::SetAnalogOutput(uint8_t output_number, uint16_t value)  
{  
    if (value > 10000) { value = 10000; }  
    if (output_number >= 1 && output_number <= 6)  
    {  
        analog_output[output_number - 1] = value;  
    }  
}  

/**
 * @brief Get analog output value (static member function)
 * @param output_number Output number (1-6, corresponds to array index 0-5)
 * @return Value in mV (0-10000)
 */
uint16_t PBlockRegisters_t::GetAnalogOutput(uint8_t output_number)  
{  
    if (output_number >= 1 && output_number <= 6)  
    {  
        return analog_output[output_number - 1];  
    }  
    return 0;  
}  

/**
 * @brief Set universal input mode (static member function)
 * @param input_number Input number (1-11)
 * @param mode 0=analog, 1=digital, 2=Dol12 temp, 3=Tafco temp
 */
void PBlockRegisters_t::SetInputMode(uint8_t input_number, UniversalInputType mode)  
{  
    if (input_number >= 1 && input_number <= 11)  
    {  
        universal_inputs[input_number - 1].input_type = static_cast<UniversalInputType>(mode);  
    }  
}  

/**
 * @brief Get universal input mode (static member function)
 * @param input_number Input number (1-11)
 * @return Mode: 0=analog, 1=digital, 2=Dol12 temp, 3=Tafco temp
 */
 UniversalInputType PBlockRegisters_t::GetInputMode(uint8_t input_number)  
{  
    if (input_number >= 1 && input_number <= 11)  
    {  
        return universal_inputs[input_number - 1].input_type;  
    }  
    return UniversalInputType::ANALOG;  //TODO: return error
}  

/**
 * @brief Get universal input analog value (static member function)
 * @param input_number Input number (1-11, corresponds to array index 0-10)
 * @return Analog input value (0-10000 mV) - maintains backward compatibility
 */
uint16_t PBlockRegisters_t::GetUniversalInput(uint8_t input_number)  
{  
    if (input_number >= 1 && input_number <= 11)  
    {  
        return universal_inputs[input_number - 1].analog_value;  
    }  
    return 0;  
}  


/**
 * @brief Get universal input discrete value (static member function)
 * @param input_number Input number (1-11, corresponds to array index 0-10)
 * @return Discrete/digital input value (true/false)
 */
bool PBlockRegisters_t::GetUniversalInputDiscrete(uint8_t input_number)  
{  
    if (input_number >= 1 && input_number <= 11)  
    {  
        return universal_inputs[input_number - 1].discrete_value != 0;  
    }  
    return false;  
}  

/**
 * @brief Log an error to the error log buffer (static member function)
 * @param error_code Error code to log
 */
void PBlockRegisters_t::LogError(uint16_t error_code)  
{  
    for (int8_t i = 9; i > 0; i--)  
    {  
        holding_registers.error_log[i] = holding_registers.error_log[i - 1];  
    }  
    holding_registers.error_log[0] = error_code;  
    holding_registers.status = error_code;  
}  

/**
 * @brief Clear error status (set to normal operation) (static member function)
 */
void PBlockRegisters_t::ClearErrorStatus(void)  
{  
    holding_registers.status = 0;  
}  

/**
 * @brief Increment boot counter (static member function)
 */
void PBlockRegisters_t::IncrementBootCount(void)  
{  
    holding_registers.boot_count++;  
}  

