# Документация структуры регистров P-Block  

## Обзор  

Структура `PBlockRegisters_t` предоставляет типобезопасный доступ к регистрам Modbus RTU устройства P‑Block и соответствует текущему заголовку `P-Block-struct.h`.  

P‑Block содержит:  
- **12 дискретных релейных выходов** (включая 1 аварийный)  
- **11 универсальных входов** (аналоговые, цифровые, температурные)  
- **6 аналоговых выходов**  
- **Диагностические и статусные регистры**  

## Организация структуры  

```cpp
struct PBlockRegisters_t  
{  
    // Статические данные  
    static HoldingRegisters_t holding_registers;  
    static Coils_t coils;  
    static UniversalInput_t universal_inputs[11];  
    static uint16_t analog_output[6];  

    // Зарезервированные входные регистры аварийного блока (нестатические)  
    uint16_t emergency_battery_voltage;  
    uint16_t emergency_room_state_1;  
    uint16_t emergency_room_state_2;  

    // Статические функции-члены (API)  
    static void Init(void);  
    static void UpdateInputs(void);  
    static void SetRelay(uint8_t relay_number, bool state);  
    static bool GetRelay(uint8_t relay_number);  
    static void SetAnalogOutput(uint8_t output_number, uint16_t value);  
    static uint16_t GetAnalogOutput(uint8_t output_number);  
    static void SetInputMode(uint8_t input_number, uint16_t mode);  
    static uint16_t GetInputMode(uint8_t input_number);  
    static uint16_t GetUniversalInput(uint8_t input_number);  
    static uint16_t GetUniversalInputType(uint8_t input_number);  
    static bool GetUniversalInputDiscrete(uint8_t input_number);  
    static void LogError(uint16_t error_code);  
    static void ClearErrorStatus(void);  
    static void IncrementBootCount(void);  
};  
```

## Holding Registers (Чтение/Запись)  

Holding registers содержат конфигурацию и диагностику. Аналоговые выходы представлены отдельным статическим массивом `analog_output[6]`.  

### Аналоговые выходы (Адреса 0–5)  
```cpp
static uint16_t analog_output[6];  // 0–10000 мВ  
```

| Индекс | Клемма | Адрес Modbus | Описание |  
|--------|--------|--------------|----------|  
| 0 | B3 | 40001 | Аналоговый выход 1 |  
| 1 | B16 | 40002 | Аналоговый выход 2 |  
| 2 | B2-A | 40003 | Аналоговый выход 3 |  
| 3 | B2-B | 40004 | Аналоговый выход 4 |  
| 4 | B15-A | 40005 | Аналоговый выход 5 |  
| 5 | B15-B | 40006 | Аналоговый выход 6 |  

**Диапазон:** 0–10000 мВ  
**Тип:** `uint16_t`  

### Конфигурация гибридных каналов (Адрес 119)  
```cpp
HybridConfigType hybrid_config; // 0=4 выхода, 2=2 входа (B), 4=4 доп. входа  
```

### Диагностика  
```cpp
uint16_t status;        // 0=OK, 1–65535=код ошибки  
uint16_t boot_count;    // инкремент при каждом запуске  
uint16_t error_log[10]; // ERR1..ERR10 (индекс 0 — последняя)  
```  

## Универсальные входы  

### Типы и структура  
```cpp
enum class UniversalInputType : uint8_t  
{  
    ANALOG = 0,     // 0–10000 мВ  
    DIGITAL = 1,    // 0/1 (счётчик до 65535)  
    DOL12_TEMP = 2, // датчик температуры Dol12  
    TAFCO_TEMP = 3  // датчик температуры Tafco  
};  

struct UniversalInput_t  
{  
    UniversalInputType input_type;  
    uint16_t analog_value;         // 0–10000 мВ  
    uint16_t discrete_value;       // цифровое/счётчик  
};  
```

### Массив универсальных входов (Адреса 0–10)  
```cpp
static UniversalInput_t universal_inputs[11];  
```

