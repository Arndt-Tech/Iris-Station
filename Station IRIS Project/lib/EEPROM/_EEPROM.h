#ifndef _EEPROM_H
#define _EEPROM_H

// Incluões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "errors.h"
#include "EEPROM.h"

// EEPROM Address
#define EEPROM_SIZE 128 // 128 bytes reservados para EEPROM

// Local chipID
#define loChID_addr_min 0
#define loChID_addr_max 10

// Sender chipID
#define chID_addr_min 15
#define chID_addr_max 25

// Funções
bool verifyEEPROM(uint8_t addr_min);
bool writeEEPROM(uint32_t data, uint8_t addr);
bool clearEEPROM(uint8_t addr0, uint8_t addr1);
uint32_t readEEPROM(uint8_t addr);

#endif