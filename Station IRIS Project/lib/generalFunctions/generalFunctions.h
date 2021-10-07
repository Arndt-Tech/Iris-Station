#ifndef GENERAL_TASKS_H
#define GENERAL_TASKS_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "Bluetooth.h"
#include "task.h"
#include "ChipID.h"
#include "_EEPROM.h"

// Definições
#define buzzChannel(a) (uint8_t)(a)

// Funções
void configBegin(networkLora *gtw);
void setupDataSystem(networkLora *gtw);
void setupBuzzer();
void error(int8_t long_bip, int8_t short_bip);
void long_bip();
void short_bip();

#endif