| Индекс | Клемма | Адрес Modbus | Описание |  
|--------|--------|--------------|----------|  
| 0 | B5 | 30001 | Универсальный вход 1 |  
| 1 | B7 | 30002 | Универсальный вход 2 |  
| 2 | B18 | 30003 | Универсальный вход 3 |  
| 3 | B9 | 30004 | Универсальный вход 4 |  
| 4 | B20 | 30005 | Универсальный вход 5 |  
| 5 | B2 | 30006 | Универсальный вход 6 |  
| 6 | B15 | 30007 | Универсальный вход 7 |  
| 7 | B2-A | 30008 | Универсальный вход 8 |  
| 8 | B2-B | 30009 | Универсальный вход 9 |  
| 9 | B15-A | 30010 | Универсальный вход 10 |  
| 10 | B15-B | 30011 | Универсальный вход 11 |  

**Диапазоны:**  
- Аналоговый: 0–10000 мВ  
- Цифровой/счётчик: 0–65535  
- Температурные режимы: 0–10000 мВ  

## Входные регистры (зарезервировано)  

Поля состояния аварийного блока присутствуют в `PBlockRegisters_t` как нестатические.  

### Данные аварийного блока (Адреса 20–22)  
```cpp
uint16_t emergency_battery_voltage; // мВ  
uint16_t emergency_room_state_1;    // состояние помещения 1  
uint16_t emergency_room_state_2;    // состояние помещения 2  
```  

Примечание: Реализация чтения появится после разработки аппаратуры.  

## Coils (битовые регистры)  

### Релейные выходы (Адреса 0–12)  
```cpp
struct Coils_t  
{  
    struct RelayBits_t  
    {  
        bool relay1 : 1;          // адрес 0  
        bool relay2 : 1;          // адрес 1  
        bool relay3 : 1;          // адрес 2  
        bool relay4 : 1;          // адрес 3  
        bool relay5 : 1;          // адрес 4  
        bool relay6 : 1;          // адрес 5  
        bool relay7 : 1;          // адрес 6  
        bool relay8 : 1;          // адрес 7  
        bool relay9 : 1;          // адрес 8  
        bool relay10 : 1;         // адрес 9  
        bool relay11 : 1;         // адрес 10  
        bool relay12 : 1;         // адрес 11  
        bool emergency_relay : 1; // адрес 12  
        bool : 3;                 // заполнение  
    } relay_bits;  
};  
```

| Поле | Адрес Modbus | Описание | Клемма |  
|------|--------------|----------|--------|  
| relay1 | 00001 | Реле 1 | B30-32 |  
| relay2 | 00002 | Реле 2 | B33-35 |  
| relay3 | 00003 | Реле 3 | B36-38 |  
| relay4 | 00004 | Реле 4 | B39-41 |  
| relay5 | 00005 | Реле 5 | B42-44 |  
| relay6 | 00006 | Реле 6 | B45-47 |  
| relay7 | 00007 | Реле 7 | B50-52 |  
| relay8 | 00008 | Реле 8 | B53-55 |  
| relay9 | 00009 | Реле 9 | B56-58 |  
| relay10 | 00010 | Реле 10 | B59-61 |  
| relay11 | 00011 | Реле 11 | B62-64 |  
| relay12 | 00012 | Реле 12 | B65-67 |  
| emergency_relay | 00013 | Аварийное реле | B12-B13AB |  

**Значения:**
- `0xFF00` (ВКЛ) - Реле активировано
- `0x0000` (ВЫКЛ) - Реле деактивировано

## Дискретные входы (логическое соответствие)  

При режиме DIGITAL состояние универсального входа читается через дискретные входы Modbus (адреса 0–10). В проекте структура `DiscreteInputs_t` не определена; соответствие логическое.  

| Универсальный вход | Дискретный адрес | Клемма |  
|--------------------|-------------------|--------|  
| 1 | 00001 | B5 |  
| 2 | 00002 | B7 |  
| 3 | 00003 | B18 |  
| 4 | 00004 | B9 |  
| 5 | 00005 | B20 |  
| 6 | 00006 | B2 |  
| 7 | 00007 | B15 |  
| 8 | 00008 | B2-A |  
| 9 | 00009 | B2-B |  
| 10 | 00010 | B15-A |  
| 11 | 00011 | B15-B |  

## Функции API  

