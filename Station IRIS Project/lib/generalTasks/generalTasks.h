#ifndef GENERAL_TASKS_H
#define GENERAL_TASKS_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "OLED.h"
#include "Bluetooth.h"
#include "_LoRa.h"
#include "multiCore.h"
#include "specFunctions.h"
#include "ChipID.h"
#include "_EEPROM.h"
#include "GPIO.h"

// Mapeamento de hardware
#define pin_resetEEPROM 17

// Definições
#define resetTmrOF 3

// Funções
void configBegin();
void setupDataSystem(networkLora *gtw);
void resetClear();
void valve(uint8_t state);

#endif