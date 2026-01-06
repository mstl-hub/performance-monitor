#include "UnitControlApp.h"

void unitControlFun(void* parameters) {


    for (;;) {
        unit_control_wave();
        wdt_notify_to_subject({.unit_control = true});
        vTaskDelay(500);
    }
}

