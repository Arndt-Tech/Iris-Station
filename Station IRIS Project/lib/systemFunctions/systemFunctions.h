#ifndef GENERAL_TASKS_H
#define GENERAL_TASKS_H

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "errors.h"
#include "Bluetooth.h"
#include "task.h"
#include "ChipID.h"
#include "_EEPROM.h"
#include "GPS.h"

// Funções
void configBegin(networkLora *gtw, GPS *gps);
void setupDataSystem(networkLora *gtw);


#endif