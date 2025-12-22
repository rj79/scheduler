#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "Arduino.h"

#ifdef ARDUINO_ARCH_AVR
#include <avr/wdt.h>
#endif

#ifdef ARDUINO_ARCH_ESP32
#include <esp_task_wdt.h>
#endif

void feedWatchdog()
{
    #ifdef ARDUINO_ARCH_ESP32
    esp_task_wdt_reset();
    #endif

    #ifdef ARDUINO_ARCH_ESP8266
    ESP.wdtFeed();
    #endif

    #ifdef ARDUINO_ARCH_AVR
    wdt_reset();
    #endif
}

#endif