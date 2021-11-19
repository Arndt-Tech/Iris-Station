#pragma once

// Incluões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "errors.h"
#include "debug.h"
#include "EEPROM.h"

// EEPROM Address
#define EEPROM_SIZE 128 // 128 bytes reservados para EEPROM

// Local chipID
#define loChID_addr_min 0
#define loChID_addr_max 10

// Sender chipID
#define chID_addr_min 15
#define chID_addr_max 25

/**
 * @brief Configuration
 * 
 */
namespace cfg
{
  class Log
  {
  public:
    static void begin();
    static bool check(uint8_t addr);
    static bool write(uint32_t data, uint8_t addr);
    static bool clear(uint8_t addr0, uint8_t addr1);
    static uint32_t read(uint8_t addr);
  };
}
