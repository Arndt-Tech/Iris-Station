#ifndef GENERAL_TASKS_H
#define GENERAL_TASKS_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "Bluetooth.h"
#include "task.h"
#include "ChipID.h"
#include "_EEPROM.h"

// Funções
void configBegin(networkLora *gtw);
void setupDataSystem(networkLora *gtw);


#endif