### Статические функции-члены  
```cpp
void PBlockRegisters_t::Init(void);  
void PBlockRegisters_t::UpdateInputs(void);  
void PBlockRegisters_t::SetAnalogOutput(uint8_t output_number, uint16_t value);  
uint16_t PBlockRegisters_t::GetAnalogOutput(uint8_t output_number);  
void PBlockRegisters_t::SetRelay(uint8_t relay_number, bool state);  
bool PBlockRegisters_t::GetRelay(uint8_t relay_number);  
void PBlockRegisters_t::SetInputMode(uint8_t input_number, uint16_t mode);  
uint16_t PBlockRegisters_t::GetInputMode(uint8_t input_number);  
uint16_t PBlockRegisters_t::GetUniversalInput(uint8_t input_number);  
uint16_t PBlockRegisters_t::GetUniversalInputType(uint8_t input_number);  
bool PBlockRegisters_t::GetUniversalInputDiscrete(uint8_t input_number);  
void PBlockRegisters_t::LogError(uint16_t error_code);  
void PBlockRegisters_t::ClearErrorStatus(void);  
void PBlockRegisters_t::IncrementBootCount(void);  
```

**Параметры:**  
- `output_number`: 1–6 (индексы массива 0–5)  
- `value`: 0–10000 мВ  
- `relay_number`: 1–13 (13 = аварийное реле)  
- `state`: true = ВКЛ (0xFF00), false = ВЫКЛ (0x0000)  
- `input_number`: 1–11 (индексы массива 0–10)  
- `mode`: 0=аналоговый, 1=цифровой, 2=Dol12, 3=Tafco  

## Примеры использования  

### Инициализация и конфигурация  
```cpp
PBlockRegisters_t::Init();  
for (uint8_t i = 1; i <= 5; i++) { PBlockRegisters_t::SetInputMode(i, 0); }  
for (uint8_t i = 6; i <= 11; i++) { PBlockRegisters_t::SetInputMode(i, 1); }  
PBlockRegisters_t::SetAnalogOutput(1, 5000);  
PBlockRegisters_t::SetAnalogOutput(2, 2500);  
```

### Управление реле  
```cpp
for (uint8_t i = 1; i <= 3; i++) { PBlockRegisters_t::SetRelay(i, true); }  
bool emergency_active = PBlockRegisters_t::GetRelay(13);  
```

### Чтение данных  
```cpp
PBlockRegisters_t::UpdateInputs();  
uint16_t v1 = PBlockRegisters_t::GetUniversalInput(1);  
uint16_t v2 = PBlockRegisters_t::GetUniversalInput(2);  
uint16_t ao1 = PBlockRegisters_t::GetAnalogOutput(1);  
```

### Ошибки  
```cpp
if (PBlockRegisters_t::holding_registers.status != 0) {  
    PBlockRegisters_t::LogError(PBlockRegisters_t::holding_registers.status);  
    PBlockRegisters_t::ClearErrorStatus();  
}  
```

## Коды ошибок (пример)  

| Код | Описание |  
|-----|----------|  
| 0 | Нормальная работа |  
| 1 | Перегрев процессора |  
| 2 | Сбой аналогового входа |  
| 3 | Сбой аналогового выхода |  
| 4 | Несоответствие конфигурации входа |  
| 5 | Сбой реле |  
| 6 | Сбой памяти |  
| 7 | Ошибка питания |  

## Макет памяти  

Структура упакована директивой `#pragma pack(push, 1)`, как указано в заголовке, для выравнивания регистров Modbus. Хранилище реализовано статическими членами.  

## Замечания по интеграции  

- Обеспечьте потокобезопасность при доступе из задач RTOS.  
- Аналоговые значения в мВ (0–10000).  
- DIGITAL-режим может использовать 16-битный диапазон для счётчиков.  
- `error_log` хранит 10 последних ошибок, индекс 0 — последняя.  
- `boot_count` инкрементируется на каждом запуске.  

## Ссылки на файлы  

- **Заголовок:** `P-Block-struct.h`  
- **Реализация:** `P-Block-struct.cpp`  
- **Спецификация протокола:** `modbus-rtu.md`  

Эта документация отражает актуальные `P-Block-struct.h` и реализацию.  
