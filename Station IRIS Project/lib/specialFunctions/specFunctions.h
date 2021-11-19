#pragma once

// Inclusões
#include <Arduino.h>
#include <FreeRTOS.h>
#include "pinout.h"
#include "errors.h"
#include "debug.h"

namespace spc
{
  class SpecialFunctions
  {
    public:
      static void IRAM_ATTR resetModule();
      static uint32_t asm_addr(uint8_t *addr);
  };
}